#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_vk_renderer.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_VK_RENDERER_H
#define LE_VK_RENDERER_H

// LE Includes
#include "../Base/le_singleton.h"

class LEVulkanRenderer:public LEObject,public Singleton<LEVulkanRenderer> {
	LE_INITIAL_DECLARE
public:
	            LEVulkanRenderer();

	virtual    ~LEVulkanRenderer();

	static bool initVulkanContext();

	static bool destoryVulkanContext();

	void        update();
};
#endif // !LE_VK_RENDERER_H