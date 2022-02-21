/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_render_queue.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_render_queue.h"
#include "../Resource/le_material.h"
#include "../Primitive/le_primitive_renderer.h"

void LERenderQueue::sort(std::vector<int>& indicesVec) {
	std::sort(indicesVec.begin(), indicesVec.end(), [=](const int& lhs, const int& rhs) {
		const LERenderContext& lhsContext = this->m_renderContext[lhs];
		const LERenderContext& rhsContext = this->m_renderContext[rhs];
		LEMaterial* lhspMaterial = lhsContext.pRenderer->getMaterial();
		LEMaterial* rhspMaterial = rhsContext.pRenderer->getMaterial();
		int lhsSortingLayer = lhsContext.pRenderer->getSortingLayer();
		int rhsSortingLayer = rhsContext.pRenderer->getSortingLayer();
		int lhsOrderInLayer = lhsContext.pRenderer->getOrderInLayer();
		int rhsOrderInLayer = rhsContext.pRenderer->getOrderInLayer();
		int lhsRenderQueueValue = lhspMaterial->getRenderQueueValue();
		int rhsRenderQueueValue = rhspMaterial->getRenderQueueValue();
		float lhsdistanceToCamera = lhsContext.distanceToCamera;
		float rhsdistanceToCamera = rhsContext.distanceToCamera;

		if (lhsSortingLayer < lhsSortingLayer) {
			return 1;
		}
		else {
			if (lhsOrderInLayer < rhsOrderInLayer) {
				return 1;
			}
			else {
				if (lhsRenderQueueValue < rhsRenderQueueValue) {
					return 1;
				}
				else {
					return (int)(lhsdistanceToCamera < rhsdistanceToCamera);
				}
			}
		}
		});
}

void LERenderQueue::addRenderContext(LEPrimitiveRenderer* pRenderer) {
	this->m_renderContext.emplace_back(pRenderer); 
}

void LERenderQueue::sort() {
	std::map<int, std::vector<int>> opaqueQueueindices;
	std::map<int, std::vector<int>> transparentQueueindices;
	// 2500 is the key value to differentiate opaque object and transparent object
	for (int i = 0; i<this->m_renderContext.size(); i++) {
		auto& renderContext = this->m_renderContext[i];
		LEMaterial* pMaterial = renderContext.pRenderer->getMaterial();
		if (pMaterial->getRenderQueueValue() > 2500) {
			transparentQueueindices[pMaterial->getRenderQueueValue()].push_back(i);
		}
		else {
			opaqueQueueindices[pMaterial->getRenderQueueValue()].push_back(i);
		}
	}
	std::vector<LERenderContext> tempRenderContext;
	int curIndex = 0;
	tempRenderContext.resize(this->m_renderContext.size());
	for (auto& [_, indicesVec] : opaqueQueueindices) {
		this->sort(indicesVec);
		for (auto& index : indicesVec) {
			tempRenderContext[curIndex++] = this->m_renderContext[index];
		}
	}
	for (auto& [_, indicesVec] : transparentQueueindices) {
		this->sort(indicesVec);
		for (auto& index : indicesVec) {
			tempRenderContext[curIndex++] = this->m_renderContext[index];
		}
	}
	this->m_renderContext.swap(tempRenderContext);
}

void LERenderQueue::reset() {
	this->m_renderContext.clear();
}

const std::vector<LERenderContext>& LERenderQueue::getRenderContext() {
	return this->m_renderContext;
}
