#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Primitive
 * \File:    le_primitive_renderer.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// Vulkan Includes
#include <vulkan/vulkan.h>

// LE Includes
#include "../Renderer/le_vk_variable_manager.h"
#include "../Base/le_singleton.h"
#include "../Base/le_component.h"
#include "../Resource/le_mesh.h"


class LEAABB;
class LEMesh;
class LEMaterial;

#define LE_RENDERER_TYPE_2D_3D        1
#define LE_RENDERER_TYPE_POST_EFFECT  2
#define LE_RENDERER_TYPE_SKY_BOX      3
#define LE_RENDERER_TYPE_OVERLAY      4

class LEPrimitiveRenderer:public LEComponent {
	LE_COMPONENT_NAME(LEPrimitiveRenderer);
protected:
	int                             m_rendererType;
	LEMaterial*                     m_material { nullptr};
	bool                            m_visible;
	int                             m_sortingLayer;
	int                             m_orderInLayer;

	VkBuffer                        m_pBuffers[16];
	VkDeviceSize                    m_VBOOffsets[16];
public:
                                    LEPrimitiveRenderer();

	virtual                        ~LEPrimitiveRenderer();

	void                            setMaterial(LEMaterial* pMaterial);

	LEMaterial*                     getMaterial() const;

	void                            setVisible(bool visible);
		                            
	bool                            isVisible() const;
		                            
	void                            setOrderInLayer(int order);
		                            
	void                            setSortingLayer(int layer);
		                            
	int                             getSortingLayer() const;
				                    
	int                             getOrderInLayer() const;
				                    
	virtual bool                    isRenderer() const;

	virtual LEMaterial*             getDefaultMaterial() const = 0;

	virtual LEAABB*                 getAABB() const = 0;

	virtual LEMesh*                 getMesh() const;

	virtual int                     getRendererType() const = 0;
};
