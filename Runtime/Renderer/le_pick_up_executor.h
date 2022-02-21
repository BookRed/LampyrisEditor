#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_pick_up_executor.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_PICK_UP_EXECUTOR_H
#define LE_PICK_UP_EXECUTOR_H

// LE Includes
#include "le_render_queue.h"
#include "../Base/le_singleton.h"

// STL Includes
#include <unordered_map>
#include <glm/vec2.hpp>

class LERenderTarget;
class LEMaterial;
class LEVkBuffer;
class LEPrimitiveRenderer;
class LERenderTargetTexture;

class LEPickUpExecutor:public Singleton<LEPickUpExecutor> {
	typedef std::unordered_map<unsigned int, LEPrimitiveRenderer*> LERendererIDMap_t;
private:
	LEMaterial*             m_pickUpMaterial{nullptr};
	LERenderTargetTexture*  m_pInstanceIDTexture{nullptr};
	LERendererIDMap_t       m_instanceIDMap;
	LEVkBuffer*             m_pVkBuffer{nullptr};
public:					   
	                        LEPickUpExecutor();

	virtual                ~LEPickUpExecutor();
						   
	LEPrimitiveRenderer*    execute(const std::vector<LERenderContext>& renderContext, glm::vec2& mousePos);

	void                    create(LERenderTarget* pTarget);
};
#endif // !LE_PICK_UP_EXECUTOR_H