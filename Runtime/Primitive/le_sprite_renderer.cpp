/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Primitive
 * \File:    le_sprite_renderer.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "../Base/le_entity.h"
#include "le_sprite_renderer.h"
#include "../Resource/le_mesh.h"
#include "../Math/le_transform.h"
#include "../Resource/le_texture.h"
#include "../Resource/le_material.h"
#include "../Renderer/le_vk_debugger.h"


void LESpriteRenderer::createMesh() {
	if (m_pTexture == nullptr) {
		return;
	}
	float widthHalf = m_pTexture->getWidth() / 20.0f;
	float heightHalf = m_pTexture->getHeight() / 20.0f;
	std::vector<glm::vec3> vertices = {
		// top-left
		{ -widthHalf,heightHalf,0 },
		// top-right
		{ widthHalf,heightHalf,0 },
		// bottom-left
		{ -widthHalf,-heightHalf,0 },
		// bottom-right
		{ widthHalf,-heightHalf,0 }
	};
	std::vector<glm::vec2> uv = {
		// top-left
		{ 0.0f,0.0f },
		// top-right
		{ 400.f/512.f,0.0f },
		// bottom-left
		{ 0.0f,727.f/1024.f },
		// bottom-right
		{ 400.f/512.f,727.f/1024.f },
	};
	std::vector<int> indices = { 0,1,2,2,1,3 };
	this->m_pMesh = LEMesh::fromPositionUV(vertices, uv, indices);
}

LESpriteRenderer::LESpriteRenderer() {

}

LESpriteRenderer::~LESpriteRenderer() {

}

LEMaterial* LESpriteRenderer::getDefaultMaterial() {
	static LEMaterial* pDefaultMaterial = nullptr;
	if (pDefaultMaterial) {
		pDefaultMaterial = new LEMaterial;
		std::string errorString;                                                                                                        
		LEShader* vsShaderSprite = LEShaderCompiler::getInstance()->compile("shaders\\le_sprite.vert",VK_SHADER_STAGE_VERTEX_BIT);
		if (!vsShaderSprite) {
			std::cout << "compile shaders\\le_sprite.vert failed: \n" << std::endl;
			std::cout << errorString << std::endl;;
		}
		LEShader* fsShaderSprite = LEShaderCompiler::getInstance()->compile("shaders\\le_sprite.frag", VK_SHADER_STAGE_FRAGMENT_BIT);
		if (!fsShaderSprite) {
			std::cout << "compile shaders\\le_sprite.frag failed: \n" << std::endl; 
			std::cout << errorString << std::endl;;
		}
		pDefaultMaterial->addSubPass(vsShaderSprite, fsShaderSprite);
	}
	return pDefaultMaterial;
}

void LESpriteRenderer::setSprite(LETexture2D* pSprite) {
	this->m_pTexture = pSprite;
	this->createMesh();
}

LETexture2D* LESpriteRenderer::getSprite() const {
	return this->m_pTexture;
}

void LESpriteRenderer::render(VkCommandBuffer cmdBuffer, int materialIndex) {
	// prepare material
	const auto pMaterial = this->m_material;
	pMaterial->beginSubPass(cmdBuffer, 0);
	pMaterial->pushConstantMatrix("model", 0, VK_SHADER_STAGE_VERTEX_BIT, this->m_pAttachedEntity->getTransform()->getWorldMatrix());
	pMaterial->pushConstantVector4("color", 0, VK_SHADER_STAGE_VERTEX_BIT, { 1.0f,0.1f,0.5f,1.0f });

	// prepare VBO and IBO
	const uint32_t vertexBindingCount = pMaterial->getInputCountAtPass(0);
	VkDeviceSize offsetsVertex[8] = {0};
	pMaterial->getInputVertexOffsetAtPass(0,offsetsVertex);
	VkBuffer vboBuffers[8];
	for (int i = 0; i < 8; i++) {
		vboBuffers[i] = this->m_pMesh->getVertexBuffer()->get()->getHandle();
		offsetsVertex[i] *= this->m_pMesh->getVertexBuffer()->getVertexCount();
	};
	vkCmdBindVertexBuffers(cmdBuffer, 0, vertexBindingCount,
		vboBuffers,
		offsetsVertex
	);
	vkCmdBindIndexBuffer(cmdBuffer, this->m_pMesh->getIndexBuffer()->get()->getHandle(), 0, VK_INDEX_TYPE_UINT32);
	// draw call for first pass
	LEVkDebugger::insert(cmdBuffer, "Draw \"" + this->m_pAttachedEntity->getName() + "\"", glm::vec4(0.0f));
	this->m_material->setTexture2D("mainTex", 0, VK_SHADER_STAGE_FRAGMENT_BIT, this->m_pTexture);

	vkCmdDrawIndexed(cmdBuffer, m_pMesh->getIndexBuffer()->getIndexCount(), 1, 0, 0, 0);
	this->m_material->endSubPass();
}

LEMaterial* LESpriteRenderer::getDefaultMaterial() const {
	return nullptr;
}

LEAABB* LESpriteRenderer::getAABB() const {
	return nullptr;
}

LEMesh* LESpriteRenderer::getMesh() const {
	return this->m_pMesh;
}
