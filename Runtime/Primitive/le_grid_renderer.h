#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Primitive
 * \File:    le_grid_renderer.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_GRID_RENDERER_H
#define LE_GRID_RENDERER_H

// LE Includes
#include "le_primitive_renderer.h"

class LEGridRenderer:public LEPrimitiveRenderer {
    LE_COMPONENT_NAME(LEGridRenderer);
public:
	                    LEGridRenderer();

	virtual            ~LEGridRenderer();

    void                render(VkCommandBuffer cmdBuffer, int materialIndex);

    virtual             LEMaterial* getDefaultMaterial() const;
                        
    virtual             LEAABB* getAABB() const;

    int                 getRendererType() const override {
        return LE_RENDERER_TYPE_2D_3D;
    }
};

#endif // !LE_GRID_RENDERER_H