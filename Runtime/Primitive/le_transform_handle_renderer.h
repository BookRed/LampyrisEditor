#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Primitive
 * \File:    le_transform_handle_renderer.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// Reference: Currently popular game engine's editor

#ifndef LE_TRANSFORM_HANDLE_RENDERER
#define LE_TRANSFORM_HANDLE_RENDERER

// LE Includes
#include "le_primitive_renderer.h"


enum LEHandleViewMode {
	HVM_2D = 0,
	HVM_3D = 1,
	HVM_MAX_ENUM = 0x7fffffff
};

enum LEHandleOpertaion {
	HO_POSITION = 0,
	HO_ROTATION = 1,
	HO_SCALE    = 2,
	HO_MAX_ENUM = 0x7fffffff
};

class LEMesh;
class LECamera;
class LEPositionHandleRenderer;
class LERotationHandleRenderer;
class LEScaleHandleRenderer;

class LETransformHandle {
private:
	LEHandleViewMode          m_viewMode;
	LEHandleOpertaion         m_operation;
	LEMesh*                   m_pMeshPositionHandle;
	LEMesh*                   m_pMeshRotationHandle;
	LEMesh*                   m_pMeshScaleHandle;
					          
	LEPositionHandleRenderer* m_pPositionHandleRenderer[3];
	LERotationHandleRenderer* m_pRotationHandleRenderer[3];
	LEScaleHandleRenderer*    m_pScaleHandleRenderer[3];
	void createHandleMesh();
public:
	LETransformHandle();

	virtual ~LETransformHandle();
};

class LEPositionHandleRenderer:public LEPrimitiveRenderer {
private:
	glm::vec3           m_orientation;
	glm::vec2           m_mouseStartPosition;
public:
	void                render(VkCommandBuffer cmdBuffer, int materialIndex);

	virtual LEMaterial* getDefaultMaterial() const;

	virtual LEAABB*     getAABB() const;

	void updateTransfrom(LECamera* pCamera, glm::vec2 mousePosition);

	int getRendererType() const override {
		return LE_RENDERER_TYPE_OVERLAY;
	}
};

class LERotationHandleRenderer :public LEPrimitiveRenderer {
public:
	void                   render(VkCommandBuffer cmdBuffer, int materialIndex);

	virtual LEMaterial* getDefaultMaterial() const;

	virtual LEAABB* getAABB() const;

	int getRendererType() const override {
		return LE_RENDERER_TYPE_OVERLAY;
	}
};

class LEScaleHandleRenderer :public LEPrimitiveRenderer {
public:
	void                render(VkCommandBuffer cmdBuffer, int materialIndex);

	virtual LEMaterial* getDefaultMaterial() const;

	virtual LEAABB*     getAABB() const;

	int getRendererType() const override {
		return LE_RENDERER_TYPE_OVERLAY;
	}
};
#endif // !LE_TRANSFORM_HANDLE_RENDERER
