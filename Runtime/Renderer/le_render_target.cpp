/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_render_target.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_vk_image.h"
#include "le_vk_render_pass.h"
#include "le_render_target.h"

#include "le_device_capabilities.h"
#include "le_vk_device.h"

LERenderTarget* LERenderTarget::ms_activeRT = nullptr;

LERenderTarget::LERenderTarget(unsigned int width, 
	                           unsigned int height,
	                           VkFormat format):
	m_width(width), m_height(height), m_colorFormat(format){}
LERenderTarget::~LERenderTarget() {}

bool LERenderTarget::hasStencilComponent() const {
	return false;
}

VkFormat LERenderTarget::getColorFormat() const {
	return this->m_colorFormat;
}

unsigned int LERenderTarget::getWidth() const { 
	return this->m_width; 
}

unsigned int LERenderTarget::getHeight() const { 
	return this->m_height; 
}

void LERenderTarget::createDepthStencilBuffer() {
	VkFormatProperties formatProperties = LEVkDevice::getFormatProperties(LEVkDevice::getDepthStencilFormat());
	VkImageTiling tiling;
	if(LEDeviceCapabilities::isDepthStencilBufferLinearTilingSupported(true)) {
		tiling = VK_IMAGE_TILING_LINEAR;
	}
	else if	(LEDeviceCapabilities::isDepthStencilBufferOptimalTilingSupported(true)) {
		tiling = VK_IMAGE_TILING_OPTIMAL;
	}
	this->m_depthStencilImage  = LEVkDevice::createImage(this->m_width,this->m_height,
		                                          this->getDepthStencilFormat(), 
		                                          VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | 
		                                          VK_IMAGE_USAGE_TRANSFER_SRC_BIT | 
		                                          VK_IMAGE_USAGE_TRANSFER_DST_BIT,
		                                          tiling);
	this->m_depthStencilImageMemory = LEVkDevice::allocateImageMemory(this->m_depthStencilImage);
	this->m_depthStencilImageView = LEVkDevice::createImageView(this->m_depthStencilImage,
		                                                        this->getDepthStencilFormat(),
		                                                        VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, 
		                                                        VK_IMAGE_VIEW_TYPE_2D);
}

void LERenderTarget::createFence() {
	this->m_fence = LEVkDevice::createFence();
}

void LERenderTarget::setResized(bool resized) {
	this->m_isResized = resized;
}

bool LERenderTarget::isResized() const {
	return this->m_isResized;
}
