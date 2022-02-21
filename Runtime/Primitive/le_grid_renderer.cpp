/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Primitive
 * \File:    le_grid_renderer.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_grid_renderer.h"
#include "../Base/le_entity.h"
#include "le_sprite_renderer.h"
#include "../Resource/le_mesh.h"
#include "../Math/le_transform.h"
#include "../Resource/le_texture.h"
#include "../Resource/le_material.h"
#include "../Renderer/le_vk_debugger.h"

LEGridRenderer::LEGridRenderer() {

}

LEGridRenderer::~LEGridRenderer() {

}

void LEGridRenderer::render(VkCommandBuffer cmdBuffer, int materialIndex) {
	// prepare material
	this->m_material->beginSubPass(cmdBuffer, 0);
	this->m_material->pushConstantMatrix("model", 0, VK_SHADER_STAGE_VERTEX_BIT, this->m_pAttachedEntity->getTransform()->getWorldMatrix());
	this->m_material->pushConstantVector4("color", 0, VK_SHADER_STAGE_VERTEX_BIT, { 1.0f,0.1f,0.5f,1.0f });

	// no VBO and IBO required
	// draw call for first pass
	LEVkDebugger::insert(cmdBuffer, "Draw \"" + this->m_pAttachedEntity->getName() + "\"", glm::vec4(0.0f));
	vkCmdDraw(cmdBuffer,6, 1, 0, 0);
	this->m_material->endSubPass();
}

LEMaterial* LEGridRenderer::getDefaultMaterial() const {
	return nullptr;
}

LEAABB* LEGridRenderer::getAABB() const {
	return nullptr;
}
