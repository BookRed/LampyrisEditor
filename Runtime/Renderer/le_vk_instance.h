#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_vk_instance.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_VK_INSTANCE_H
#define LE_VK_INSTANCE_H

// LE Includes
#include "../Base/le_assert.h"
#include "../Base/le_config_manager.h"
#include "le_vk_variable_manager.h"

class LEVkInstance {
public:
	            LEVkInstance()  = delete;
 
	           ~LEVkInstance() = delete;

	static void createGlobalInstance();

	static void destoryGlobalInstance();

	template<typename T>
	static T    getProcessAddress(const char* name) {
		void* pAddress = vkGetInstanceProcAddr(vkVariableManager->vkInstance, name);
		return static_cast<T>(pAddress);
	}

	static VkSurfaceKHR createSurface(HWND hWnd) {
		VkSurfaceKHR vkSurface;
#if defined(LE_PLATFORM_WINDOWS)
		VkWin32SurfaceCreateInfoKHR createInfo = {};
		createInfo.sType                       = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hwnd                        = hWnd;
		createInfo.hinstance                   = ::GetModuleHandle(0); // instance of Windows Application
		// Create KHR surface
		LE_ASSERT(::vkCreateWin32SurfaceKHR(vkVariableManager->vkInstance,&createInfo,nullptr,&vkSurface) == VK_SUCCESS);
#else
#error "unsupported operation system to create vkSurface"
#endif // !LE_PLATFORM_WINDOWS
		return vkSurface;
	}
};
#endif // !LE_VK_INSTANCE_H