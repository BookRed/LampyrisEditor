/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Primitive
 * \File:    le_primitive_renderer.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_primitive_renderer.h"

LEPrimitiveRenderer::LEPrimitiveRenderer():
	m_visible(true),m_sortingLayer(0),m_orderInLayer(0) {}

LEPrimitiveRenderer::~LEPrimitiveRenderer() {};

void LEPrimitiveRenderer::setMaterial(LEMaterial* pMaterial) {
	this->m_material = pMaterial;
}

LEMaterial* LEPrimitiveRenderer::getMaterial() const {
	if(!this->m_material) {
		return this->getDefaultMaterial();
	}
	return this->m_material;
}

void LEPrimitiveRenderer::setVisible(bool visible) {
	this->m_visible = visible;
}

bool LEPrimitiveRenderer::isVisible() const {
	return this->m_visible;
}

int LEPrimitiveRenderer::getSortingLayer() const {
	return this->m_sortingLayer;
}

void LEPrimitiveRenderer::setSortingLayer(int layer) {
	this->m_sortingLayer = layer;
}

int LEPrimitiveRenderer::getOrderInLayer() const {
	return this->m_orderInLayer;
}

void LEPrimitiveRenderer::setOrderInLayer(int order) {
	this->m_orderInLayer = order;
}

bool LEPrimitiveRenderer::isRenderer() const {
	return true;
}

LEMesh* LEPrimitiveRenderer::getMesh() const {
	return nullptr;
}