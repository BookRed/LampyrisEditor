/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_render_pass.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_camera.h"
#include "le_vk_render_pass.h"
#include "le_device_capabilities.h"
#include "le_vk_frame_buffer.h"
#include "Runtime/Resource/le_shader.h"
#include "le_vk_command_buffer.h"

LEVkRenderPass::LEVkRenderPass():
	m_pRenderTarget(nullptr) {}

LEVkRenderPass::~LEVkRenderPass() {
	LEVkDevice::destroyRenderPass();
}

void LEVkRenderPass::setRenderTarget(LERenderTarget* pRenderTarget) {
	this->m_pRenderTarget = pRenderTarget;
}

void LEVkRenderPass::createDepthPass() {
	// depth & stencil
	VkAttachmentDescription attachment;
	VkAttachmentReference   reference;
	attachment.format         = LEVkDevice::getDepthStencilFormat();
	attachment.samples        = VK_SAMPLE_COUNT_1_BIT;
	attachment.loadOp         = VK_ATTACHMENT_LOAD_OP_LOAD;
	attachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
	attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_LOAD;
	attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
	attachment.finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	attachment.flags          = 0;
	reference.attachment      = 0;
	reference.layout          = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass    = {};
	subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.flags                   = 0;
	// input
	subpass.inputAttachmentCount    = 0;
	subpass.pInputAttachments       = nullptr;
	// output
	subpass.colorAttachmentCount    = 0;
	subpass.pColorAttachments       = 0;
	// resolve
	subpass.pResolveAttachments     = 0;
	// depth
	subpass.pDepthStencilAttachment = &reference;
	// preserve
	subpass.preserveAttachmentCount = 0;
	subpass.pPreserveAttachments    = nullptr;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.pNext                  = nullptr;
	renderPassInfo.attachmentCount        = 1;
	renderPassInfo.pAttachments           = &attachment;
	renderPassInfo.subpassCount           = 1;
	renderPassInfo.pSubpasses             = &subpass;
	renderPassInfo.dependencyCount        = 0;
	renderPassInfo.pDependencies          = nullptr;

	this->m_handle = LEVkDevice::createRenderPass(&renderPassInfo);
	this->m_frameBufferCreateInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	this->m_frameBufferCreateInfo.pNext           = nullptr;
	this->m_frameBufferCreateInfo.renderPass      = this->m_handle;
	this->m_frameBufferCreateInfo.attachmentCount = 1;
	this->m_frameBufferCreateInfo.layers          = 1;
}

void LEVkRenderPass::createForwardPass(const std::vector<LEShaderIOInfo>& output, VkSampleCountFlagBits multiSample) {
	multiSample                    = std::min(multiSample, LEDeviceCapabilities::getMaxUsableSampleCount());
	const bool hasMultiSample      = multiSample != VK_SAMPLE_COUNT_1_BIT;
	const uint32_t attachmentCount = (hasMultiSample ? 1 : 0) +  (output.size() +1);

	// these fields are preserved for specific render targets
	this->m_frameBufferCreateInfo.pAttachments    = nullptr; 
	this->m_frameBufferCreateInfo.width           = 0;
	this->m_frameBufferCreateInfo.height          = 0;

	std::vector<VkAttachmentDescription> attachments(attachmentCount);
	std::vector<VkAttachmentReference>   outputReferences(output.size());
	this->m_attachmentFormats.resize(output.size());
	VkAttachmentReference                depthStencilReference;
	VkAttachmentReference                resolveReference;

	uint32_t index = 0;
	// resolve attachment
	if(hasMultiSample) {
		attachments[index].format         = s_glslTypeFormatMap[output[0].type];
		attachments[index].samples        = multiSample;
		attachments[index].loadOp         = VK_ATTACHMENT_LOAD_OP_LOAD;
		attachments[index].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[index].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[index].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[index].initialLayout  = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachments[index].finalLayout    = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachments[index].flags          = 0;
		resolveReference.attachment       = 0; // the subscript of VkAttachmentDescription array
		resolveReference.layout           = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		index++;
	}								      

	// depth & stencil
	attachments[index].format                   = LEVkDevice::getDepthStencilFormat();
	attachments[index].samples                  = multiSample;
	attachments[index].loadOp                   = VK_ATTACHMENT_LOAD_OP_LOAD;
	attachments[index].storeOp                  = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[index].stencilLoadOp            = VK_ATTACHMENT_LOAD_OP_LOAD;
	attachments[index].stencilStoreOp           = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[index].initialLayout            = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	attachments[index].finalLayout              = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	attachments[index].flags                    = 0;
	depthStencilReference.attachment        = index;
	depthStencilReference.layout            = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	index++;

	// render targets
	for (int i = 0; i < output.size(); i++) {
		attachments[index+i].format         = s_glslTypeFormatMap[output[i].type];
		attachments[index+i].samples        = multiSample;
		attachments[index+i].loadOp         = VK_ATTACHMENT_LOAD_OP_LOAD;
		attachments[index+i].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[index+i].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_LOAD;
		attachments[index+i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[index+i].initialLayout  = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachments[index+i].finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		attachments[index+i].flags          = 0;
		outputReferences[i].attachment  = index + i;
		outputReferences[i].attachment  = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		this->m_attachmentFormats[i]    = attachments[index + i].format;
	}

	VkSubpassDescription subpass    = {};
	subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.flags                   = 0;
	// input
	subpass.inputAttachmentCount    = 0;
	subpass.pInputAttachments       = nullptr;
	// output
	subpass.colorAttachmentCount    = output.size();
	subpass.pColorAttachments       = outputReferences.data();
	// resolve
	subpass.pResolveAttachments     = hasMultiSample ? &resolveReference : nullptr;
	// depth
	subpass.pDepthStencilAttachment = &depthStencilReference;
	// preserve
	subpass.preserveAttachmentCount = 0;
	subpass.pPreserveAttachments    = nullptr;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.pNext                  = nullptr;
	renderPassInfo.attachmentCount        = attachments.size();
	renderPassInfo.pAttachments           = attachments.data();
	renderPassInfo.subpassCount           = 1;
	renderPassInfo.pSubpasses             = &subpass;
	renderPassInfo.dependencyCount        = 0;
	renderPassInfo.pDependencies          = nullptr;

	this->m_handle = LEVkDevice::createRenderPass(&renderPassInfo);
}

void LEVkRenderPass::createDeferredPass(const std::vector<LEShaderIOInfo>& output) {
}

void LEVkRenderPass::begin() {
	LECamera* pRenderingCamera = LECamera::getRenderingCamera();
	if(!pRenderingCamera) {
		// [Error]:
		return;
	}
	this->prepareFrameBuffer();
	// pRenderingCamera->getCommandBuffer()->beginRenderPass();
}

void LEVkRenderPass::end() {
	LECamera* pRenderingCamera = LECamera::getRenderingCamera();
	if (!pRenderingCamera) {
		// [Error]:
		return;
	}
	pRenderingCamera->getCommandBuffer()->endRenderPass();
}

VkRenderPassBeginInfo& LEVkRenderPass::getRenderPassBeginInfo() {
	// clear value will not be used here, and will be used in 'LEClearFrameBufferPass'
	this->m_renderPassBeginInfo.sType                    = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	this->m_renderPassBeginInfo.pNext                    = nullptr;
	this->m_renderPassBeginInfo.renderPass               = this->m_handle;
	this->m_renderPassBeginInfo.renderArea.offset.x      = 0;
	this->m_renderPassBeginInfo.renderArea.offset.y      = 0;
	this->m_renderPassBeginInfo.renderArea.extent.width  = LECamera::getRenderingCamera()->getRenderTarget()->getWidth();
	this->m_renderPassBeginInfo.renderArea.extent.height = LECamera::getRenderingCamera()->getRenderTarget()->getHeight();
	this->m_renderPassBeginInfo.clearValueCount          = 0;
	this->m_renderPassBeginInfo.pClearValues             = nullptr;

	this->prepareFrameBuffer();
	this->m_renderPassBeginInfo.framebuffer              = this->m_pFrameBuffer->getHandle();

	return this->m_renderPassBeginInfo;
}

void LEVkRenderPass::prepareFrameBuffer() {
	if(!this->m_pFrameBuffer) {
		this->m_pFrameBuffer = LEVkFrameBuffer::createForRenderPass(this, this->m_attachmentFormats);
	}
}
