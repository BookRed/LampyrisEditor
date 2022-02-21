/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Resource
 * \File:    le_texture.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_texture.h"
#include "../Utility/le_png_builder.h"
#include "../Renderer/le_vk_command_buffer.h"
#include "../Renderer/le_vk_buffer.h"
#include "../Renderer/le_vk_device.h"
#include "../Renderer/le_vk_sampler.h"

// STB Includes
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>


int LEImage::getMinimumUpperPow2Number(int value) {
	if (std::_Is_pow_2(value)) {
		return value;
	}
	for (int i = 0; i < sizeof(ms_pow2Numbers) / sizeof(int); i++) {
		if (value <= ms_pow2Numbers[i]) {
			return ms_pow2Numbers[i];
		}
	}
}

LEImage::LEImage(const std::string& path) :
	m_width(-1),
	m_height(-1),
	m_channelCount(0),
	m_path(path),
	m_pixelData(nullptr) {

	this->m_pixelData = stbi_load(path.c_str(),
		                          &this->m_width,
		                          &this->m_height,
		                          &m_channelCount,
		                          STBI_rgb_alpha);
	if (!this->m_pixelData) {
		return;
	}
	// ensure the width and height are pow of 2
	this->m_height_pow2 = this->getMinimumUpperPow2Number(this->m_height);
	this->m_width_pow2  = this->getMinimumUpperPow2Number(this->m_width);
	if (this->m_height != this->m_height_pow2 || this->m_width != this->m_width_pow2) {
		auto pPixelData = new unsigned char[4 * this->m_height_pow2 * this->m_width_pow2];
		::memset(pPixelData, 0, 4 * this->m_height_pow2 * this->m_width_pow2);
		for (int i = 0; i < this->m_height; i++) {
			::memcpy(pPixelData + this->m_width_pow2*i*4, this->m_pixelData + this->m_width*i*4, 4 * sizeof(unsigned char) * this->m_width);
		}
		LE_SAFE_RELEASE_ARRAY(this->m_pixelData);
		this->m_pixelData = pPixelData;
	}
}

LEImage::~LEImage() {
	LE_SAFE_RELEASE_ARRAY(this->m_pixelData);
}

int LEImage::getByteCount() const {
	return this->m_width_pow2 * this->m_height_pow2 * 4;
}

unsigned char* LEImage::readPixelData() const {
	return this->m_pixelData;
}

bool LEImage::isEmpty() const {
	return this->m_pixelData == nullptr;
}

int LEImage::getWidth() const {
	return this->m_width_pow2;
}

int LEImage::getHeight() const {
	return this->m_height_pow2;
}

int LEImage::getRealWidth() const {
	return this->m_width;
}

int LEImage::getRealHeight() const {
	return this->m_height;
}

bool LEImage::save(const std::string& path) {
	return LEPngBuilder::createFromRGBA(path, this->m_pixelData, this->m_width_pow2, this->m_height_pow2);
}

LETexture::LETexture():LEResource() {}

LETexture::~LETexture() {}

VkDescriptorImageInfo& LETexture::getDescriptorImageInfo() {
	return this->m_imageInfo;
}

LETexture::ResourceType LETexture::getType() const {
	return ResourceType::RT_TEXTURE;
}

void LETexture::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspect,int levelCount, int faceCount) {
	auto pCommandBuffer = LEVkCommandBuffer::getTempBuffer();

	VkImageMemoryBarrier barrier{};
	barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout                       = oldLayout;
	barrier.newLayout                       = newLayout;
	barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
	barrier.image                           = image;
	barrier.subresourceRange.aspectMask     = aspect;
	barrier.subresourceRange.baseMipLevel   = 0;
	barrier.subresourceRange.levelCount     = levelCount;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount     = faceCount;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;
	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED) {
		barrier.srcAccessMask = 0;
		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	}
	else if(oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
		barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		sourceStage           = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}

	if (newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
		barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	}
	else if (newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else if (newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
		barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	pCommandBuffer->startRecord();
	pCommandBuffer->setupImageMemoryBarrier(sourceStage, destinationStage,0,1,&barrier);
	pCommandBuffer->submit();
}

uint32_t LETexture::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(vkVariableManager->gpus[0], &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void LETexture::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height,int levelCount,int faceCount) {
	auto pCommandBuffer = LEVkCommandBuffer::createBuffer();
	std::vector<VkBufferImageCopy> bufferCopyRegions;
	int totalOffset = 0;
	for (int level = 0; level < levelCount; level++) {
		for (int face = 0; face <faceCount; face++) {
			VkBufferImageCopy region{};
			region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.mipLevel       = level;
			region.imageSubresource.baseArrayLayer = face;
			region.imageSubresource.layerCount     = 1;
			region.imageExtent.width               = width >> level;
			region.imageExtent.height              = height >> level;
			region.imageExtent.depth               = 1;
			region.bufferOffset                    = totalOffset;
			bufferCopyRegions.push_back(region);
			totalOffset += 4 * region.imageExtent.width * region.imageExtent.height;
		}
	}
	pCommandBuffer->startRecord();
	pCommandBuffer->copyBufferToImage(buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, bufferCopyRegions);
	pCommandBuffer->submit();
	LE_SAFE_RELEASE(pCommandBuffer);
}

void LETexture::onLoaded() {
	VkImageView    imageView;
	VkDeviceMemory textureImageMemory;
	VkDeviceSize   imageSize = 4 * this->m_width * this->m_height;

	LEVkBuffer     stagingBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
	stagingBuffer.map();
	stagingBuffer.writeByte(this->m_pData, imageSize, 0);
	stagingBuffer.unmap();

	this->m_image = LEVkDevice::createImage(this->m_width,
	                                        this->m_height,
	                                        VK_FORMAT_R8G8B8A8_SRGB, 
	                                        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
	this->m_imageMemory = LEVkDevice::allocateImageMemory(this->m_image);
	transitionImageLayout(this->m_image, 
	                      VK_FORMAT_R8G8B8A8_SRGB, 
	                      VK_IMAGE_LAYOUT_UNDEFINED, 
	                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	copyBufferToImage(stagingBuffer.getHandle(), this->m_image, this->m_width, this->m_height);
	transitionImageLayout(this->m_image,
	                      VK_FORMAT_R8G8B8A8_SRGB, 
	                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
	                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	this->m_imageView = LEVkDevice::createImageView(this->m_image, 
	                                                VK_FORMAT_R8G8B8A8_SRGB, 
	                                                VK_IMAGE_ASPECT_COLOR_BIT, 
	                                                VK_IMAGE_VIEW_TYPE_2D);

	VkSamplerCreateInfo samplerInfo     = {};
	samplerInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter               = VK_FILTER_NEAREST;
	samplerInfo.minFilter               = VK_FILTER_LINEAR;
	samplerInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.anisotropyEnable        = VK_FALSE;
	samplerInfo.maxAnisotropy           = 0;
	samplerInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable           = VK_FALSE;
	samplerInfo.compareOp               = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;

	this->m_pSampler = LEVkSampler::getSampler(samplerInfo);
	
	VkDescriptorImageInfo vkImageInfo = {};
	vkImageInfo.imageView             = this->m_imageView;
	vkImageInfo.sampler               = this->m_pSampler->getHandle();            
	vkImageInfo.imageLayout           = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	this->m_imageInfo                 = vkImageInfo;
}

void LETexture::onUnloaded() {
	LE_SAFE_RELEASE(this->m_pData);
	LEVkDevice::destroyImage(this->m_image);
	LEVkDevice::destroyImageView(this->m_imageView);
	LEVkDevice::releaseMemory(this->m_imageMemory);
}


// Texture2D
LETexture2D::LETexture2D(uint32_t width, uint32_t height, unsigned char* pData) {
	this->m_width = width;
	this->m_height = height;
	this->m_pData = pData;
}

LETexture2D::~LETexture2D() {

}

LETexture2D* LETexture2D::createFromFile(const std::string& path) {
	LEImage image(path);
	if (image.isEmpty()) {
		return nullptr;
	}
	LETexture2D* pTexture = new LETexture2D(image.getWidth(), image.getHeight(), image.readPixelData());
	pTexture->onLoaded();
	return pTexture;

}

LECubemap::LECubemap(uint32_t faceWidth, std::array<std::string, 6> faceFiles):
	m_faceWidth(faceWidth) {
	this->m_xPositiveImage = new LEImage(faceFiles[0]);
	this->m_xNegativeImage = new LEImage(faceFiles[1]);
	this->m_yPositiveImage = new LEImage(faceFiles[2]);
	this->m_yNegativeImage = new LEImage(faceFiles[3]);
	this->m_zPositiveImage = new LEImage(faceFiles[4]);
	this->m_zNegativeImage = new LEImage(faceFiles[5]);
}

LECubemap::~LECubemap()
{
}

void LECubemap::onLoaded() {
	VkDeviceSize   faceSize = 4 * this->m_faceWidth * this->m_faceWidth;
	VkDeviceSize   imageSize = 6 * faceSize;
	this->m_pData = new unsigned char[imageSize];
	::memcpy(this->m_pData, this->m_xPositiveImage->readPixelData(), faceSize);
	::memcpy(this->m_pData + faceSize, this->m_xNegativeImage->readPixelData(), faceSize);
	::memcpy(this->m_pData + 2* faceSize, this->m_yPositiveImage->readPixelData(), faceSize);
	::memcpy(this->m_pData + 3* faceSize, this->m_yNegativeImage->readPixelData(), faceSize);
	::memcpy(this->m_pData + 4* faceSize, this->m_zPositiveImage->readPixelData(), faceSize);
	::memcpy(this->m_pData + 5* faceSize, this->m_zNegativeImage->readPixelData(), faceSize);
	

	LEVkBuffer     stagingBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
	stagingBuffer.map();
	stagingBuffer.writeByte(this->m_pData, imageSize, 0);
	stagingBuffer.unmap();

	this->m_image = LEVkDevice::createImage(this->m_width,
	                                        this->m_height,
	                                        VK_FORMAT_R8G8B8A8_SRGB, 
	                                        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,VK_IMAGE_TILING_OPTIMAL,
		                                    1,
		                                    VK_IMAGE_TYPE_2D,
		                                    1,
		                                    6,
		                                    VK_SAMPLE_COUNT_1_BIT, 
		                                    VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT);
	this->m_imageMemory = LEVkDevice::allocateImageMemory(this->m_image);

	transitionImageLayout(this->m_image, 
	                      VK_FORMAT_R8G8B8A8_SRGB, 
	                      VK_IMAGE_LAYOUT_UNDEFINED, 
	                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,1,6);
	copyBufferToImage(stagingBuffer.getHandle(), this->m_image, this->m_width, this->m_height);
	transitionImageLayout(this->m_image,
	                      VK_FORMAT_R8G8B8A8_SRGB, 
	                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
	                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,1,6);
	this->m_imageView = LEVkDevice::createImageView(this->m_image, 
	                                                       VK_FORMAT_R8G8B8A8_SRGB, 
	                                                       VK_IMAGE_ASPECT_COLOR_BIT, 
	                                                       VK_IMAGE_VIEW_TYPE_2D,0,1,0,6);

	VkSamplerCreateInfo samplerInfo     = {};
	samplerInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter               = VK_FILTER_NEAREST;
	samplerInfo.minFilter               = VK_FILTER_LINEAR;
	samplerInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.anisotropyEnable        = VK_FALSE;
	samplerInfo.maxAnisotropy           = 0;
	samplerInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable           = VK_FALSE;
	samplerInfo.compareOp               = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;

	if (vkCreateSampler(vkVariableManager->vkDevice, &samplerInfo, nullptr, &this->m_pSampler->getHandle()) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}
	;
	VkDescriptorImageInfo vkImageInfo = {};
	vkImageInfo.imageView             = this->m_imageView;
	vkImageInfo.sampler               = this->m_pSampler->getHandle();            
	vkImageInfo.imageLayout           = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	this->m_imageInfo                 = vkImageInfo;
}
