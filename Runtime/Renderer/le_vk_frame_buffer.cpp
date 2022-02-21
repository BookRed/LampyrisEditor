/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_vk_frame_buffer.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_camera.h"
#include "le_vk_render_pass.h"
#include "le_vk_frame_buffer.h"
#include "le_render_target_texture.h"
#include "le_vk_device.h"

LEVkFrameBuffer* LEVkFrameBuffer::createForRenderPass(LEVkRenderPass* pRenderPass, const std::vector<VkFormat> &formats) {
	if(!LECamera::getRenderingCamera()) {
		return nullptr;
	}
	LEVkFrameBuffer* pFrameBuffer = new LEVkFrameBuffer;
	auto multiSample = LECamera::getRenderingCamera()->getMultiSampleLevel();
	multiSample = std::min(multiSample, LEDeviceCapabilities::getMaxUsableSampleCount());
	bool hasMultiSample = multiSample != VK_SAMPLE_COUNT_1_BIT;
	const uint32_t attachmentCount = formats.size() +
	                                 1 + // depth & stencil 
	                                 hasMultiSample ? 1 : 0;
	const uint32_t width = LECamera::getRenderingCamera()->getRenderTarget()->getWidth();
	const uint32_t height = LECamera::getRenderingCamera()->getRenderTarget()->getHeight();
	std::vector<VkImageView> attachments(attachmentCount);
	pFrameBuffer->m_pRenderTargets.resize(attachmentCount);

	int index = 0;
	// resolve
	if(hasMultiSample) {
		auto* pRTTResolve = new LERenderTargetTexture(width,height,formats[0], multiSample);
		attachments[index] = pRTTResolve->getRenderImageView();
		pFrameBuffer->m_pRenderTargets[index] = pRTTResolve;
		index++;
	}
	// depth & stencil 
	// attachments[index] = LECamera::getRenderingCamera()->getRenderTarget()->getDepthStencilImageView();
	index++;

	if(formats.size()!= 0) {
		// render target of camera
		attachments[index++] = LECamera::getRenderingCamera()->getRenderTarget()->getRenderImageView();
		// others render target (MRT)
		for (uint32_t i = 1; i < formats.size(); i++) {
			auto* pRTT = new LERenderTargetTexture(width, height, formats[i], multiSample);
			attachments[index] = pRTT->getRenderImageView();
			pFrameBuffer->m_pRenderTargets[index] = pRTT;
			index++;
		}
	}

	VkFramebufferCreateInfo frameBufferCreateInfo = {};
	frameBufferCreateInfo.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	frameBufferCreateInfo.pNext                   = nullptr;
	frameBufferCreateInfo.renderPass              = pRenderPass->getHandle();
	frameBufferCreateInfo.attachmentCount         = attachmentCount;
	frameBufferCreateInfo.pAttachments            = attachments.data();
	frameBufferCreateInfo.width                   = width;
	frameBufferCreateInfo.height                  = height;
	frameBufferCreateInfo.layers                  = 1;

	pFrameBuffer->m_handle = LEVkDevice::createFrameBuffer(&frameBufferCreateInfo);
	return pFrameBuffer;
}

LEVkFrameBuffer::~LEVkFrameBuffer() {
	for(int i = 0;i<this->m_pRenderTargets.size();i++) {
		LE_SAFE_RELEASE(this->m_pRenderTargets[i]);
	}
}
