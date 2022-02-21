#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_device_capabilities.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// Vulkan Includes
#include <vulkan/vulkan.hpp>

class LEDeviceCapabilities {
public:
	static bool isDepthStencilBufferLinearTilingSupported(bool hasStencil = false);

	static bool isDepthStencilBufferOptimalTilingSupported(bool hasStencil = false);

	static VkSampleCountFlagBits getMaxUsableSampleCount();
};
