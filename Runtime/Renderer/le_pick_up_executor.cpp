/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_pick_up_executor.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_pick_up_executor.h"
#include "../Resource/le_material.h"
#include "le_render_target_texture.h"
#include "../Primitive/le_primitive_renderer.h"
#include "../Interact/le_input_system.h"
#include "../Base/le_entity.h"
#include "../Math/le_transform.h"

// GLM Includes
#include <glm/glm.hpp> // glm::clamp

LEPickUpExecutor::LEPickUpExecutor() :m_pickUpMaterial(new LEMaterial) {
	// load matertial
	LEShader* vsShaderPickup = nullptr;
	LEShader* fsShaderPickup = nullptr;
	std::string errorString;
	vsShaderPickup = LEShaderCompiler::getInstance()->compile("shaders\\le_pickup.vert", VK_SHADER_STAGE_VERTEX_BIT);
	fsShaderPickup = LEShaderCompiler::getInstance()->compile("shaders\\le_pickup.frag", VK_SHADER_STAGE_FRAGMENT_BIT);
	LE_ASSERT_MSG(vsShaderPickup != nullptr && fsShaderPickup != nullptr, "load build-in material 'pickup' failed!");
	// remember to disable color blend
	this->m_pickUpMaterial->addSubPass(vsShaderPickup, fsShaderPickup);
}

LEPickUpExecutor::~LEPickUpExecutor() {

}

LEPrimitiveRenderer* LEPickUpExecutor::execute(const std::vector<LERenderContext>& renderContext,glm::vec2 & mousePos) {
	/*
	// clear id-renderer map
	this->m_instanceIDMap.clear();
	// begin render
	LERenderTarget* pRenderTarget = this->m_pInstanceIDTexture;
	LERenderTarget::setActiveRenderTarget(pRenderTarget);
	this->m_pickUpMaterial->beginSubPass(pRenderTarget->getCommandBuffer(), 0);
	this->m_pickUpMaterial->setVector2("mousePos", 0, VK_SHADER_STAGE_FRAGMENT_BIT, mousePos);
	// render all visible objects
	for (int i = 0; i < renderContext.size(); i++) {
		auto pRenderer = renderContext[i].pRenderer;
		auto pMesh = pRenderer->getMesh();
		if (pMesh != nullptr) {
			const auto vbo = pMesh->getVertexBuffer();
			const auto ibo = pMesh->getIndexBuffer();
			const VkDeviceSize offset[] = { 0 };

			vkCmdBindVertexBuffers(pRenderTarget->getCommandBuffer(), 0, 1,
				&vbo->get()->getHandle(), offset
			);
			auto id = renderContext[i].pRenderer->getInstanceID();
			this->m_instanceIDMap[id] = pRenderer;
			this->m_pickUpMaterial->pushConstantMatrix("model",0, VK_SHADER_STAGE_VERTEX_BIT, pRenderer->getAttachedEntity()->getTransform()->getWorldMatrix());
			this->m_pickUpMaterial->pushConstantUInt("instanceID",0, VK_SHADER_STAGE_VERTEX_BIT, id);
			vkCmdBindIndexBuffer(pRenderTarget->getCommandBuffer(), ibo->get()->getHandle(), 0, VK_INDEX_TYPE_UINT32);
			vkCmdDrawIndexed(pRenderTarget->getCommandBuffer(), ibo->getIndexCount(), 1, 0, 0, 0);
		}
	}
	this->m_pickUpMaterial->endSubPass();
	// end render
	pRenderTarget->endRenderPass();
	pRenderTarget->endCommandBuffer();
	pRenderTarget->submitCommandBuffer();
	*/
	return nullptr;
}

void LEPickUpExecutor::create(LERenderTarget* pTarget) {
	LE_SAFE_RELEASE(this->m_pInstanceIDTexture);
	LE_SAFE_RELEASE(this->m_pVkBuffer);
	this->m_pInstanceIDTexture = new LERenderTargetTexture(pTarget->getWidth(), pTarget->getHeight(), VK_FORMAT_R32_UINT);
	this->m_pVkBuffer = new LEVkBuffer(this->m_pInstanceIDTexture->getImageByteCount(), VK_BUFFER_USAGE_TRANSFER_DST_BIT);
}
