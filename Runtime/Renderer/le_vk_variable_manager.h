#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Common
 * \File:    le_vk_variable_manager.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "../Base/le_platform.h"
#include "../Base/le_assert.h"
#include "../Base/le_config_manager.h"

#if defined(LE_PLATFORM_WINDOWS)       
#include <windows.h>
#endif // LE_PLATFORM_WINDOWS

// Vulkan Includes
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>
// STL Includes
#include <vector>

class LERenderTarget;
struct LEVulkanVariableStruct {
#if defined(LE_PLATFORM_WINDOWS)          
	HINSTANCE                             hInstance;
#endif
	VkInstance                            vkInstance = VK_NULL_HANDLE;

	// physical devices
	unsigned int                          gpuCount = 0;
	VkPhysicalDevice                      gpus[8];
	VkPhysicalDeviceMemoryProperties      gpuMemoryProperties[8];
	VkPhysicalDeviceProperties            gpuProperties[8];

	// logical devices
	std::vector<VkQueueFamilyProperties>  gpuQueueFamilyProperty;
	uint32_t                              gpuQueueFamilyCount{ 0 };
	uint32_t                              gpuQueueGraphicsFamilyIndex{ UINT32_MAX };
	uint32_t                              gpuQueuePresentFamilyIndex{ UINT32_MAX };
	VkDevice                              vkDevice = VK_NULL_HANDLE;
	LERenderTarget*                       pRenderTarget;
};

class LEVulkanVariable {
public:
	static LEVulkanVariableStruct* s_vkVariable;
};

#define vkVariableManager LEVulkanVariable::s_vkVariable