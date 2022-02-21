#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_render_target_window.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_RENDER_TARGET_WINDOW_H
#define LE_RENDER_TARGET_WINDOW_H

// LE Includes
#include "../Base/le_global.h"
#include "le_render_target.h"
#include "../Base/le_assert.h"
#include "le_vk_variable_manager.h"

#if defined(LE_EDITOR)
// qt window's includes
#include <QWidget>
#else
// GLFW Includes
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#pragma comment(lib,"glfw3.lib")
#endif 

#if defined(LE_PLATFORM_WINDOWS)
#include <windows.h>
#endif // !LE_PLATFORM_WINDOWS
// STL Includes
#include <string>
#include <vector>

#include "le_vk_device.h"

class LEVkRenderPass;
class LERenderTargetWindow:public LERenderTarget {
private:
#if !defined(LE_EDITOR)
	/* glfw context*/
	GLFWwindow*              m_glfwWindow;
#else
	QWidget*                 m_qtWindow;
#endif 
	/* vulkan context*/
	VkSurfaceKHR             m_vkSurface;
	VkExtent2D               m_vkSwapchainExtent;
	VkSwapchainKHR           m_vkSwapChain;
	VkSemaphore              m_renderFinishedSemaphore;
	VkSemaphore              m_imageAvailableSemaphore;
	uint32_t                 m_swapchainImageCount;
	mutable uint32_t         m_swapChainIndex;
	std::vector<VkImage>     m_swapchainImages;
	std::vector<VkImageView> m_swapchainImageViews;

	bool                     m_shouldClose;
	bool                     m_fullScreen;
	bool                     m_isVulkanContextCreated;
#if defined(LE_PLATFORM_WINDOWS)
	HWND                     m_hwnd;
#endif 
	void                     refreshWindowStatus() {
#if !defined(LE_EDITOR)
		glfwSetWindowMonitor(this->m_glfwWindow, 
			                 this->m_fullScreen ? glfwGetPrimaryMonitor() : nullptr,
                             0, 0, 1280, 720, 144);
#else
#endif
	}
public:
	                         LERenderTargetWindow(int width = 0,int height = 0,bool fullScreen = false,
	                         	                  const std::string& windowName = "Lampyris Render Window");
			                
	                        ~LERenderTargetWindow();

	void                     destroyVulkanContext() {
		LEVkDevice::destroyImage(this->m_depthStencilImage);
		LEVkDevice::destroyImageView(this->m_depthStencilImageView);
		LEVkDevice::releaseMemory(this->m_depthStencilImageMemory);
		LEVkDevice::destroySwapChain(this->m_vkSwapChain);
	}

	void                     recreateVulkanContext() {
		// destroy first
		this->destroyVulkanContext();
		// and re-create
		this->createSwapChain();
		this->createDepthStencilBuffer();
	}

	void                     createVulkanContext() {
		if(!m_isVulkanContextCreated) {
			this->createSwapChain();
			this->createDepthStencilBuffer();
			this->createSemaphore();
			this->createFence();
			this->m_isVulkanContextCreated = true;
		} else {
			this->m_isVulkanContextCreated = false;
			this->recreateVulkanContext();
			this->m_isVulkanContextCreated = true;
		}
	}

	bool                     showClose() const;
		                     
	void                     update();
		                     
	void                     createSwapChain();
				             
	void                     refreshFrameBuffer();
				             
	void                     present();
		                     
	void                     setFullScreen(bool fullScreen) {
		this->m_fullScreen = fullScreen;
	}
		                     
	void                     createRenderPass();
				             
	virtual VkImageLayout    getColorAttachmentFinalLayout() const;

	void                     createSemaphore();

	void                     clearColor();

	bool                     isVulkanContextInitialized() const {
		return this->m_isVulkanContextCreated;
	}

	void                     resize(uint32_t width,uint32_t height) {
		if(width >= (1<<16) || height >= (1<<16) || width == 0 || height == 0) {
			return;
		}
		this->m_width = width;
		this->m_height = height;
		this->createVulkanContext();
	}

	void                     acquireNextFrame() {
		LEVkDevice::acquireNextImage(this->m_vkSwapChain, this->m_imageAvailableSemaphore, VK_NULL_HANDLE, &this->m_swapChainIndex);
	}

	VkImageView              getRenderImageView() const override {
		return this->m_swapchainImageViews[this->m_swapChainIndex];
	}

	VkImage                  getRenderImage() const override {
		return this->m_swapchainImages[this->m_swapChainIndex];
	}

	VkSemaphore&             getImageAvailableSemaphore()  {
		return this->m_imageAvailableSemaphore;
	}

	uint32_t                 getRenderTargetType() const override {
		return LE_RENDER_TARGET_WINDOW;
	}
#if defined(LE_EDITOR)
	QWidget* getQtWindow() const { return this->m_qtWindow; }
#endif 
};
#endif // !LE_RENDER_TARGET_WINDOW_H