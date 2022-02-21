#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 //* \Module:  Renderer
 * \File:    le_vk_debugger.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_VK_DEBUGGER_H
#define LE_VK_DEBUGGER_H

// LE Includes
#include "le_vk_variable_manager.h"
// STL Includes
#include <iostream>
// GLM Includes
#include <glm/glm.hpp>
#ifdef LE_EDITOR
#include <QDebug>
#endif // !LE_EDITOR
// Reference:
// [1]:https://blog.csdn.net/qq_35312463/article/details/106313766
// [2]:https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VK_EXT_debug_marker.html

class LEVkDebugger {
private:
	static VkDebugReportCallbackEXT ms_vkDebugReportCallback;
	static VkDebugUtilsMessengerEXT ms_messenger;
public:
	LEVkDebugger() = delete;
	~LEVkDebugger() = delete;
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

	// callback function
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallBackFunction(
		VkFlags                    msgFlags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t                   srcObject,
		size_t                     location,
		int32_t                    msgCode,
		const char* layerPrefix,
		const char* msg,
		void* pUserData) {
		// if editor mode, print to console window
		// else print to std::cout
		if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
			qDebug() << "[VK-Error]: " << msg << " at code:" << msgCode << " lines " << location;
		}
		else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
			// qDebug() << "[VK-Warning]: " << msg << " at code:" << msgCode << " lines " << location;
		}
		else if (msgFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
			qDebug() << "[VK-Info]:  " << msg << " at code:" << msgCode << " lines " << location;
		}
		else if (msgFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
			qDebug() << "[VK-Performance]: " << msg << " at code:" << msgCode << " lines " << location;
		}
		else if (msgFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
			qDebug() << "[VK-Report]: " << msg << " at code:" << msgCode << " lines " << location;
		}
		else {
			return VK_FALSE; // Unknown message
		}
		return VK_SUCCESS;
	}

	static void installDebugCallBack();

	static void uninstallDebugCallBack() {

	}

	static bool                              ms_active;
	static bool                              ms_extensionPresent;
	static PFN_vkDebugMarkerSetObjectTagEXT  ms_vkDebugMarkerSetObjectTag;
	static PFN_vkDebugMarkerSetObjectNameEXT ms_vkDebugMarkerSetObjectName;
	static PFN_vkCmdDebugMarkerBeginEXT      ms_vkCmdDebugMarkerBegin;
	static PFN_vkCmdDebugMarkerEndEXT        ms_vkCmdDebugMarkerEnd;
	static PFN_vkCmdDebugMarkerInsertEXT     ms_vkCmdDebugMarkerInsert;

	static void setupDebugMarker(VkDevice device, VkPhysicalDevice physicalDevice) {
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions.data());
		for (auto extension : extensions) {
			if (strcmp(extension.extensionName, VK_EXT_DEBUG_MARKER_EXTENSION_NAME) == 0) {
				ms_extensionPresent = true;
				break;
			}
		}

		if (ms_extensionPresent) {
			ms_vkDebugMarkerSetObjectTag  = (PFN_vkDebugMarkerSetObjectTagEXT)vkGetDeviceProcAddr(device, "vkDebugMarkerSetObjectTagEXT");
			ms_vkDebugMarkerSetObjectName = (PFN_vkDebugMarkerSetObjectNameEXT)vkGetDeviceProcAddr(device, "vkDebugMarkerSetObjectNameEXT");
			ms_vkCmdDebugMarkerBegin      = (PFN_vkCmdDebugMarkerBeginEXT)vkGetDeviceProcAddr(device, "vkCmdDebugMarkerBeginEXT");
			ms_vkCmdDebugMarkerEnd        = (PFN_vkCmdDebugMarkerEndEXT)vkGetDeviceProcAddr(device, "vkCmdDebugMarkerEndEXT");
			ms_vkCmdDebugMarkerInsert     = (PFN_vkCmdDebugMarkerInsertEXT)vkGetDeviceProcAddr(device, "vkCmdDebugMarkerInsertEXT");
			ms_active = (ms_vkDebugMarkerSetObjectName != VK_NULL_HANDLE);
		}
	}

	static void setObjectName(VkDevice device, uint64_t object, VkDebugReportObjectTypeEXT objectType, const char* name) {
		if (ms_active) {
			VkDebugMarkerObjectNameInfoEXT nameInfo = {};
			nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
			nameInfo.objectType = objectType;
			nameInfo.object = object;
			nameInfo.pObjectName = name;
			ms_vkDebugMarkerSetObjectName(device, &nameInfo);
		}
	}

	static void setObjectTag(VkDevice device, uint64_t object, VkDebugReportObjectTypeEXT objectType, uint64_t name, size_t tagSize, const void* tag) {
		if (ms_active) {
			VkDebugMarkerObjectTagInfoEXT tagInfo = {};
			tagInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
			tagInfo.objectType = objectType;
			tagInfo.object = object;
			tagInfo.tagName = name;
			tagInfo.tagSize = tagSize;
			tagInfo.pTag = tag;
			ms_vkDebugMarkerSetObjectTag(device, &tagInfo);
		}
	}

	static void beginRegion(VkCommandBuffer cmdbuffer, const char* pMarkerName, glm::vec4 color) {
		if (ms_active) {
			VkDebugMarkerMarkerInfoEXT markerInfo = {};
			markerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
			memcpy(markerInfo.color, &color[0], sizeof(float) * 4);
			markerInfo.pMarkerName = pMarkerName;
			ms_vkCmdDebugMarkerBegin(cmdbuffer, &markerInfo);
		}
	}

	static void insert(VkCommandBuffer cmdbuffer, std::string markerName, glm::vec4 color) {
		if (ms_active) {
			VkDebugMarkerMarkerInfoEXT markerInfo = {};
			markerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT;
			memcpy(markerInfo.color, &color[0], sizeof(float) * 4);
			markerInfo.pMarkerName = markerName.c_str();
			ms_vkCmdDebugMarkerInsert(cmdbuffer, &markerInfo);
		}
	}

	static void endRegion(VkCommandBuffer cmdBuffer) {
		if (ms_vkCmdDebugMarkerEnd) {
			ms_vkCmdDebugMarkerEnd(cmdBuffer);
		}
	}
};
#endif // !LE_VK_DEBUGGER_H