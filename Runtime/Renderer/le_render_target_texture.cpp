#pragma once
/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_render_target_texture.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_vk_render_pass.h"
#include "le_render_target_texture.h"
#include "le_vk_buffer.h"
#include "le_vk_command_buffer.h"
#include "../Interact/le_input_system.h"
// GLM Includes
#include <glm/glm.hpp>

std::unordered_map<uint32_t, std::vector<LERenderTargetTexture*>> LERenderTargetTexture::ms_renderTexturePool;

void LERenderTargetTexture::createImage() {
	if(this->m_colorFormat == VkFormat::VK_FORMAT_UNDEFINED) {
		return;
	}
	this->m_vkImage = LEVkDevice::createImage(this->m_width, this->m_height,
		                                      this->m_colorFormat,
		                                      VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
	this->m_vkImageMemory = LEVkDevice::allocateImageMemory(this->m_vkImage);
	this->m_vkImageView = LEVkDevice::createImageView(this->m_vkImage,
		                                              this->m_colorFormat,
		                                              VK_IMAGE_ASPECT_COLOR_BIT,
		                                              VK_IMAGE_VIEW_TYPE_2D);
}

void LERenderTargetTexture::refreshFrameBuffer() {
	return;
}

LERenderTargetTexture::LERenderTargetTexture(uint32_t width, 
                                             uint32_t height, 
	                                         VkFormat format, VkSampleCountFlagBits multiSample):
	LERenderTarget(width, height, format) {
	this->createImage();
	this->createDepthStencilBuffer();
	this->createFence();
}

LERenderTargetTexture::~LERenderTargetTexture() {

}
void LERenderTargetTexture::present() {
	return;
}

uint32_t LERenderTargetTexture::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(vkVariableManager->gpus[0], &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void LERenderTargetTexture::clearColor() {
	LECamera* pRenderingCamera = LECamera::getRenderingCamera();
	glm::vec4 color = pRenderingCamera->getClearColor();
	VkClearColorValue clearValue;
	clearValue.float32[0] = color.r;
	clearValue.float32[1] = color.g;
	clearValue.float32[2] = color.b;
	clearValue.float32[3] = color.a;
	auto CMDBuffer = pRenderingCamera->getCommandBuffer();
	CMDBuffer->clearColorImage(this->m_vkImage, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, clearValue);
}

void LERenderTargetTexture::restoreTemporary(LERenderTargetTexture* pRTT) {
	if(pRTT && pRTT->m_isUsed) {
		pRTT->m_isUsed = false;
	}
}

void LERenderTargetTexture::clearUpTemporary(LERenderTargetTexture* pRTT) {
	for(auto &[key,pRTTVector]: ms_renderTexturePool) {
		for(auto &pRTT: pRTTVector) {
			LE_SAFE_RELEASE(pRTT);
		}
	}
	ms_renderTexturePool.clear();
}

void LERenderTargetTexture::copyPixelDataToBuffer(LEVkBuffer* pVkBuffer) {
	if (!pVkBuffer) {
		return;
	}
	auto pCommandBuffer = LEVkCommandBuffer::getTempBuffer();
	pCommandBuffer->reset();
	pCommandBuffer->startRecord();
	VkBufferImageCopy region{ 0 };
	region.imageExtent.width               = this->m_width;
	region.imageExtent.height              = this->m_height;
	region.imageExtent.depth               = 1;
	region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount     = 1;

	pCommandBuffer->copyImageToBuffer(this->m_vkImage, pVkBuffer->getHandle(), { region });
	pCommandBuffer->submit();
}

uint32_t LERenderTargetTexture::getImageByteCount() const {
	return this->m_imageByteCount;
}

LERenderTargetTexture* LERenderTargetTexture::getTemporary(uint32_t width, uint32_t height, VkFormat format,
	VkSampleCountFlagBits multiSample) {
	const uint32_t hashValue = le_hash_value(width, height, format, multiSample);
	if(ms_renderTexturePool.contains(hashValue)) {
		auto& pRTTVector = ms_renderTexturePool[hashValue];
		// find unused RTT
		for(const auto & pRTT: pRTTVector) {
			if(!pRTT->m_isUsed) {
				pRTT->m_isUsed = true;
				return pRTT;
			}
		}
	} 
	// create a new rtt
	LERenderTargetTexture* pRTT = new LERenderTargetTexture(width, height, format);
	ms_renderTexturePool[hashValue].push_back(pRTT);
	return pRTT;
}
