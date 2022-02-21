/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_vk_debugger.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_vk_debugger.h"

VkDebugReportCallbackEXT LEVkDebugger::ms_vkDebugReportCallback; 
VkDebugUtilsMessengerEXT LEVkDebugger::ms_messenger;

bool                              LEVkDebugger::ms_active = false;
bool                              LEVkDebugger::ms_extensionPresent = false;
PFN_vkDebugMarkerSetObjectTagEXT  LEVkDebugger::ms_vkDebugMarkerSetObjectTag = VK_NULL_HANDLE;
PFN_vkDebugMarkerSetObjectNameEXT LEVkDebugger::ms_vkDebugMarkerSetObjectName = VK_NULL_HANDLE;
PFN_vkCmdDebugMarkerBeginEXT      LEVkDebugger::ms_vkCmdDebugMarkerBegin = VK_NULL_HANDLE;
PFN_vkCmdDebugMarkerEndEXT        LEVkDebugger::ms_vkCmdDebugMarkerEnd = VK_NULL_HANDLE;
PFN_vkCmdDebugMarkerInsertEXT     LEVkDebugger::ms_vkCmdDebugMarkerInsert = VK_NULL_HANDLE;

VKAPI_ATTR VkBool32 VKAPI_CALL LEVkDebugger::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity, 
	                                                       VkDebugUtilsMessageTypeFlagsEXT             messageType, 
	                                                       const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
	                                                       void*                                       pUserData) {
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}

void LEVkDebugger::installDebugCallBack() {
	VkDebugReportCallbackCreateInfoEXT dbgReportCreateInfo = {};
	dbgReportCreateInfo.sType       = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	dbgReportCreateInfo.pfnCallback = debugCallBackFunction;
	dbgReportCreateInfo.pUserData   = NULL;
	dbgReportCreateInfo.pNext       = NULL;
	dbgReportCreateInfo.flags       = VK_DEBUG_REPORT_WARNING_BIT_EXT |
	VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
	VK_DEBUG_REPORT_ERROR_BIT_EXT |
	VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
	VK_DEBUG_REPORT_DEBUG_BIT_EXT;
	auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(vkVariableManager->vkInstance, "vkCreateDebugReportCallbackEXT");
	LE_ASSERT(vkCreateDebugReportCallbackEXT(vkVariableManager->vkInstance,
	&dbgReportCreateInfo,
	NULL, &ms_vkDebugReportCallback) == VK_SUCCESS);
	
	LEVkDebugger::setupDebugMarker(vkVariableManager->vkDevice,vkVariableManager->gpus[0]);
	/*
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr; // ¿ÉÑ¡
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
		vkGetInstanceProcAddr(vkVariableManager->vkInstance, "vkCreateDebugUtilsMessengerEXT");

	LE_ASSERT(func != nullptr);
	LE_ASSERT(func(vkVariableManager->vkInstance, &createInfo, nullptr, &ms_messenger) == VK_SUCCESS);
	*/
}
