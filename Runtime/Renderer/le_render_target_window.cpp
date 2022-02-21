/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_render_target_window.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_vk_device.h"
#include "le_vk_instance.h"
#include "le_vk_render_pass.h"
#include "le_vk_command_buffer.h"
#include "le_render_target_window.h"
#include "../Interact/le_input_system.h"

LERenderTargetWindow::LERenderTargetWindow(int width, int height, bool fullScreen,const std::string& windowName):
	m_vkSurface(VK_NULL_HANDLE),m_swapchainImageCount(-1), 
	m_swapChainIndex(-1), m_vkSwapChain(VK_NULL_HANDLE), m_isVulkanContextCreated(false),
	LERenderTarget(width, height, VK_FORMAT_B8G8R8A8_UNORM) {
#if defined(LE_PLATFORM_WINDOWS)
#if defined(LE_EDITOR)
	// use qt window to render
	this->m_qtWindow = new QWidget;
	this->m_hwnd = reinterpret_cast<HWND>(this->m_qtWindow->winId());
#else 
	// use glfw to render
	// this->createNativeWindow();
	// return;
	glfwInit(); 
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWmonitor* pMonitor = fullScreen ? glfwGetPrimaryMonitor() : nullptr;
	//::glfwWindowHint(GLFW_RESIZABLE, true);
	this->m_glfwWindow = ::glfwCreateWindow(width,
		                  height,
		                  windowName.c_str(),
		                  pMonitor, nullptr);
	LE_ASSERT(this->m_glfwWindow != nullptr);
	this->m_hwnd = ::glfwGetWin32Window(this->m_glfwWindow);
#endif // !LE_EDITOR
#endif // !LE_PLATFORM_WINDOWS
}

LERenderTargetWindow::~LERenderTargetWindow() {
#if !defined(LE_EDITOR)
	glfwTerminate();
#endif 
}

bool LERenderTargetWindow::showClose() const {
#if defined(LE_EDITOR)
	// qt will close the window, so it's not necessary to worry about it
	return false;
#else
	return this->m_shouldClose;
#endif 
}

void LERenderTargetWindow::update() {
#if !defined(LE_EDITOR)
	while (!::glfwWindowShouldClose(m_glfwWindow)) {
		// update window event
		LEInputSystem::getInstance()->update();
		::glfwPollEvents();
	}
	this->m_shouldClose = true;
	::exit(0);
#endif // !LE_EDITOR
}

void LERenderTargetWindow::createSwapChain() {
	this->m_vkSurface           = LEVkInstance::createSurface(this->m_hwnd);
	this->m_vkSwapChain         = LEVkDevice::createSwapChain(this->m_vkSurface, this->m_width, this->m_height,&this->m_swapchainImageCount);
	this->m_swapchainImages     = LEVkDevice::getSwapchainImages(this->m_vkSwapChain, this->m_swapchainImageCount);
	this->m_swapchainImageViews.resize(this->m_swapchainImageCount);

	auto surfaceFormats = LEVkDevice::getPhysicalDeviceSurfaceFormats(this->m_vkSurface);
	for (int i = 0; i < this->m_swapchainImageCount; i++) {
		this->m_swapchainImageViews[i] = LEVkDevice::createImageView(this->m_swapchainImages[i], 
			                                                         surfaceFormats[0], 
			                                                         VK_IMAGE_ASPECT_COLOR_BIT,
			                                                         VK_IMAGE_VIEW_TYPE_2D);
	}
}


void LERenderTargetWindow::refreshFrameBuffer() {
	VkResult result = vkAcquireNextImageKHR(vkVariableManager->vkDevice, this->m_vkSwapChain, UINT64_MAX,
		this->m_imageAvailableSemaphore, VK_NULL_HANDLE, &this->m_swapChainIndex);
	LE_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR);
}

void LERenderTargetWindow::present() {
	VkPresentInfoKHR vkPresentInfo;
	vkPresentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	vkPresentInfo.pNext              = nullptr;
	vkPresentInfo.swapchainCount     = 1;
	vkPresentInfo.pSwapchains        = &this->m_vkSwapChain;
	vkPresentInfo.waitSemaphoreCount = 0;
	vkPresentInfo.pWaitSemaphores    = nullptr;
	vkPresentInfo.pResults           = nullptr;
	vkPresentInfo.pImageIndices      = &this->m_swapChainIndex;

	VkResult result;
	if((result = vkQueuePresentKHR(LEVkDevice::getGraphicsQueue(), &vkPresentInfo)) != VK_SUCCESS) {
		if(result == VK_ERROR_OUT_OF_DATE_KHR) {
			return;
		}
	}
	vkQueueWaitIdle(LEVkDevice::getGraphicsQueue());
}


void LERenderTargetWindow::createSemaphore() {
	this->m_imageAvailableSemaphore = LEVkDevice::createSemaphore();
	this->m_renderFinishedSemaphore = LEVkDevice::createSemaphore();
}

void LERenderTargetWindow::clearColor() {
	LECamera* pRenderingCamera = LECamera::getRenderingCamera();
	glm::vec4 color = pRenderingCamera->getClearColor();
	VkClearColorValue clearValue;
	clearValue.float32[0] = color.r;
	clearValue.float32[1] = color.g;
	clearValue.float32[2] = color.b;
	clearValue.float32[3] = color.a;
	const auto CMDBuffer  = pRenderingCamera->getCommandBuffer();
	CMDBuffer->clearColorImage(this->m_swapchainImages[this->m_swapChainIndex], VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, clearValue);
}

void LERenderTargetWindow::createRenderPass() {
}

VkImageLayout LERenderTargetWindow::getColorAttachmentFinalLayout() const {
	// the final layout of image in swapchain must be used for present.
	return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
}
