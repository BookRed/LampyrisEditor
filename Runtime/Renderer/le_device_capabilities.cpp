/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_device_capabilities.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_vk_device.h"
#include "le_device_capabilities.h"

bool LEDeviceCapabilities::isDepthStencilBufferLinearTilingSupported(bool hasStencil) {
	VkFormat format = hasStencil ? VK_FORMAT_D16_UNORM : VK_FORMAT_D24_UNORM_S8_UINT;
	VkFormatProperties formatProperties = LEVkDevice::getFormatProperties(format);
	return formatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
}

bool LEDeviceCapabilities::isDepthStencilBufferOptimalTilingSupported(bool hasStencil) {
	VkFormat format = hasStencil ? VK_FORMAT_D16_UNORM : VK_FORMAT_D24_UNORM_S8_UINT;
	VkFormatProperties formatProperties = LEVkDevice::getFormatProperties(format);
	return formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
}

VkSampleCountFlagBits LEDeviceCapabilities::getMaxUsableSampleCount() {
	auto limits = LEVkDevice::getPhysicalDeviceLimits();
	VkSampleCountFlags counts = std::min(limits.framebufferColorSampleCounts, limits.framebufferDepthSampleCounts);
	if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
	if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
	if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
	if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
	if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
	if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

	return VK_SAMPLE_COUNT_1_BIT;
}
