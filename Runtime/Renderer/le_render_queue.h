#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_render_queue.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_RENDER_QUEUE_H
#define LE_RENDER_QUEUE_H

// STL Includes
#include <vector>
#include <map>
#include <algorithm>

class LEPrimitiveRenderer;
enum RenderQueueType {
	RQT_BACKGROUND = 1000,
	RQT_OPAQUE     = 2000,
	RQT_TRANSPARENT= 3000,
	RQT_OVERLAY    = 4000,
	RQT_MAX        = 0x7fffffff
};

struct LERenderContext {
	LEPrimitiveRenderer* pRenderer;
	float                distanceToCamera;
};
class LERenderQueue {
private:
	std::vector<LERenderContext> m_renderContext;

	void sort(std::vector<int>& indicesVec); 
public:

	void addRenderContext(LEPrimitiveRenderer* pRenderer);

	void sort();

	void reset();

	const std::vector<LERenderContext>& getRenderContext();
};
#endif // !LE_RENDER_QUEUE_H