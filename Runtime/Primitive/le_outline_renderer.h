#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Primitive
 * \File:    le_outline_renderer.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_OUTLINE_RENDERER_H
#define LE_OUTLINE_RENDERER_H

// LE Includes
#include "le_primitive_renderer.h"

// STL Includes
#include <vector>

class LECamera;
// responsible for rendering the object's outline
class LEOutlineRenderer :public LEPrimitiveRenderer {
	typedef std::vector<LEPrimitiveRenderer*> RendererVector;
private:
	glm::vec4              m_outlineColor;
	float                  m_outlineThickness;
	LEPrimitiveRenderer** m_ppRenderers;
	int                    m_pRendererCount;
	LECamera* m_pRenderingCamera;

	void                   render(VkCommandBuffer cmdBuffer, int materialIndex);
public:
	glm::vec4              getOutlineColor() const;

	void                   setOutlineColor(const glm::vec4& color);

	float                  getOutlineThickness() const;

	void                   setOutlineThickness(float thickness);

	virtual LEMaterial*    getDefaultMaterial() const;

	virtual LEAABB*        getAABB() const;

	void                   render(VkCommandBuffer cmdBuffer, LECamera* pCamera, LEPrimitiveRenderer* pRenderer);

	void                   render(VkCommandBuffer cmdBuffer, LECamera* pCamera, RendererVector& pRenderers);
};
#endif // !LE_OUTLINE_RENDERER_H