#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_vk_command_buffer.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_VK_COMMAND_BUFFER_H
#define LE_VK_COMMAND_BUFFER_H

// LE Includes
#include "le_vk_buffer.h"
#include "le_vk_render_pass.h"
#include "le_vk_variable_manager.h"

class LEMesh;
class LEIndexBuffer;
class LEVertexBuffer;
class LEVkGraphicsPipeline;
class LEVkRenderPass;

class LEVkCommandBuffer {
private:
	static VkQueue            ms_graphicsQueue;
	static LEVkCommandBuffer* ms_pTempBuffer;

	VkCommandPool             m_vkCommandPool {VK_NULL_HANDLE};
	VkCommandBuffer           m_vkCommandBuffer{ VK_NULL_HANDLE };
	VkFence                   m_fence{ VK_NULL_HANDLE };
	VkSemaphore               m_semaphore{ VK_NULL_HANDLE };

	                          LEVkCommandBuffer();
public:					     
	virtual                  ~LEVkCommandBuffer();
						     
	void                      startRecord();
		                     
	void                      submit(uint32_t waitSemaphoreCount = 0, const VkSemaphore* pWaitSemaphores = nullptr);

	void                      reset();

	static LEVkCommandBuffer* createBuffer();

	static LEVkCommandBuffer* getTempBuffer();

	VkCommandBuffer           getHandle() const;

	void beginRenderPass() const;

	void endRenderPass() const;

	void bindGraphicsPipeline(LEVkGraphicsPipeline* pPipeline) const;

	void bindVertexBuffer(LEVertexBuffer* pBuffer, uint32_t offset, uint32_t mask) const;

	void bindIndexBuffer(LEIndexBuffer* pBuffer, uint32_t offset) const;

	void drawMesh(LEMesh* pMesh) const;

	void draw(uint32_t vertexCount) const;

	void drawIndexed(uint32_t indexCount) const;

	void drawIndirect() const;

	void drawInstancing() const;

	void drawInstancingIndirect() const;

	void drawIndexedIndirect(LEVkBuffer* pBuffer,VkDeviceSize offset,uint32_t drawCount);

	void setupMemoryBarrier(VkPipelineStageFlags srcStageMask, 
	                        VkPipelineStageFlags dstStageMask, 
	                        VkDependencyFlags dependencyFlags,
	                        uint32_t barrierCount,
	                        const VkMemoryBarrier* pMemoryBarriers) const ;

	void setupBufferMemoryBarrier(VkPipelineStageFlags srcStageMask, 
	                              VkPipelineStageFlags dstStageMask, 
	                              VkDependencyFlags dependencyFlags,
	                              uint32_t barrierCount,
	                              const VkBufferMemoryBarrier* pBufferMemoryBarriers) const;

	void setupImageMemoryBarrier(VkPipelineStageFlags srcStageMask, 
	                             VkPipelineStageFlags dstStageMask, 
	                             VkDependencyFlags dependencyFlags,
	                             uint32_t barrierCount,
	                             const VkImageMemoryBarrier* pImageMemoryBarriers) const;

	void copyBufferToImage(VkBuffer srcBuffer,
	                       VkImage  dstImage,
	                       VkImageLayout dstImageLayout,
	                       std::vector<VkBufferImageCopy> regions) const;

	void copyImageToBuffer(VkImage vkImage, 
		                   VkBuffer vkBuffer, 
		                   std::vector<VkBufferImageCopy> regions) const;

	void clearColorImage(VkImage image,VkImageLayout imageLayout, VkClearColorValue color) const;

	void clearDepthStencilImage(VkImage image, VkImageLayout imageLayout, VkClearDepthStencilValue depthStencil) const;

	void clearDepthImage(VkImage image, VkImageLayout imageLayout, VkClearDepthStencilValue depthStencil) const;

	void clearStencilImage(VkImage image, VkImageLayout imageLayout, VkClearDepthStencilValue depthStencil) const;


	void bindDescriptorSet(VkPipelineBindPoint   pipelineBindPoint,
		                  VkPipelineLayout       layout,
		                  uint32_t               firstSet,
		                  uint32_t               descriptorSetCount,
		                  const VkDescriptorSet* pDescriptorSets,
		                  uint32_t               dynamicOffsetCount = 0,
		                  const uint32_t*        pDynamicOffsets = nullptr);

	void setViewPort(VkViewport viewport) const ;

	void setScissor(VkRect2D scissor) const;

	void setupPipelineBarrier(VkImageMemoryBarrier barrier,
	                          VkPipelineStageFlags srcStageMask,
	                          VkPipelineStageFlags dstStageMask ) {
		vkCmdPipelineBarrier(
			this->m_vkCommandBuffer,
			srcStageMask,
			dstStageMask,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier);
	}

	void pushConstants(VkPipelineLayout   layout,
	                   VkShaderStageFlags stageFlags,
                       uint32_t           offset,
                       uint32_t           size,
                       const void*        pValues) const {
		::vkCmdPushConstants(this->m_vkCommandBuffer, layout, stageFlags, offset, size, pValues);
	}
};
#endif // !LE_VK_COMMAND_BUFFER_H