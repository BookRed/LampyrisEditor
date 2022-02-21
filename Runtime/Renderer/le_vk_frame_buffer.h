#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_vk_frame_buffer.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_VK_FRAME_BUFFER_H
#define LE_VK_FRAME_BUFFER_H

// LE Includes
#include "le_vk_object.h"
#include "le_device_capabilities.h"

class LEVkRenderPass;

class LEVkFrameBuffer:public LEVulkanObject<VkFramebuffer> {
private:
	std::vector<LERenderTarget*> m_pRenderTargets;

	LEVkFrameBuffer() = default;
public:
	static LEVkFrameBuffer* createForRenderPass(LEVkRenderPass* pRenderPass, const std::vector<VkFormat>& formats);

	virtual ~LEVkFrameBuffer();
};
#endif // !LE_VK_FRAME_BUFFER_H