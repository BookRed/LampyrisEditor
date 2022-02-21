#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Resource
 * \File:    le_texture.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// Vulkan Includes
#include <vulkan/vulkan_core.h>

template<class T>
class LEVulkanObject {
protected:
	T   m_handle;
public:
	LEVulkanObject() :m_handle(VK_NULL_HANDLE) {}

	T& getHandle() {
		return this->m_handle;
	}
};