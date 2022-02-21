#pragma once
/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Primitive
 * \File:    le_mesh_renderer.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_mesh_renderer.h"
#include "../Base/le_entity.h"
#include "../Resource/le_mesh.h"
#include "../Math/le_transform.h"
#include "../Renderer/le_camera.h"
#include "../Resource/le_material.h"

LEMaterial* LEMeshRenderer::getDefaultMaterial() const {
	static LEMaterial* pMaterial = nullptr;
	if (!pMaterial) {
		pMaterial = new LEMaterial;
		LEShader* vsShaderGrid = LEShaderCompiler::getInstance()->compile("shaders\\le_default.glsl", VK_SHADER_STAGE_VERTEX_BIT);
		LEShader* fsShaderGrid = LEShaderCompiler::getInstance()->compile("shaders\\le_default.glsl", VK_SHADER_STAGE_FRAGMENT_BIT);
		pMaterial->addSubPass(vsShaderGrid, fsShaderGrid);
	}
	return pMaterial;
}

void LEMeshRenderer::setMesh(LEMesh* pMesh) {
	this->m_pMesh = pMesh;
}

LEMesh* LEMeshRenderer::getMesh() const {
	return this->m_pMesh;
}

#if defined(LE_EDITOR)
void LEMeshRenderer::onPropertyEditorGUI() {
	
}
#endif // !LE_EDITOR