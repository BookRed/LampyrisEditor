#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Primitive
 * \File:    le_sprite_renderer.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_SPRITE_RENDERER_H
#define LE_SPRITE_RENDERER_H

// LE Includes
#include "../Primitive/le_primitive_renderer.h"

// vulkan Includes
#include <vulkan/vulkan.h>

class LEMesh;
class LEMaterial;
class LETexture2D;

class LESpriteRenderer:public LEPrimitiveRenderer {
	LE_COMPONENT_NAME(LESpriteRenderer);
private:
	LETexture2D*        m_pTexture;
	LEMesh*             m_pMesh;

	void                createMesh();
public:
	                    LESpriteRenderer();
			           
	virtual            ~LESpriteRenderer();

	virtual LEMaterial* getDefaultMaterial();

	void                setSprite(LETexture2D* pSprite);

	LETexture2D*        getSprite() const;

	void                render(VkCommandBuffer cmdBuffer, int materialIndex);

	virtual LEMaterial* getDefaultMaterial() const;

	virtual LEAABB*     getAABB() const;

	virtual LEMesh*     getMesh() const;
};  
#endif // !LE_SPRITE_RENDERER_H