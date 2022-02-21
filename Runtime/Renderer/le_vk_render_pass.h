#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_render_pass.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_RENDER_PASS_H
#define LE_RENDER_PASS_H

// LE Includes
#include "le_device_capabilities.h"
#include "le_vk_object.h"
#include "le_render_target.h"
#include "le_vk_device.h"
#include "../Resource/le_shader.h"

class LEShaderIOInfo;
class LEVkFrameBuffer;

// 'LEVkRenderPass' is combination of 'VkRenderPass' and 'VkFrameBuffer', as a member of 'LEVkGraphicsPipeline'.
class LEVkRenderPass:public LEVulkanObject<VkRenderPass> {
protected:
	LEVkFrameBuffer*               m_pFrameBuffer {nullptr};
	LERenderTarget*                m_pRenderTarget {nullptr};
	VkRenderPassBeginInfo          m_renderPassBeginInfo;
	VkFramebufferCreateInfo        m_frameBufferCreateInfo;
	std::vector<VkFormat>          m_attachmentFormats;
public:						    
	                               LEVkRenderPass();
				                  
	virtual                       ~LEVkRenderPass();
				                  
	void                           setRenderTarget(LERenderTarget* pRenderTarget);

	void                           createDepthPass();

	void                           createForwardPass(const std::vector<LEShaderIOInfo>& output, VkSampleCountFlagBits multiSample);

	void                           createDeferredPass(const std::vector<LEShaderIOInfo>& output);
				                  
	void                           begin();
				                  
	void                           end();

	virtual VkRenderPassBeginInfo& getRenderPassBeginInfo();

	void                           prepareFrameBuffer();
};
#endif // !LE_RENDER_PASS_H