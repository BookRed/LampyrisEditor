#pragma once
/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_vk_logical_device.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_vk_device.h"

void LEVkDevice::createGlobalInstance() {
	uint32_t deviceCount = -1;
	// get the count of physical device 
	LE_ASSERT(::vkEnumeratePhysicalDevices(vkVariableManager->vkInstance,
		                                   &deviceCount,
		                                   NULL) == VK_SUCCESS);
	// get physical device
	LE_ASSERT(::vkEnumeratePhysicalDevices(vkVariableManager->vkInstance,
		                                  &deviceCount,
		                                   vkVariableManager->gpus) == VK_SUCCESS);

	::vkGetPhysicalDeviceMemoryProperties(vkVariableManager->gpus[0],&vkVariableManager->gpuMemoryProperties[0]);

	::vkGetPhysicalDeviceProperties(vkVariableManager->gpus[0],&vkVariableManager->gpuProperties[0]);

	::vkGetPhysicalDeviceQueueFamilyProperties(vkVariableManager->gpus[0],&vkVariableManager->gpuQueueFamilyCount,nullptr);

	vkVariableManager->gpuQueueFamilyProperty.resize(vkVariableManager->gpuQueueFamilyCount);

	::vkGetPhysicalDeviceQueueFamilyProperties(vkVariableManager->gpus[0], &vkVariableManager->gpuQueueFamilyCount, vkVariableManager->gpuQueueFamilyProperty.data());

	VkDeviceQueueCreateInfo queueInfo = {};
	// flag 'found' is to record whether we find a QueueFamily that supports Graphic work
	// if we find it ,record the index of QueueFamily
	bool graphicsQueueFound = false;
	for(uint32_t i = 0; i < vkVariableManager->gpuQueueFamilyCount; i++) {
		// VK_QUEUE_GRAPHICS_BIT represents a QueueFamily supports Graphic work
		if (vkVariableManager->gpuQueueFamilyProperty[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			// if we find it
			queueInfo.queueFamilyIndex = i;
			vkVariableManager->gpuQueueGraphicsFamilyIndex = i;
			graphicsQueueFound = true;
			break;
		}
	}
	LE_ASSERT_MSG(graphicsQueueFound == true, "Cannot find a valid family queue for graphics work!");
	// declare an array to record the priorities of Queues
	float queuePriorities[1]   = { 0.0 };
	queueInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueInfo.pNext            = NULL;
	queueInfo.queueCount       = 1;
	queueInfo.pQueuePriorities = queuePriorities;
	queueInfo.queueFamilyIndex = vkVariableManager->gpuQueueGraphicsFamilyIndex;

	// a vector to store device extension's name;
	std::vector<const char*> vkDeviceExtensionNames;

	// VK_KHR_SWAPCHAIN_EXTENSION_NAME : whether GPU support swapchain
	vkDeviceExtensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	vkDeviceExtensionNames.push_back(VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME);
	vkDeviceExtensionNames.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
	vkDeviceExtensionNames.push_back(VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME);
	vkDeviceExtensionNames.push_back(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);

	typedef struct VkPhysicalDeviceDynamicRenderingFeaturesKHR {
		VkStructureType    sType;
		void*              pNext;
		VkBool32           dynamicRendering;
	} VkPhysicalDeviceDynamicRenderingFeaturesKHR;

	VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeature;
	dynamicRenderingFeature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
	dynamicRenderingFeature.pNext = nullptr;
	dynamicRenderingFeature.dynamicRendering = true;

	// fill the VkDeviceCreateInfo struct
	VkDeviceCreateInfo deviceInfo      = {};
	deviceInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pNext                   = &dynamicRenderingFeature;
	deviceInfo.queueCreateInfoCount    = 1;
	deviceInfo.pQueueCreateInfos       = &queueInfo;
	deviceInfo.enabledExtensionCount   = vkDeviceExtensionNames.size();// the count of Device Extensions
	deviceInfo.ppEnabledExtensionNames = vkDeviceExtensionNames.data();// the pointer of Device Extensions
	deviceInfo.enabledLayerCount       = 0;                            // the count of layer
	deviceInfo.ppEnabledLayerNames     = NULL;                         // the pointer of layer
	deviceInfo.pEnabledFeatures        = NULL;                         // the pointer of enabled features

	LE_ASSERT(vkCreateDevice(vkVariableManager->gpus[0],
		                     &deviceInfo,
		                     NULL,
		                     &vkVariableManager->vkDevice) == VK_SUCCESS);

}

void LEVkDevice::destroyGlobalInstance() {
	vkDestroyDevice(vkVariableManager->vkDevice, nullptr);
}
