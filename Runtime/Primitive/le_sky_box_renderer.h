#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Primitive
 * \File:    le_sky_box_renderer.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_SKY_BOX_RENDERER
#define LE_SKY_BOX_RENDERER

// LE Includes
#include "le_primitive_renderer.h"
#include "../Resource/le_mesh.h"

class LECamera;
class LECubemap;
class LESkyBoxRenderer:public LEPrimitiveRenderer {
    LE_COMPONENT_NAME(LESkyBoxRenderer);
private:
    LECamera*           m_pCamera;
    LECubemap*          m_pCubeTexture;
    static LEMesh*      ms_pMesh;
public:
                        LESkyBoxRenderer();

    virtual            ~LESkyBoxRenderer();

    virtual void        render(VkCommandBuffer cmdBuffer, int materialIndex);

    virtual LEMaterial* getDefaultMaterial() const;

    virtual LEAABB*     getAABB() const;

    void                setSkyBoxTexture(LECubemap* pCubeTexture);
    
    LECubemap*          getSkyBoxTexture() const;

    void                setCamera(LECamera* pCamera);

    int getRendererType() const override {
        return LE_RENDERER_TYPE_POST_EFFECT;
    }
};  
#endif // !LE_SKY_BOX_RENDERER