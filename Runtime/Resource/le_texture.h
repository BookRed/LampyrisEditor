#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Resource
 * \File:    le_texture.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// Vulkan Includes
#include <vulkan/vulkan.h>

// STL Includes
#include <string>
#include <utility>

// LE Includes
#include <stb_image/stb_image.h>

#include "../Renderer/le_vk_variable_manager.h"
#include "../Renderer/utility/le_memory_type_from_properties.h"
#include "le_resource.h"
#include "Runtime/Base/le_hash.h"
#include "Runtime/Renderer/le_vk_buffer.h"
#include "Runtime/Renderer/le_vk_device.h"
#include "Runtime/Renderer/le_vk_object.h"
#include "Runtime/Utility/le_png_builder.h"

// References
// [1]:https://vulkan-tutorial.com/code/23_texture_image.cpp

class LEImage {
private:
	static constexpr int  ms_pow2Numbers[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192};
	int                   m_width;
	int                   m_height;
	int                   m_width_pow2;
	int                   m_height_pow2;
	int                   m_channelCount;
	std::string           m_path;
	unsigned char*        m_pixelData;

	static int            getMinimumUpperPow2Number(int value);
public: 
	                      LEImage(const std::string& path = std::string());
	
	virtual              ~LEImage();

	int                   getByteCount() const;

	unsigned char*        readPixelData() const;

	bool                  isEmpty() const;

	int                   getWidth() const;
	                      
	int                   getHeight() const;
	                      
	int                   getRealWidth() const;
	                      
	int                   getRealHeight() const;
		                  
	bool                  save(const std::string& path);
};

class LEVkSampler;

class LETexture:public LEResource, public LEObject {
protected:
	LEVkSampler*           m_pSampler;
	VkImage                m_image;
	VkImageView            m_imageView;
	VkDeviceMemory         m_imageMemory;
	VkDescriptorImageInfo  m_imageInfo;
						   
	uint32_t               m_width;
	uint32_t               m_height;

	unsigned char*         m_pData;
public:					   
	                       LETexture();
						   
	virtual                ~LETexture();

	VkDescriptorImageInfo& getDescriptorImageInfo();

	ResourceType           getType() const;

	static void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT,int levelCount = 1, int faceCount = 1);

	static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	static void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height,int levelCount = 1,int faceCount = 1);

	virtual uint32_t getWidth() const { return this->m_width;}

	virtual uint32_t getHeight() const {return this->m_height;}

	void onLoaded() override;

	void onUnloaded() override;
};
class LETexture2D:public LETexture {
public:
	         LETexture2D(uint32_t width, uint32_t height, unsigned char* pData);

	virtual ~LETexture2D();

	static LETexture2D* createFromFile(const std::string& path);

	static LETexture2D* getDefaultResource(int i = 0) {
		static LETexture2D* pTexture[3] = { nullptr,nullptr,nullptr };
		if (!pTexture[0]) {
			pTexture[0] = LETexture2D::createFromFile("res\\default\\default_texture.png");
			pTexture[1] = LETexture2D::createFromFile("res\\default\\texture.png");
		}
		if (i < 3) {
			return pTexture[i];
		}
		else {
			return nullptr;
		}
	}

};

// here,I have learnt how to use field 'layerCount',which means the number of the faces of texture
// a cubemap has 6 faces, so the layerCount = 6
class LECubemap :public LETexture {
private:
	LEImage*       m_xPositiveImage{nullptr};
	LEImage*       m_yPositiveImage{nullptr};
	LEImage*       m_zPositiveImage{nullptr};
	LEImage*       m_xNegativeImage{nullptr};
	LEImage*       m_yNegativeImage{nullptr};
	LEImage*       m_zNegativeImage{nullptr};
			         
	uint32_t         m_faceWidth{0};
public:		         
	                 LECubemap(uint32_t faceWidth,std::array<std::string,6> faceFiles);
			         
	virtual         ~LECubemap();
			         
	void             onLoaded() override;

	virtual uint32_t getWidth() const { return this->m_faceWidth; }

	virtual uint32_t getHeight() const { return this->m_faceWidth; }
};

