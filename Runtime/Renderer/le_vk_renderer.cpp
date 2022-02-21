/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_vk_renderer.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_camera.h"
#include "le_vk_renderer.h"
#include "../LevelManage/le_world.h"
#include "../LevelManage/le_level.h"
#include "le_vk_instance.h"
#include "le_vk_device.h"
#include "le_vk_debugger.h"
#include "le_vk_loaded_api.h"
#include "Runtime/Base/le_class_info.h"
#include "Runtime/Base/le_initial.h"

// STL Includes
#include <unordered_set>

LE_INITIAL_IMPL_BEGIN(LEVulkanRenderer)
{
	LE_INITIAL_METHOD(&LEVulkanRenderer::initVulkanContext);
	LE_TERMINATE_METHOD(&LEVulkanRenderer::destoryVulkanContext);
}
LE_INITIAL_IMPL_END(LEVulkanRenderer);

LEVulkanRenderer::LEVulkanRenderer() {
}

LEVulkanRenderer::~LEVulkanRenderer() {

}

bool LEVulkanRenderer::initVulkanContext() {
	LEVkInstance::createGlobalInstance();
	LEVkDevice::createGlobalInstance();
	LEVkDebugger::installDebugCallBack();
	LELoadedAPI::loadAPIFromDLL();
	return true;
}

bool LEVulkanRenderer::destoryVulkanContext() {
	LEVkDevice::destroyGlobalInstance();
	LEVkInstance::destoryGlobalInstance();
	return true;
}

void LEVulkanRenderer::update() {
	// get all cameras in the active level.
	LEWorld* pWorld = LEWorld::getInstance();
	auto &cameras = pWorld->getActiveLevel()->getCameras();
	// Cameras with smaller depth values will render earlier 
	std::sort(cameras.begin(), cameras.end(), [](const LECamera* lhs, const LECamera* rhs) {
		return lhs->getDepth() > rhs->getDepth();
	});
	std::unordered_set<LERenderTarget*> pRenderTargetSet;
	
	for (int i = 0;i<cameras.size();i++) {
		LECamera * pCamera = cameras[i];
		if (!pCamera->isEnabled() ||(pCamera->getRenderTarget() == nullptr)) {
			continue;
		}
		pCamera->resetRenderContext();
		pWorld->getActiveLevel()->computeVisibleRenderer(pCamera);
		pCamera->render();
	}
}