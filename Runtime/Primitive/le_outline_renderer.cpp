/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Primitive
 * \File:    le_outline_renderer.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_outline_renderer.h"
#include "../Resource/le_material.h"
#include "../Renderer/le_camera.h"
#include "../Math/le_transform.h"
#include "../Base/le_entity.h"
#include "../Renderer/le_vk_debugger.h"

glm::vec4 LEOutlineRenderer::getOutlineColor() const {
	return this->m_outlineColor;
}

void LEOutlineRenderer::setOutlineColor(const glm::vec4& color) {
	this->m_outlineColor = color;
}

float LEOutlineRenderer::getOutlineThickness() const {
	return this->m_outlineThickness;
}

void LEOutlineRenderer::setOutlineThickness(float thickness) {
	this->m_outlineThickness = thickness;
}

void LEOutlineRenderer::render(VkCommandBuffer cmdBuffer, int materialIndex) {
	LE_ASSERT_MSG(cmdBuffer != VK_NULL_HANDLE, "'function 'render' in primitiveRenderer must be called after command buffer is begin");
	if (!this->m_pRendererCount || !this->m_pRenderingCamera) {
		return;
	}
	// prepare material
	auto pMaterial = this->getDefaultMaterial();
	pMaterial->beginSubPass(cmdBuffer, 0);
	// set the outline color
	pMaterial->pushConstantVector4("color", 0, VK_SHADER_STAGE_VERTEX_BIT, this->m_outlineColor);
	// move the object a distance in the opposite direction of camera's orientation
	auto pRenderTarget = this->m_pRenderingCamera;
	glm::vec3 translation = -this->m_pRenderingCamera->getDirection() * this->m_outlineThickness;
	for (int i = 0; i < this->m_pRendererCount; i++) {
		auto pRenderer = this->m_ppRenderers[i];
		auto pEntity   = pRenderer->getAttachedEntity();
		// move first
		pEntity->getTransform()->translate(translation);
		pMaterial->pushConstantMatrix("model", 0, VK_SHADER_STAGE_VERTEX_BIT, pEntity->getTransform()->getWorldMatrix());
		LEVkDebugger::insert(cmdBuffer, "Draw \"outline_" + pEntity->getName() + "\"", glm::vec4(0.0f));
		auto pMesh = pRenderer->getMesh();
		if (pMesh != nullptr) {
			auto vbo = pMesh->getVertexBuffer();
			auto ibo = pMesh->getIndexBuffer();
			const VkDeviceSize offset[] = { 0 };
			vkCmdBindVertexBuffers(cmdBuffer, 0, 1,
				&vbo->get()->getHandle(), offset
			);
			vkCmdBindIndexBuffer(cmdBuffer, ibo->get()->getHandle(), 0, VK_INDEX_TYPE_UINT32);
			vkCmdDrawIndexed(cmdBuffer, ibo->getIndexCount(), 1, 0, 0, 0);
		}
		// restore to original position
		pEntity->getTransform()->translate(-translation);
	}
	pMaterial->endSubPass();
}

LEMaterial* LEOutlineRenderer::getDefaultMaterial() const {
	static LEMaterial* pMaterial = nullptr;
	if (!pMaterial) {
		// load matertial
		static LEShader* vsShaderOutline = nullptr;
		static LEShader* fsShaderOutline = nullptr;
		std::string errorString;
		vsShaderOutline = LEShaderCompiler::getInstance()->compile("shaders\\le_outline.vert",VK_SHADER_STAGE_VERTEX_BIT);
		fsShaderOutline = LEShaderCompiler::getInstance()->compile("shaders\\le_outline.frag", VK_SHADER_STAGE_FRAGMENT_BIT);
		LE_ASSERT_MSG(vsShaderOutline != nullptr && fsShaderOutline != nullptr, "load build-in material 'outline' failed!");
		pMaterial = new LEMaterial;;
		pMaterial->addSubPass(vsShaderOutline, fsShaderOutline);
	}
	return pMaterial;
}

void LEOutlineRenderer::render(VkCommandBuffer cmdBuffer,LECamera* pCamera,LEPrimitiveRenderer* pRenderer) {
	this->m_pRenderingCamera = pCamera;
	this->m_pRendererCount = 1;
	this->m_ppRenderers    = &pRenderer;
	this->render(cmdBuffer, 0);
}

void LEOutlineRenderer::render(VkCommandBuffer cmdBuffer, LECamera* pCamera, RendererVector& pRenderers) {
	this->m_pRenderingCamera = pCamera;
	this->m_pRendererCount = pRenderers.size();
	this->m_ppRenderers    = pRenderers.data();
	this->render(cmdBuffer, 0);
}

LEAABB* LEOutlineRenderer::getAABB() const {
	return nullptr;
}
