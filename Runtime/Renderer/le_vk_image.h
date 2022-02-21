#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_vk_image.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_VK_IMAGE_H
#define LE_VK_IMAGE_H

// LE Includes
#include "le_vk_variable_manager.h"

class LEVkImage {
private:
	VkImage               m_vkImage;
	VkImageView           m_vkImageView;
	VkDeviceMemory        m_vkImageMemory;
	VkImageAspectFlagBits m_apsectLayer;

	                      LEVkImage() {}
public:
	static uint32_t       findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(vkVariableManager->gpus[0], &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}
	virtual              ~LEVkImage() {
		vkDestroyImage(vkVariableManager->vkDevice,this->m_vkImage,nullptr);
		vkDestroyImageView(vkVariableManager->vkDevice,this->m_vkImageView,nullptr);
		vkFreeMemory(vkVariableManager->vkDevice,this->m_vkImageMemory,nullptr);
	}

	static LEVkImage*     create(int width,int height,
		                         int usage,VkFormat format, VkImageAspectFlagBits aspectFlag = VK_IMAGE_ASPECT_COLOR_BIT,
		                         VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL,
		                         int mipLevels = 1,int layerCount = 1,int flag = 0) {
		VkImage        vkImage;
		VkImageView    vkImageView;
		VkDeviceMemory vkImageMemory;
		VkImageCreateInfo imageInfo{};
		imageInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType     = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width  = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth  = 1;
		imageInfo.mipLevels     = mipLevels;
		imageInfo.arrayLayers   = layerCount;
		imageInfo.format        = format;
		imageInfo.tiling        = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage         = usage;
		imageInfo.samples       = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags         = flag;
		LE_ASSERT(vkCreateImage(vkVariableManager->vkDevice, &imageInfo, nullptr, &vkImage) == VK_SUCCESS);

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(vkVariableManager->vkDevice, vkImage, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize  = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		LE_ASSERT(vkAllocateMemory(vkVariableManager->vkDevice, &allocInfo, nullptr, &vkImageMemory) == VK_SUCCESS);
		vkBindImageMemory(vkVariableManager->vkDevice, vkImage, vkImageMemory, 0);

		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image                           = vkImage;
		viewInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format                          = VK_FORMAT_R8G8B8A8_SRGB;
		viewInfo.subresourceRange.aspectMask     = aspectFlag;
		viewInfo.subresourceRange.baseMipLevel   = 0;
		viewInfo.subresourceRange.levelCount     = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount     = 1;

		LE_ASSERT(vkCreateImageView(vkVariableManager->vkDevice, &viewInfo, nullptr, &vkImageView) == VK_SUCCESS);

		LEVkImage* pImage       = new LEVkImage;
		pImage->m_vkImage       = vkImage;
		pImage->m_vkImageView   = vkImageView;
		pImage->m_vkImageMemory = vkImageMemory;
		pImage->m_apsectLayer   = aspectFlag;
		return pImage;
	}

	VkImage               getImageHandle() const {
		return this->m_vkImage;
	}

	VkImageView           getImageViewHandle() const {
		return this->m_vkImageView;
	}
};
#endif // !LE_VK_IMAGE_H