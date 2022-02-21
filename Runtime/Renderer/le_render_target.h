#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_render_target.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_RENDER_TARGET_H
#define LE_RENDER_TARGET_H

// LE Includes
#include "le_camera.h"
#include "le_vk_variable_manager.h"
#include "utility/le_memory_type_from_properties.h"

class LEVkBuffer;
class LEVkRenderPass;

#define LE_RENDER_TARGET_TEXTURE 1u
#define LE_RENDER_TARGET_WINDOW  2u
class LERenderTarget {
protected:
	static LERenderTarget* ms_activeRT;

	unsigned int           m_width;
	unsigned int           m_height;
	VkFormat               m_colorFormat;

	VkImage                m_depthStencilImage{ VK_NULL_HANDLE };
	VkImageView            m_depthStencilImageView{ VK_NULL_HANDLE };
	VkDeviceMemory         m_depthStencilImageMemory{ VK_NULL_HANDLE };

	/*command buffer*/	   
	VkFence                m_fence;

	// as information that the pipeline needs to be recreated 
	bool                   m_isResized;

	uint32_t               m_type;
public:
	static LERenderTarget* activeRenderTarget() {
		return ms_activeRT;
	}

	static void            setActiveRenderTarget(LERenderTarget* pRenderTarget) {
		ms_activeRT = pRenderTarget;
	}

	                      LERenderTarget(unsigned int width, 
							             unsigned int height,
		                                 VkFormat format);

	virtual              ~LERenderTarget();

	bool                  hasStencilComponent() const;
		                  
	void                  createDepthStencilBuffer();
			              
	VkFormat              getColorFormat() const;
			              
	VkFormat              getDepthStencilFormat() const {
		return VK_FORMAT_D24_UNORM_S8_UINT;
	}

	uint32_t              getWidth() const;
				          
	uint32_t              getHeight() const;

	virtual void          present() = 0;

	void                  createFence();

	void                  setResized(bool resized);

	bool                  isResized() const;

	virtual VkImage       getRenderImage() const = 0;

	virtual VkImageView   getRenderImageView() const = 0;

	VkImage               getDepthStencilImage() const {
		return this->m_depthStencilImage;
	}
	VkImageView           getDepthStencilImageView() const {
		return this->m_depthStencilImageView;
	}

	virtual uint32_t      getRenderTargetType() const = 0;
};
#endif // !LE_RENDER_TARGET_H