#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_render_target_texture.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_RENDER_TARGET_TEXTURE_H
#define LE_RENDER_TARGET_TEXTURE_H

// LE Includes
#include "../Base/le_global.h"
#include "le_render_target.h"
#include "Runtime/Base/le_hash.h"

class LERenderTargetTexture:public LERenderTarget {
private:
	bool                  m_isUsed{ false };
	VkImage               m_vkImage;
	VkImageView           m_vkImageView;
	VkDeviceMemory        m_vkImageMemory;
	unsigned int          m_imageByteCount;

	void                  createImage();

	void                  refreshFrameBuffer();
public:
	                      LERenderTargetTexture(uint32_t width, uint32_t height, VkFormat format , VkSampleCountFlagBits multiSample = VK_SAMPLE_COUNT_1_BIT);

	virtual              ~LERenderTargetTexture();

	void                  present();

	uint32_t              findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	void                  copyPixelDataToBuffer(LEVkBuffer* pVkBuffer);

	uint32_t              getImageByteCount() const;

	void                  clearColor();

	static std::unordered_map<uint32_t,std::vector<LERenderTargetTexture*>> ms_renderTexturePool;

	// obtain a render target texture (RTT) from pool
	static LERenderTargetTexture* getTemporary(uint32_t width,uint32_t height,VkFormat format,VkSampleCountFlagBits multiSample);

	static void           restoreTemporary(LERenderTargetTexture* pRTT);
				          
	static void           clearUpTemporary(LERenderTargetTexture* pRTT);
				          
	VkImageView           getRenderImageView() const override { return this->m_vkImageView; }
				          
	VkImage               getRenderImage() const override { return this->m_vkImage; }

	uint32_t getRenderTargetType() const override {
		return LE_RENDER_TARGET_WINDOW;
	}
};
#endif // !LE_RENDER_TARGET_TEXTURE_H