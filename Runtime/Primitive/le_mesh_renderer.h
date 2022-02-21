#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Primitive
 * \File:    le_mesh_renderer.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_MESH_RENDERER_H
#define LE_MESH_RENDERER_H

// LE Includes
#include "le_primitive_renderer.h"

// Vulkan Includes
#include <vulkan/vulkan.h>

class LEMesh;
class LECamera;
class LEVkCommandBuffer;

// LE Includes
class LEMeshRenderer:public LEPrimitiveRenderer {
	LE_COMPONENT_NAME(LEMeshRenderer);
private:
	LEMesh* m_pMesh {nullptr};
public:
	void render(LEVkCommandBuffer cmdBuffer, int materialIndex);

	LEMaterial* getDefaultMaterial() const override;

	virtual LEAABB* getAABB() const {
		return this->m_pMesh->getAABB();
	}

	void setMesh(LEMesh* pMesh);

	virtual LEMesh* getMesh() const;

	int getRendererType() const override {
		return LE_RENDERER_TYPE_2D_3D;
	}
#if defined(LE_EDITOR)
	void onPropertyEditorGUI() override;
#endif // !LE_EDITOR
};
#endif // !LE_MESH_RENDERER_H