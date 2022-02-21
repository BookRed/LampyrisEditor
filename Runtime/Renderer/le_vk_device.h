#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_vk_logical_device.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_VK_LOGICAL_DEVICE_H
#define LE_VK_LOGICAL_DEVICE_H

// LE Includes
#include "le_vk_variable_manager.h"
#include "utility/le_memory_type_from_properties.h"
// STL Includes
#include <array>

#include "le_vk_graphics_pipeline.h"

// lowest level wrapping for device-relative methods
class LEVkDevice {
public:
	            LEVkDevice()  = delete;

	           ~LEVkDevice() = delete;

	static void createGlobalInstance();

	static void destroyGlobalInstance();

	static void waitIdle() {
		vkQueueWaitIdle(LEVkDevice::getGraphicsQueue());
		// vkQueueWaitIdle(LEVkDevice::getPresentQueue());
		vkDeviceWaitIdle(vkVariableManager->vkDevice);
	}

	static VkFormatProperties getFormatProperties(VkFormat format) {
		VkFormatProperties properties;
		vkGetPhysicalDeviceFormatProperties(vkVariableManager->gpus[0],format, &properties);
		return properties;
	}

	static VkImage createImage(uint32_t width,
		                       uint32_t height, 
		                       VkFormat format,
		                       VkImageUsageFlags usage,
		                       VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL,
		                       int32_t depth = 1,
		                       VkImageType type = VK_IMAGE_TYPE_2D, 
		                       uint32_t mipLevels = 1, 
		                       uint32_t layers = 1, 
		                       VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT, 
		                       VkImageCreateFlags flag = 0) {
		VkImage vkImage                       = VK_NULL_HANDLE;
		VkImageCreateInfo imageCreateInfo     = {};
		imageCreateInfo.sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.pNext                 = nullptr;
		imageCreateInfo.imageType             = type;
		imageCreateInfo.format                = format;
		imageCreateInfo.extent.width          = width;
		imageCreateInfo.extent.height         = height;
		imageCreateInfo.extent.depth          = depth;
		imageCreateInfo.mipLevels             = mipLevels;
		imageCreateInfo.arrayLayers           = layers;
		imageCreateInfo.samples               = samples;
		imageCreateInfo.tiling                = tiling;
		imageCreateInfo.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.usage                 = usage;
		imageCreateInfo.queueFamilyIndexCount = 0;
		imageCreateInfo.pQueueFamilyIndices   = nullptr;
		imageCreateInfo.sharingMode           = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.flags                 = flag;
		LE_ASSERT(vkCreateImage(vkVariableManager->vkDevice,&imageCreateInfo, NULL, &vkImage) == VK_SUCCESS);
		return vkImage;
	}

	static VkImageView createImageView(VkImage vkImage,
		                               VkFormat format, 
		                               VkImageAspectFlags aspectMask, 
		                               VkImageViewType viewType ,
		                               uint32_t baseMipLevel = 0, 
		                               uint32_t levelCount = 1, 
		                               uint32_t baseArrayLayer = 0, 
		                               uint32_t layerCount = 1) {
		VkImageView vkImageView = VK_NULL_HANDLE;
		// image view info
		VkImageViewCreateInfo imageViewInfo           = {};
		imageViewInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.pNext                           = NULL;
		imageViewInfo.format                          = format;
		imageViewInfo.components.r                    = VK_COMPONENT_SWIZZLE_R;
		imageViewInfo.components.g                    = VK_COMPONENT_SWIZZLE_G;
		imageViewInfo.components.b                    = VK_COMPONENT_SWIZZLE_B;
		imageViewInfo.components.a                    = VK_COMPONENT_SWIZZLE_A;
		imageViewInfo.subresourceRange.aspectMask     = aspectMask;
		imageViewInfo.subresourceRange.baseMipLevel   = baseMipLevel;
		imageViewInfo.subresourceRange.levelCount     = levelCount;
		imageViewInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
		imageViewInfo.subresourceRange.layerCount     = layerCount;
		imageViewInfo.viewType                        = viewType;
		imageViewInfo.flags                           = 0;
		imageViewInfo.image                           = vkImage;
		LE_ASSERT(::vkCreateImageView(vkVariableManager->vkDevice,&imageViewInfo, NULL, &vkImageView) == VK_SUCCESS);
		return vkImageView;
	}

	static VkDeviceMemory allocateImageMemory(VkImage vkImage) {
		VkDeviceMemory vkDeviceMemory;
		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.pNext                = NULL;
		memoryAllocateInfo.allocationSize       = 0;
		memoryAllocateInfo.memoryTypeIndex      = 0;

		VkMemoryRequirements memoryRequirement = {};
		vkGetImageMemoryRequirements(vkVariableManager->vkDevice, 
			                         vkImage, &memoryRequirement);
		memoryAllocateInfo.allocationSize = memoryRequirement.size;

		VkFlags mask = 0;
		getMemoryTypeFromProperties(vkVariableManager->gpuMemoryProperties[0],
			                        memoryRequirement.memoryTypeBits,
			                        mask,

			                        &memoryAllocateInfo.memoryTypeIndex);
		LE_ASSERT(::vkAllocateMemory(vkVariableManager->vkDevice,
		                            &memoryAllocateInfo, 
			                         nullptr, 
			                        &vkDeviceMemory) == VK_SUCCESS);
		LE_ASSERT(::vkBindImageMemory(vkVariableManager->vkDevice,
			                          vkImage, 
			                          vkDeviceMemory,
			                          0) == VK_SUCCESS);
		return vkDeviceMemory;
	}

	static void bindImageMemory(VkImage image,VkDeviceMemory memory,VkDeviceSize offset = 0) {
		vkBindImageMemory(vkVariableManager->vkDevice, image, memory,offset);
	}

	static void destroyImage(VkImage vkImage) {
		if(vkImage == VK_NULL_HANDLE) {
			return;
		}
		vkDestroyImage(vkVariableManager->vkDevice, vkImage,nullptr);
	}

	static void destroyImageView(VkImageView vkImageView) {
		if (vkImageView == VK_NULL_HANDLE) {
			return;
		}
		vkDestroyImageView(vkVariableManager->vkDevice, vkImageView, nullptr);
	}

	static VkFramebuffer createFrameBuffer(const VkFramebufferCreateInfo *pCreateInfo) {
		VkFramebuffer frameBuffer;
		LE_ASSERT(::vkCreateFramebuffer(vkVariableManager->vkDevice, pCreateInfo, nullptr,&frameBuffer) == VK_SUCCESS);
		return frameBuffer;
	}

	static void destroyFrameBuffer(VkFramebuffer vkFrameBuffer) {
		if (vkFrameBuffer == VK_NULL_HANDLE) {
			return;
		}
		vkDestroyFramebuffer(vkVariableManager->vkDevice, vkFrameBuffer, nullptr);
	}

	static VkPipeline createGraphicsPipeline(VkPipelineCache cache, VkGraphicsPipelineCreateInfo pipelineInfo) {
		VkPipeline pipeline;
		vkCreateGraphicsPipelines(vkVariableManager->vkDevice, cache,1, &pipelineInfo, nullptr, &pipeline);
		return pipeline;
	}

	static void destroyGraphicsPipeline() {

	}

	static VkPipelineCache createGraphicsPipelineCache() {
		VkPipelineCacheCreateInfo pipelineCacheInfo;
		pipelineCacheInfo.sType           = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		pipelineCacheInfo.pNext           = nullptr;
		pipelineCacheInfo.initialDataSize = 0;
		pipelineCacheInfo.pInitialData    = nullptr;
		pipelineCacheInfo.flags           = 0;
		VkPipelineCache pipelineCache;
		vkCreatePipelineCache(vkVariableManager->vkDevice, &pipelineCacheInfo, nullptr, &pipelineCache);
		return pipelineCache;
	}

	static void destroyGraphicsPipelineCache(VkPipelineCache pipelineCache) {
		vkDestroyPipelineCache(vkVariableManager->vkDevice, pipelineCache, nullptr );
	}

	static VkCommandBuffer allocateCommandBuffer(VkCommandPool vkCommandPool) {
		VkCommandBuffer vkCommandBuffer = VK_NULL_HANDLE;
		VkCommandBufferAllocateInfo allocationInfo = {};
		allocationInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocationInfo.commandPool                 = vkCommandPool;
		allocationInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocationInfo.commandBufferCount          = 1;
		// Create Command Buffer
		LE_ASSERT(::vkAllocateCommandBuffers(vkVariableManager->vkDevice,&allocationInfo, &vkCommandBuffer) == VK_SUCCESS);
	}

	static void destroyCommandBuffer() {
		
	}

	static VkCommandPool createCommandPool() {
		VkCommandPool commandPool = VK_NULL_HANDLE;
		VkCommandPoolCreateInfo cmdPoolCreateInfo = {};
		cmdPoolCreateInfo.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cmdPoolCreateInfo.queueFamilyIndex        = vkVariableManager->gpuQueueGraphicsFamilyIndex;
		cmdPoolCreateInfo.flags                   = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		// Create Command Pool
		LE_ASSERT(::vkCreateCommandPool(vkVariableManager->vkDevice,&cmdPoolCreateInfo, NULL, &commandPool)== VK_SUCCESS);
	}

	static void destroyCommandPool() {

	}

	static VkRenderPass createRenderPass(VkRenderPassCreateInfo *pCreateInfo) {
		VkRenderPass renderPass;
		vkCreateRenderPass(vkVariableManager->vkDevice, pCreateInfo, nullptr, &renderPass);
		return renderPass;
	}

	static void destroyRenderPass() {

	}

	static VkShaderModule createShaderModule(uint32_t codeSize, uint32_t* pCode) {
		VkShaderModule shaderModule;
		VkShaderModuleCreateInfo  shaderCreateInfo = {};
		shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderCreateInfo.pNext = NULL;
		shaderCreateInfo.flags = 0;
		shaderCreateInfo.codeSize = codeSize;
		shaderCreateInfo.pCode = pCode;
		LE_ASSERT(::vkCreateShaderModule(vkVariableManager->vkDevice, &shaderCreateInfo, NULL, &shaderModule) == VK_SUCCESS);
		return shaderModule;
	}

	static void destroyShaderModule(VkShaderModule shaderModule) {
		if(shaderModule) {
			vkDestroyShaderModule(vkVariableManager->vkDevice,shaderModule,nullptr);
		}
	}

	static VkQueue getGraphicsQueue() {
		static VkQueue queue;
		if(!queue) {
			vkGetDeviceQueue(vkVariableManager->vkDevice, vkVariableManager->gpuQueueGraphicsFamilyIndex, 0, &queue);
		}
		return queue;
	}

	static VkQueue getComputeQueue() {
		static VkQueue queue;
		if (!queue) {
			// vkGetDeviceQueue(vkVariableManager->vkDevice, vkVariableManager->gpuQueueGraphicsFamilyIndex, 0, &queue);
		}
		return queue;
	}

	static VkQueue getPresentQueue() {
		static VkQueue queue;
		if (!queue) {
			vkGetDeviceQueue(vkVariableManager->vkDevice, vkVariableManager->gpuQueuePresentFamilyIndex, 0, &queue);
		}
		return queue;
	}

	static uint32_t getGraphicsQueueIndex() {
		return vkVariableManager->gpuQueueGraphicsFamilyIndex;
	}

	static uint32_t getComputeQueueIndex() {
		return -1;
	}

	static uint32_t getPresentQueueIndex() {
		return vkVariableManager->gpuQueuePresentFamilyIndex;
	}

	static VkSemaphore createSemaphore() {
		VkSemaphore semaphore;
		VkSemaphoreCreateInfo imageAcquiredSemaphoreCreateInfo;
		imageAcquiredSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		imageAcquiredSemaphoreCreateInfo.pNext = NULL;
		imageAcquiredSemaphoreCreateInfo.flags = 0;
		LE_ASSERT(vkCreateSemaphore(vkVariableManager->vkDevice, &imageAcquiredSemaphoreCreateInfo, NULL, &semaphore) == VK_SUCCESS);
		return semaphore;
	}

	static void destroySemaphore(VkSemaphore semaphore) {
		if(semaphore) {
			vkDestroySemaphore(vkVariableManager->vkDevice,semaphore, nullptr);
		}
	}

	static VkFence createFence() {
		VkFence vkFence = VK_NULL_HANDLE;
		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType             = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.pNext             = NULL;
		fenceInfo.flags             = 0;
		vkCreateFence(vkVariableManager->vkDevice, &fenceInfo, nullptr, &vkFence);
		return vkFence;
	}

	static void destroyFence(VkFence vkFence) {
		if (vkFence == VK_NULL_HANDLE) {
			return;
		}
		vkDestroyFence(vkVariableManager->vkDevice, vkFence, nullptr);
	}

	static bool getMemoryTypeFromProperties(VkPhysicalDeviceMemoryProperties& props,
	                                        uint32_t                          typeBit, 
	                                        VkFlags                           reqMask, 
	                                        uint32_t*                         typeIndex) {
		for (int i = 0; i < 32; i++) {
			if ((typeBit & 1) == 1) {
				if ((props.memoryTypes[i].propertyFlags & reqMask) == reqMask) {
					*typeIndex = i;
					return true;
				}
			}
			typeBit >>= 1;
		}
		return false;
	}

	static uint32_t acquireNextImageIndex(VkSwapchainKHR swapChain,VkSemaphore semaphore) {
		uint32_t swapChainIndex;
		VkResult result = vkAcquireNextImageKHR(vkVariableManager->vkDevice, swapChain, UINT64_MAX, semaphore, VK_NULL_HANDLE, &swapChainIndex);
		LE_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR);
		return swapChainIndex;
	}

	static VkSwapchainKHR createSwapChain(VkSurfaceKHR surface, uint32_t windowWidth, uint32_t windowHeight, uint32_t* pSwapchainImageCount) {
		// traverse all the queue families to find one that support present
		VkBool32* pSupportsPresent = LEVkDevice::getPhysicalDeviceSurfaceSupport(surface);
		// the index of Queue Family that support Graphics Work 
		unsigned int gpuQueueGraphicsFamilyIndex = UINT32_MAX;
		// the index of Queue Family that support Present Work 
		unsigned int gpuQueuePresentFamilyIndex = UINT32_MAX;

		// traverse all the Queue Families
		for (int i = 0; i < vkVariableManager->gpuQueueFamilyCount; i++) {
			// if supports Graphics work
			if ((vkVariableManager->gpuQueueFamilyProperty[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
				// record the index
				if (gpuQueueGraphicsFamilyIndex == UINT32_MAX) {
					gpuQueueGraphicsFamilyIndex = i;
				}
				// if supports both Graphics and Present work
				if (pSupportsPresent[i] == VK_TRUE) {
					gpuQueueGraphicsFamilyIndex = gpuQueuePresentFamilyIndex = i;
					break;
				}
			}
		}
		// if didn't find a Queue Family that supports both Graphics and Present work
		// find and record it's index
		if (gpuQueuePresentFamilyIndex == UINT32_MAX) {
			for (int i = 0; i < vkVariableManager->gpuQueueFamilyCount; ++i) {
				if (pSupportsPresent[i] == VK_TRUE) {
					gpuQueuePresentFamilyIndex = i;
					break;
				}
			}
		}
		LE_SAFE_RELEASE(pSupportsPresent);
		// make sure we have found found Queue Family supports Graphics work and Queue Family supports Present work
		LE_ASSERT(!(gpuQueueGraphicsFamilyIndex == UINT32_MAX || gpuQueuePresentFamilyIndex == UINT32_MAX));
		// surface's format
		const auto surfaceFormats = LEVkDevice::getPhysicalDeviceSurfaceFormats(surface);
		// surface's present mode
		const VkPresentModeKHR swapchainPresentMode = LEVkDevice::getPhysicalDeviceSurfacePresentMode(surface);
		// swapchain's extent
		const VkExtent2D swapChainExtent = LEVkDevice::getExpectedSwapchainExtent(surface, windowWidth,windowHeight);
		// swapchain's image count
		const uint32_t desiredMinNumberOfSwapChainImages = LEVkDevice::getExpectedSwapChainImageCount(surface);
		// swapchain's transform
		VkSurfaceTransformFlagBitsKHR surfaceTransformFlagBits = LEVkDevice::getSurfaceTransformFlag(surface);
		VkSwapchainCreateInfoKHR  swapChainCreateInfo = {};
		swapChainCreateInfo.pNext                     = nullptr;
		swapChainCreateInfo.sType                     = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChainCreateInfo.surface                   = surface;
		swapChainCreateInfo.minImageCount             = desiredMinNumberOfSwapChainImages;
		swapChainCreateInfo.imageFormat               = surfaceFormats[0];
		swapChainCreateInfo.imageExtent.width         = swapChainExtent.width;
		swapChainCreateInfo.imageExtent.height        = swapChainExtent.height;
		swapChainCreateInfo.imageArrayLayers          = 1;
		swapChainCreateInfo.presentMode               = swapchainPresentMode;
		swapChainCreateInfo.oldSwapchain              = VK_NULL_HANDLE;
		swapChainCreateInfo.clipped                   = true;
		swapChainCreateInfo.imageColorSpace           = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
		swapChainCreateInfo.imageUsage                = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		swapChainCreateInfo.imageSharingMode          = VK_SHARING_MODE_EXCLUSIVE;
		swapChainCreateInfo.queueFamilyIndexCount     = 0;
		swapChainCreateInfo.pQueueFamilyIndices       = NULL;
		swapChainCreateInfo.preTransform              = surfaceTransformFlagBits;
		swapChainCreateInfo.compositeAlpha            = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		// If the queue family that supports graphics and display work is different 
		if (gpuQueueGraphicsFamilyIndex != gpuQueuePresentFamilyIndex) {
			swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapChainCreateInfo.queueFamilyIndexCount = 2;
			unsigned int queueFamilyIndices[2] = { gpuQueueGraphicsFamilyIndex, gpuQueuePresentFamilyIndex };
			swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		*pSwapchainImageCount = desiredMinNumberOfSwapChainImages;
		VkSwapchainKHR swapchain;
		vkCreateSwapchainKHR(vkVariableManager->vkDevice, &swapChainCreateInfo, nullptr,&swapchain);
		return swapchain;
	}

	static void destroySwapChain(VkSwapchainKHR vkSwapchain) {
		if (vkSwapchain == VK_NULL_HANDLE) {
			return;
		}
		vkDestroySwapchainKHR(vkVariableManager->vkDevice, vkSwapchain, nullptr);
	}

	template<typename T>
	static T getProcessAddress(const char* name) {
		void* pAddress = vkGetDeviceProcAddr(vkVariableManager->vkDevice, name);
		return static_cast<T>(pAddress);
	}

	static VkBool32* getPhysicalDeviceSurfaceSupport(VkSurfaceKHR surface) {
		VkBool32* pSupported = new VkBool32[vkVariableManager->gpuQueueFamilyCount];
		for (int i = 0; i < getQueueFamilyCount(); i++) {
			vkGetPhysicalDeviceSurfaceSupportKHR(vkVariableManager->gpus[0], i, surface, &pSupported[i]);
		}
		return pSupported;
	}

	static uint32_t getQueueFamilyCount() {
		return vkVariableManager->gpuQueueFamilyCount;
	}

	static std::vector<VkFormat> getPhysicalDeviceSurfaceFormats(VkSurfaceKHR surface) {
		uint32_t formatCount = -1;
		LE_ASSERT(::vkGetPhysicalDeviceSurfaceFormatsKHR(vkVariableManager->gpus[0], surface,&formatCount,nullptr) == VK_SUCCESS);
		// color space is ignored
		VkSurfaceFormatKHR* vkSurfaceformats = new VkSurfaceFormatKHR[formatCount];
		// get the supported formats of Surface
		LE_ASSERT(::vkGetPhysicalDeviceSurfaceFormatsKHR(vkVariableManager->gpus[0],surface,&formatCount,vkSurfaceformats) == VK_SUCCESS);
		std::vector<VkFormat> vkFormats(formatCount);
		for (int i = 0; i < formatCount; i++) {
			// record supported formats
			vkFormats[i] = vkSurfaceformats[i].format;
		}
		// if format is undefined , use generic format instead
		if (formatCount == 1 && vkSurfaceformats[0].format == VK_FORMAT_UNDEFINED) {
			vkFormats[0] = VK_FORMAT_B8G8R8A8_UNORM;
		}
		return vkFormats;
	}

	static VkSurfaceCapabilities2KHR getPhysicalSurfaceCapabilities(VkSurfaceKHR surface) {
		VkSurfaceCapabilities2KHR vkSurfaceCapabilities2KHR = {};
		vkSurfaceCapabilities2KHR.pNext = nullptr;
		vkSurfaceCapabilities2KHR.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;

		VkPhysicalDeviceSurfaceInfo2KHR vkPhysicalDeviceSurfaceInfo2KHR;
		vkPhysicalDeviceSurfaceInfo2KHR.sType   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
		vkPhysicalDeviceSurfaceInfo2KHR.surface = surface;
		vkPhysicalDeviceSurfaceInfo2KHR.pNext   = 0;

		// get capabilities of Surface
		LE_ASSERT(vkGetPhysicalDeviceSurfaceCapabilities2KHR(vkVariableManager->gpus[0], &vkPhysicalDeviceSurfaceInfo2KHR, &vkSurfaceCapabilities2KHR) == VK_SUCCESS);
		return vkSurfaceCapabilities2KHR;
	}

	static VkPresentModeKHR getPhysicalDeviceSurfacePresentMode(VkSurfaceKHR surface) {
		uint32_t vkPresentModeCount = -1;
		LE_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(vkVariableManager->gpus[0],surface,&vkPresentModeCount,NULL) == VK_SUCCESS);

		std::vector<VkPresentModeKHR> vkPresentModes(vkPresentModeCount);
		LE_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(vkVariableManager->gpus[0],surface,&vkPresentModeCount,vkPresentModes.data()) == VK_SUCCESS);

		VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
		 // Detail of VK PRESENT MODE
		 // VK_PRESENT_MODE_IMMEDIATE_KHR:
		 // Means that images submitted will be immediately presented to the screen,which may cause tearing;

		 // VK_PRESENT_MODE_FIFO_KHR:
		 // Means that the swap chain is a queue
		 // When it is displayed, it takes an image from the head of the queue
		 // and the program inserts the rendered image to the end of the queue.
		 // If the queue is full, the program has to wait, which is almost like vertical synchronization
		 // and the display refresh time is vertical blank;


		 // VK_PRESENT_MODE_FIFO_RELAXED_KHR:
		 // In the last vertical blank, if the application is late and the queue is empty //
		 // the mode will be different from the previous one.In this way, 
		 // instead of waiting for the next vertical blank, 
		 // the image will be transmitted directly to the screen, which may cause tearing;

		 // VK_PRESENT_MODE_MAILBOX_KHR:
		 // This is another variation of the second mode.When the queue is full, 
		 // it will not block the application, and the images already in the queue will be replaced with new ones.
		 // This mode can achieve triple buffering and avoid tearing,
		 // which is much less delay than vertical synchronization using double buffering.

		for (int i = 0; i < swapchainPresentMode; i++) {
			if (vkPresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
				swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
				break;
			}
			if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) &&
				(vkPresentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)) {
				swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			}
		}
		return swapchainPresentMode;
	}

	static VkExtent2D getExpectedSwapchainExtent(VkSurfaceKHR surface,uint32_t windowWidth,uint32_t windowHeight) {
		// surface's capabilities
		VkSurfaceCapabilities2KHR vkSurfaceCapabilities2KHR = LEVkDevice::getPhysicalSurfaceCapabilities(surface);
		VkSurfaceCapabilitiesKHR vkSurfaceCapabilities = vkSurfaceCapabilities2KHR.surfaceCapabilities;

		VkExtent2D extent;
		if (vkSurfaceCapabilities.currentExtent.width == 0xFFFFFFFF) {
			extent.width  = windowWidth;
			extent.height = windowHeight;
			// limit the maximum of height and width of swapchain
			if (extent.width < vkSurfaceCapabilities.minImageExtent.width) {
				extent.width = vkSurfaceCapabilities.minImageExtent.width;
			}
			else if (extent.width > vkSurfaceCapabilities.maxImageExtent.width) {
				extent.width = vkSurfaceCapabilities.maxImageExtent.width;
			}
			if (extent.height < vkSurfaceCapabilities.minImageExtent.height) {
				extent.height = vkSurfaceCapabilities.minImageExtent.height;
			}
			else if (extent.height > vkSurfaceCapabilities.maxImageExtent.height) {
				extent.height = vkSurfaceCapabilities.maxImageExtent.height;
			}
		}
		else {
			// if has certain extent
			extent = vkSurfaceCapabilities.currentExtent;
		}
		return extent;
	}

	static uint32_t getExpectedSwapChainImageCount(VkSurfaceKHR surface) {
		// surface's capabilities
		VkSurfaceCapabilities2KHR vkSurfaceCapabilities2KHR = LEVkDevice::getPhysicalSurfaceCapabilities(surface);
		VkSurfaceCapabilitiesKHR vkSurfaceCapabilities = vkSurfaceCapabilities2KHR.surfaceCapabilities;

		uint32_t desiredMinNumberOfSwapChainImages = vkSurfaceCapabilities.minImageCount + 1;
		// limit the quantity to given range
		if ((vkSurfaceCapabilities.maxImageCount > 0) &&
			(desiredMinNumberOfSwapChainImages > vkSurfaceCapabilities.maxImageCount)) {
			desiredMinNumberOfSwapChainImages = vkSurfaceCapabilities.maxImageCount;
		}
		return desiredMinNumberOfSwapChainImages;
	}

	static VkSurfaceTransformFlagBitsKHR getSurfaceTransformFlag(VkSurfaceKHR surface) {
		// surface's capabilities
		VkSurfaceCapabilities2KHR vkSurfaceCapabilities2KHR = LEVkDevice::getPhysicalSurfaceCapabilities(surface);
		VkSurfaceCapabilitiesKHR vkSurfaceCapabilities = vkSurfaceCapabilities2KHR.surfaceCapabilities;

		VkSurfaceTransformFlagBitsKHR surfaceTransformFlagBits;
		// If it supports required transformation
		if (vkSurfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
			surfaceTransformFlagBits = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}

		else {
			surfaceTransformFlagBits = vkSurfaceCapabilities.currentTransform;
		}
		return surfaceTransformFlagBits;
	}

	static std::vector<VkImage> getSwapchainImages(VkSwapchainKHR swapchain,uint32_t swapchainImageCount) {
		std::vector<VkImage> images;
		// get the number of images in the swap chain 
		LE_ASSERT(::vkGetSwapchainImagesKHR(vkVariableManager->vkDevice, swapchain, &swapchainImageCount, NULL) == VK_SUCCESS);
		images.resize(swapchainImageCount);
		LE_ASSERT(::vkGetSwapchainImagesKHR(vkVariableManager->vkDevice, swapchain, &swapchainImageCount, images.data()) == VK_SUCCESS);
		return images;
	}

	static VkDescriptorSetLayout createDescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& bindings) {
		VkDescriptorSetLayout descriptorSetLayout;
		// descriptor set layout
		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType                           = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.pNext                           = nullptr;
		layoutInfo.bindingCount                    = bindings.size();
		layoutInfo.pBindings                       = bindings.data();
		layoutInfo.flags                           = 0;
		LE_ASSERT(vkCreateDescriptorSetLayout(vkVariableManager->vkDevice, &layoutInfo, NULL, &descriptorSetLayout) == VK_SUCCESS);
		return descriptorSetLayout;
	}

	static void destroyDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout) {
		vkDestroyDescriptorSetLayout(vkVariableManager->vkDevice, descriptorSetLayout, nullptr);
	}

	static VkPipelineLayout createPipelineLayout(const std::vector<VkPushConstantRange> pushConstantRanges,VkDescriptorSetLayout descriptorSetLayout) {
		VkPipelineLayout pipelineLayout;
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
		pipelineLayoutCreateInfo.sType                      = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.pNext                      = NULL;
		pipelineLayoutCreateInfo.pushConstantRangeCount     = pushConstantRanges.size();
		pipelineLayoutCreateInfo.pPushConstantRanges        = pushConstantRanges.data();
		pipelineLayoutCreateInfo.setLayoutCount             = 1;
		pipelineLayoutCreateInfo.pSetLayouts                = &descriptorSetLayout;
		LE_ASSERT(vkCreatePipelineLayout(vkVariableManager->vkDevice, &pipelineLayoutCreateInfo, NULL, &pipelineLayout) == VK_SUCCESS);
		return pipelineLayout;
	}

	static void destroyPipelineLayout(VkPipelineLayout pipelineLayout) {
		if (pipelineLayout == VK_NULL_HANDLE) {
			return;
		}
		vkDestroyPipelineLayout(vkVariableManager->vkDevice, pipelineLayout, nullptr);
	}

	static void* mapMemory(VkDeviceMemory deviceMemory,uint32_t byteCount) {
		void* p = nullptr;
		const auto result = vkMapMemory(vkVariableManager->vkDevice,
			                            deviceMemory,
			                            0, // offset
			                            byteCount,
			                            0, // flag
			                            reinterpret_cast<void**>(&p)) == VK_SUCCESS;
		LE_ASSERT_MSG(result == true, "Map memory failed!");
		return p;
	}

	static void unmapMemory(VkDeviceMemory deviceMemory) {
		vkUnmapMemory(vkVariableManager->vkDevice, deviceMemory);
	}

	static std::tuple<VkBuffer,VkDeviceMemory, uint32_t> createBuffer(VkBufferUsageFlags usage,
		                         uint32_t byteCount,
		                         std::vector<uint32_t> pQueueFamilyIndices = std::vector<uint32_t>(),
		                         VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		                         VkBufferCreateFlags flag = 0) {
		VkBuffer       vkBuffer;
		VkDeviceMemory vkDeviceMemory;
		uint32_t       allocateByteCount;

		VkBufferCreateInfo bufferInfo    = {};
		bufferInfo.sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.pNext                 = nullptr;
		bufferInfo.usage                 = usage;
		bufferInfo.size                  = byteCount;
		bufferInfo.queueFamilyIndexCount = pQueueFamilyIndices.size();
		bufferInfo.pQueueFamilyIndices   = pQueueFamilyIndices.data();
		bufferInfo.sharingMode           = sharingMode;
		bufferInfo.flags                 = flag;

		LE_ASSERT(vkCreateBuffer(vkVariableManager->vkDevice,
			                     &bufferInfo,
			                     NULL,
			                     &vkBuffer) == VK_SUCCESS);

		VkMemoryRequirements memoryRequirements = {};
		vkGetBufferMemoryRequirements(vkVariableManager->vkDevice,
			                          vkBuffer,
			                          &memoryRequirements);

		VkMemoryAllocateInfo allocateInfo = {};
		allocateInfo.sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.pNext                = NULL;
		allocateInfo.memoryTypeIndex      = 0;
		allocateInfo.allocationSize       = memoryRequirements.size;

		allocateByteCount = memoryRequirements.size;

		VkFlags bufferMemoryMask = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		LE_ASSERT(getMemoryTypeFromProperties(vkVariableManager->gpuMemoryProperties[0],
			                                  memoryRequirements.memoryTypeBits,
			                                  bufferMemoryMask,
			                                  &allocateInfo.memoryTypeIndex)
		);
		LE_ASSERT(vkAllocateMemory(vkVariableManager->vkDevice,&allocateInfo,nullptr,&vkDeviceMemory) == VK_SUCCESS);
		LE_ASSERT(vkBindBufferMemory(vkVariableManager->vkDevice,vkBuffer,vkDeviceMemory,0) == VK_SUCCESS);
		return { vkBuffer,vkDeviceMemory,allocateByteCount };
	}

	static void destroyBuffer(VkBuffer vkBuffer,VkDeviceMemory memory) {
		vkDestroyBuffer(vkVariableManager->vkDevice, vkBuffer, nullptr);
		vkFreeMemory(vkVariableManager->vkDevice, memory, nullptr);
	}

	static VkSampler createSampler(VkSamplerCreateInfo samplerInfo) {
		VkSampler sampler;
		vkCreateSampler(vkVariableManager->vkDevice, &samplerInfo, nullptr,&sampler);
		return sampler;
	}

	static void destroySampler(VkSampler sampler) {
		if (sampler == VK_NULL_HANDLE) {
			return;
		}
		vkDestroySampler(vkVariableManager->vkDevice, sampler, nullptr);
	}

	static void releaseMemory(VkDeviceMemory memory) {
		if (memory == VK_NULL_HANDLE) {
			return;
		}
		vkFreeMemory(vkVariableManager->vkDevice, memory, nullptr);
	}

	static VkPhysicalDeviceLimits getPhysicalDeviceLimits() {
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(vkVariableManager->gpus[0], &physicalDeviceProperties);
		return physicalDeviceProperties.limits;
	}

	static void flushMappedMemoryRanges(VkDeviceMemory deviceMemory, VkDeviceSize offset, VkDeviceSize size) {
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType               = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory              = deviceMemory;
		mappedRange.offset              = offset;
		mappedRange.size                = size;
		vkFlushMappedMemoryRanges(vkVariableManager->vkDevice, 1, &mappedRange);
	}

	static VkFormat getDepthStencilFormat() {
		static VkFormat depthFormat = VK_FORMAT_UNDEFINED;
		if (depthFormat == VK_FORMAT_UNDEFINED) {
			std::array<VkFormat, 5> candidateFormats = {
				VK_FORMAT_D32_SFLOAT_S8_UINT,
				VK_FORMAT_D32_SFLOAT,
				VK_FORMAT_D24_UNORM_S8_UINT,
				VK_FORMAT_D16_UNORM_S8_UINT,
				VK_FORMAT_D16_UNORM
			};
			for (const auto& format:candidateFormats) {
				VkFormatProperties formatProps;
				vkGetPhysicalDeviceFormatProperties(vkVariableManager->gpus[0], format, &formatProps);
				if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
					depthFormat = format;
					return format;
				}
			}
		}
		return depthFormat;
	}

	static void acquireNextImage(VkSwapchainKHR swapchain,VkSemaphore semaphore,VkFence fence,uint32_t* pImageIndex){
		const VkResult result = vkAcquireNextImageKHR(vkVariableManager->vkDevice, swapchain, UINT64_MAX,semaphore, fence, pImageIndex);
		LE_ASSERT(result == VK_SUCCESS);
	}

	static void resetFence(VkFence vkFence) {
		vkResetFences(vkVariableManager->vkDevice, 1, &vkFence);
	}

	static void waitFence(VkFence vkFence) {
		VkResult result;
		do {
			// Wait for the rendering to complete
			result = vkWaitForFences(vkVariableManager->vkDevice, 1, &vkFence, VK_TRUE, 0x1000);
		} while (result == VK_TIMEOUT);
	}

	static VkDescriptorPool createDescriptorPool(std::vector<VkDescriptorPoolSize> poolSize,uint32_t count) {
		VkDescriptorPoolCreateInfo cb{};
		cb.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		cb.maxSets       = count;
		cb.poolSizeCount = poolSize.size();
		cb.pPoolSizes    = poolSize.data();
		cb.pNext         = nullptr;
		
		VkDescriptorPool descriptorPool;
		vkCreateDescriptorPool(vkVariableManager->vkDevice, &cb, nullptr,&descriptorPool);
		return descriptorPool;
	}

	static std::vector<VkDescriptorSet> allocateDescriptorSet(VkDescriptorPool descriptorPool,VkDescriptorSetLayout layout, uint32_t count) {
		std::vector<VkDescriptorSet>       descriptorSets(count);
		std::vector<VkDescriptorSetLayout> descriptorSetLayout(count, layout);
		VkDescriptorSetAllocateInfo descriptorAllocateInfo{};
		descriptorAllocateInfo.descriptorPool     = descriptorPool;
		descriptorAllocateInfo.descriptorSetCount = count;
		descriptorAllocateInfo.pNext              = nullptr;
		descriptorAllocateInfo.pSetLayouts        = descriptorSetLayout.data();
		descriptorAllocateInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		vkAllocateDescriptorSets(vkVariableManager->vkDevice, &descriptorAllocateInfo, descriptorSets.data());
		return descriptorSets;
	}

	static void updateDescriptorSets(uint32_t descriptorWriteCount,
		                  const VkWriteDescriptorSet* pDescriptorWrites,
		                  uint32_t descriptorCopyCount = 0,
		                  const VkCopyDescriptorSet* pDescriptorCopies = nullptr) {
		vkUpdateDescriptorSets(vkVariableManager->vkDevice, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
	}
};

#endif // !LE_VK_LOGICAL_DEVICE_H