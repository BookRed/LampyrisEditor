#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_vk_loaded.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_VK_LOADED_API_H
#define LE_VK_LOADED_API_H
// vulkan Includes
#include <vulkan/vulkan.h>

#define LE_DECLARED_LOADED_API(name) \
	static PFN_##name ms_##name;

#define LE_IMPL_LOADED_API(name) \
	PFN_##name LELoadedAPI::ms_##name = VK_NULL_HANDLE;

#define LE_LOAD_API(name) \
	ms_##name = (PFN_##name)(vkGetDeviceProcAddr(vkVariableManager->vkDevice, #name));

class LELoadedAPI {
public:
	LE_DECLARED_LOADED_API(vkCmdPushDescriptorSetKHR);
	
	static void loadAPIFromDLL();
};
#endif // !LE_VK_LOADED_API_H