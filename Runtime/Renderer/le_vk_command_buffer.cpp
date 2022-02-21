/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_vk_command_buffer.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_vk_command_buffer.h"
#include "le_vk_graphics_pipeline.h"
#include "../Resource/le_mesh.h"

VkQueue LEVkCommandBuffer::ms_graphicsQueue = VK_NULL_HANDLE;
LEVkCommandBuffer* LEVkCommandBuffer::ms_pTempBuffer = VK_NULL_HANDLE;

LEVkCommandBuffer::LEVkCommandBuffer() {
	if (!ms_graphicsQueue) {
		vkGetDeviceQueue(vkVariableManager->vkDevice, vkVariableManager->gpuQueueGraphicsFamilyIndex, 0, &ms_graphicsQueue);
	}
	if (!m_vkCommandPool) {
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = vkVariableManager->gpuQueueGraphicsFamilyIndex;

		if (vkCreateCommandPool(vkVariableManager->vkDevice, &poolInfo, nullptr, &m_vkCommandPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create graphics command pool!");
		}
	}
	this->m_fence = LEVkDevice::createFence();
}

LEVkCommandBuffer::~LEVkCommandBuffer() {
	vkFreeCommandBuffers(vkVariableManager->vkDevice, this->m_vkCommandPool, 1,&this->m_vkCommandBuffer);
	vkDestroyCommandPool(vkVariableManager->vkDevice, this->m_vkCommandPool, nullptr);
}

void LEVkCommandBuffer::startRecord() {
	// reset first !
	vkResetCommandBuffer(this->m_vkCommandBuffer, 0);
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags                    = 0;
	vkBeginCommandBuffer(this->m_vkCommandBuffer, &beginInfo);
}

void LEVkCommandBuffer::submit(uint32_t waitSemaphoreCount,const VkSemaphore* pWaitSemaphores){
	vkEndCommandBuffer(this->m_vkCommandBuffer);
	VkPipelineStageFlags flags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,VK_PIPELINE_STAGE_TRANSFER_BIT };
	VkSubmitInfo vkSubmitInfo;
	vkSubmitInfo.pNext                = nullptr;
	vkSubmitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	vkSubmitInfo.pWaitDstStageMask    = flags;
	vkSubmitInfo.commandBufferCount   = 1;
	vkSubmitInfo.pCommandBuffers      = &this->m_vkCommandBuffer;
	vkSubmitInfo.signalSemaphoreCount = 0;
	vkSubmitInfo.pSignalSemaphores    = &this->m_semaphore;
	vkSubmitInfo.waitSemaphoreCount   = waitSemaphoreCount;
	vkSubmitInfo.pWaitSemaphores      = pWaitSemaphores;
	LEVkDevice::resetFence(this->m_fence);
	vkQueueSubmit(ms_graphicsQueue, 1, &vkSubmitInfo, this->m_fence);
	LEVkDevice::waitFence(this->m_fence);
}

void LEVkCommandBuffer::reset() {
	vkResetCommandBuffer(this->m_vkCommandBuffer, 0);
}

LEVkCommandBuffer* LEVkCommandBuffer::getTempBuffer() {
	if(!ms_pTempBuffer) {
		ms_pTempBuffer = LEVkCommandBuffer::createBuffer();
	}
	return ms_pTempBuffer;
}

LEVkCommandBuffer* LEVkCommandBuffer::createBuffer() {
	LEVkCommandBuffer* cmdBuffer = new LEVkCommandBuffer;
	VkCommandPoolCreateInfo cmdPoolCreateInfo = {};
	cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO; 	   //  the type of struct
	cmdPoolCreateInfo.queueFamilyIndex = vkVariableManager->gpuQueueGraphicsFamilyIndex;   //  bind index of queueGraphicsFamily 
	cmdPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;  //  the behavior flag of CommandBuffer

																				// Create Command Pool
	VkResult result = ::vkCreateCommandPool(vkVariableManager->vkDevice,
		&cmdPoolCreateInfo,
		NULL,
		&cmdBuffer->m_vkCommandPool);
	if (result != VK_SUCCESS) {
		LE_SAFE_RELEASE(cmdBuffer);
		return nullptr;
	}

	// Create an instance of command buffer allocation information structure
	VkCommandBufferAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;   //  the type of struct
	allocateInfo.commandPool = cmdBuffer->m_vkCommandPool;                       //  bind the Command Pool
	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;                  //  the level of allocated Command Buffer 
	allocateInfo.commandBufferCount = 1;                                                //  count of Command Buffer

																						// Create Command Buffer
	result = ::vkAllocateCommandBuffers(vkVariableManager->vkDevice,
		&allocateInfo,
		&cmdBuffer->m_vkCommandBuffer);
	if (result != VK_SUCCESS) {
		LE_SAFE_RELEASE(cmdBuffer);
		return nullptr;
	}

	return cmdBuffer;
}

VkCommandBuffer LEVkCommandBuffer::getHandle() const {
	return this->m_vkCommandBuffer;
}

void LEVkCommandBuffer::beginRenderPass() const {
	static auto vkCmdBeginRenderingKHR = LEVkDevice::getProcessAddress<PFN_vkCmdBeginRenderingKHR>("vkCmdBeginRenderingKHR");
	LERenderTarget* pRenderTarget = LERenderTarget::activeRenderTarget();

	VkRenderingAttachmentInfoKHR  colorAttachmentInfo;
	colorAttachmentInfo.sType              = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
	colorAttachmentInfo.pNext              = nullptr;
	colorAttachmentInfo.imageView          = pRenderTarget->getRenderImageView();
	colorAttachmentInfo.imageLayout        = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	colorAttachmentInfo.loadOp             = VK_ATTACHMENT_LOAD_OP_LOAD;
	colorAttachmentInfo.storeOp            = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentInfo.resolveMode        = VK_RESOLVE_MODE_NONE;
	colorAttachmentInfo.resolveImageView   = VK_NULL_HANDLE;
	colorAttachmentInfo.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachmentInfo.clearValue         = {};

	VkRenderingAttachmentInfoKHR  depthAttachmentInfo;
	depthAttachmentInfo.sType              = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
	depthAttachmentInfo.pNext              = nullptr;
	depthAttachmentInfo.imageView          = pRenderTarget->getDepthStencilImageView();
	depthAttachmentInfo.imageLayout        = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	depthAttachmentInfo.loadOp             = VK_ATTACHMENT_LOAD_OP_LOAD;
	depthAttachmentInfo.storeOp            = VK_ATTACHMENT_STORE_OP_STORE;
	depthAttachmentInfo.resolveMode        = VK_RESOLVE_MODE_NONE;
	depthAttachmentInfo.resolveImageView   = VK_NULL_HANDLE;
	depthAttachmentInfo.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachmentInfo.clearValue         = {};

	VkRenderingAttachmentInfoKHR  stencilAttachmentInfo;
	stencilAttachmentInfo.sType              = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
	stencilAttachmentInfo.pNext              = nullptr;
	stencilAttachmentInfo.imageView          = pRenderTarget->getDepthStencilImageView();
	stencilAttachmentInfo.imageLayout        = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	stencilAttachmentInfo.loadOp             = VK_ATTACHMENT_LOAD_OP_LOAD;
	stencilAttachmentInfo.storeOp            = VK_ATTACHMENT_STORE_OP_STORE;
	stencilAttachmentInfo.resolveMode        = VK_RESOLVE_MODE_NONE;
	stencilAttachmentInfo.resolveImageView   = VK_NULL_HANDLE;
	stencilAttachmentInfo.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	stencilAttachmentInfo.clearValue         = {};

	VkRenderingInfoKHR renderInfo;
	renderInfo.sType                    = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
	renderInfo.pNext                    = nullptr;
	renderInfo.flags                    = 0;
	renderInfo.renderArea.offset.x      = 0;
	renderInfo.renderArea.offset.y      = 0;
	renderInfo.renderArea.extent.width  = pRenderTarget->getWidth();
	renderInfo.renderArea.extent.height = pRenderTarget->getHeight();
	renderInfo.layerCount               = 1;
	renderInfo.viewMask                 = 0;
	renderInfo.colorAttachmentCount     = 1;
	renderInfo.pColorAttachments        = &colorAttachmentInfo;
	renderInfo.pDepthAttachment         = &depthAttachmentInfo;
	renderInfo.pStencilAttachment       = &stencilAttachmentInfo;

	vkCmdBeginRenderingKHR(this->m_vkCommandBuffer, &renderInfo);
	// vkCmdBeginRenderPass(this->m_vkCommandBuffer, &pRenderPass->getRenderPassBeginInfo(), VK_SUBPASS_CONTENTS_INLINE);
}

void LEVkCommandBuffer::endRenderPass() const {
	static auto vkCmdEndRenderingKHR = LEVkDevice::getProcessAddress<PFN_vkCmdEndRenderingKHR>("vkCmdEndRenderingKHR");

	vkCmdEndRenderingKHR(this->m_vkCommandBuffer);
	// vkCmdEndRenderPass(this->m_vkCommandBuffer);
}

void LEVkCommandBuffer::draw(uint32_t vertexCount) const {
	vkCmdDraw(this->m_vkCommandBuffer, vertexCount, 1, 0, 0);
}

void LEVkCommandBuffer::drawIndexed(uint32_t indexCount) const {
	vkCmdDrawIndexed(this->m_vkCommandBuffer,indexCount, 1, 0, 0, 0);
}

void LEVkCommandBuffer::drawIndirect() const {
		
}

void LEVkCommandBuffer::drawInstancing() const {
		
}

void LEVkCommandBuffer::drawInstancingIndirect() const {

}

void LEVkCommandBuffer::drawIndexedIndirect(LEVkBuffer* pBuffer, VkDeviceSize offset, uint32_t drawCount) {
	vkCmdDrawIndexedIndirect(this->m_vkCommandBuffer, pBuffer->getHandle(), offset, drawCount, sizeof(VkDrawIndexedIndirectCommand));
}

void LEVkCommandBuffer::bindDescriptorSet(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout,
	uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount,
	const uint32_t* pDynamicOffsets) {
	vkCmdBindDescriptorSets(this->m_vkCommandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}

void LEVkCommandBuffer::setScissor(VkRect2D scissor) const {
	vkCmdSetScissor(this->m_vkCommandBuffer, 0, 1, &scissor);
}

void LEVkCommandBuffer::setViewPort(VkViewport viewport) const{
	vkCmdSetViewport(this->m_vkCommandBuffer, 0, 1, &viewport);
}

void LEVkCommandBuffer::setupMemoryBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
                                           VkDependencyFlags dependencyFlags, uint32_t barrierCount, const VkMemoryBarrier* pMemoryBarriers) const {
	vkCmdPipelineBarrier(this->m_vkCommandBuffer, srcStageMask, dstStageMask, dependencyFlags, 
	                     barrierCount, pMemoryBarriers, 0, nullptr, 0, nullptr);
}

void LEVkCommandBuffer::setupBufferMemoryBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
	VkDependencyFlags dependencyFlags, uint32_t barrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers) const {
	vkCmdPipelineBarrier(this->m_vkCommandBuffer, srcStageMask, dstStageMask, dependencyFlags, 
	                     0, nullptr, barrierCount, pBufferMemoryBarriers, 0, nullptr);
}

void LEVkCommandBuffer::setupImageMemoryBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
	VkDependencyFlags dependencyFlags, uint32_t barrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const {
	vkCmdPipelineBarrier(this->m_vkCommandBuffer, srcStageMask, dstStageMask, dependencyFlags, 
	                     0, nullptr, 0, nullptr, barrierCount, pImageMemoryBarriers);
}

void LEVkCommandBuffer::copyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout,
	std::vector<VkBufferImageCopy> regions) const {
	vkCmdCopyBufferToImage(this->m_vkCommandBuffer, srcBuffer, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, regions.size(), regions.data());
}

void LEVkCommandBuffer::copyImageToBuffer(VkImage vkImage,VkBuffer vkBuffer, std::vector<VkBufferImageCopy> regions) const {
	vkCmdCopyImageToBuffer(this->m_vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vkBuffer, regions.size(),regions.data());
}

void LEVkCommandBuffer::clearColorImage(VkImage image, VkImageLayout imageLayout, VkClearColorValue color) const {
	VkImageSubresourceRange range{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1,0,1 };
	vkCmdClearColorImage(this->m_vkCommandBuffer, image, imageLayout, &color, 1, &range);
}

void LEVkCommandBuffer::clearDepthStencilImage(VkImage image, VkImageLayout imageLayout,
	VkClearDepthStencilValue depthStencil) const {
	VkImageSubresourceRange range{ VK_IMAGE_ASPECT_DEPTH_BIT|VK_IMAGE_ASPECT_STENCIL_BIT, 0, 1,0,1 };
	vkCmdClearDepthStencilImage(this->m_vkCommandBuffer, image, imageLayout, &depthStencil, 1, &range);
}

void LEVkCommandBuffer::clearDepthImage(VkImage image, VkImageLayout imageLayout,
	VkClearDepthStencilValue depthStencil) const {
	VkImageSubresourceRange range{ VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1,0,1 };
	vkCmdClearDepthStencilImage(this->m_vkCommandBuffer, image, imageLayout, &depthStencil, 1, &range);
}

void LEVkCommandBuffer::clearStencilImage(VkImage image, VkImageLayout imageLayout,
	VkClearDepthStencilValue depthStencil) const {
	VkImageSubresourceRange range{VK_IMAGE_ASPECT_STENCIL_BIT, 0, 1,0,1 };
	vkCmdClearDepthStencilImage(this->m_vkCommandBuffer, image, imageLayout, &depthStencil, 1, &range);
}

void LEVkCommandBuffer::bindGraphicsPipeline(LEVkGraphicsPipeline* pPipeline) const {
	vkCmdBindPipeline(this->m_vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pPipeline->getHandle());
}

void LEVkCommandBuffer::bindVertexBuffer(LEVertexBuffer* pBuffer, uint32_t offset, uint32_t mask) const {
	std::array<VkBuffer, LEVertexBuffer::ms_vertexFormatCount> buffers{};
	std::array<VkDeviceSize, LEVertexBuffer::ms_vertexFormatCount> offsets{};
	uint32_t index = 0;
	for (uint32_t i = 0; i < LEVertexBuffer::ms_vertexFormatCount; i++) {
		buffers[i] = pBuffer->get()->getHandle();
		offsets[i] = pBuffer->getVertexCount() * pBuffer->getFormatOffset(static_cast<LEVertexBuffer::VertexFormat>(i));
	}
	vkCmdBindVertexBuffers(this->m_vkCommandBuffer, 0, LEVertexBuffer::ms_vertexFormatCount,
	                       buffers.data(),offsets.data());
}

void LEVkCommandBuffer::bindIndexBuffer(LEIndexBuffer* pBuffer, uint32_t offset) const {
	vkCmdBindIndexBuffer(this->m_vkCommandBuffer, pBuffer->get()->getHandle(), offset, VK_INDEX_TYPE_UINT32);
}

void LEVkCommandBuffer::drawMesh(LEMesh* pMesh) const {
	if (pMesh != nullptr) {
		const auto vbo = pMesh->getVertexBuffer();
		const auto ibo = pMesh->getIndexBuffer();
		const VkDeviceSize offset[] = { 0 };

		vkCmdBindVertexBuffers(this->m_vkCommandBuffer, 0, 1,
			&(vbo->get()->getHandle()), offset
		);
		if(ibo != nullptr) {
			vkCmdBindIndexBuffer(this->m_vkCommandBuffer, ibo->get()->getHandle(), 0, VK_INDEX_TYPE_UINT32);
			vkCmdDrawIndexed(this->m_vkCommandBuffer, ibo->getIndexCount(), 1, 0, 0, 0);
		}
		else {
			vkCmdDraw(this->m_vkCommandBuffer, vbo->getVertexCount(), 1, 0, 0);
		}
	}
}