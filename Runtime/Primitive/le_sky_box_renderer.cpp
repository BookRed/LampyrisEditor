/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Primitive
 * \File:    le_sky_box_renderer.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "../Base/le_entity.h"
#include "../Math/le_transform.h"
#include "le_sky_box_renderer.h"
#include "../Resource/le_material.h"
#include "../Renderer/le_vk_debugger.h"
#include "../Renderer/le_camera.h"

LEMesh* LESkyBoxRenderer::ms_pMesh = nullptr;
/*      
             4    FRONT     7
             ______________ 
            / |           /|
           /  |  UP    3 / |
        0 /___|_________/  |
   LEFT   |  5|_________|__|6  RIGHT
          |  /          |  /
          | /   BOTTOM  | /
          |/____________|/        
        1              2   
                BACK
*/
LESkyBoxRenderer::LESkyBoxRenderer() {
    if (!ms_pMesh) {
        ms_pMesh = LEMesh::fromPosition(
            // the diagram describing vertex of skybox cube is shown above  
            // vertices
            std::vector<glm::vec3> {
            {-1, 1, -1}, { -1,-1,-1 }, { 1,-1,-1 }, { 1,1,-1 },
            {-1, 1, 1},  { -1,-1,1 },  { 1,-1,1 },  { 1,1, 1 }},
            // indices
            std::vector<int> {
                // right (+X)
                3,2,6,6,7,3,
                // left  (-X)
                4,5,1,1,0,4,
                // up    (+Y)
                4,0,3,3,7,4,
                // down  (-Y)
                1,5,6,6,2,1,
                // front (+Z)
                7,6,5,5,4,7,
                // back  (-Z)
                0,1,2,2,3,0
          });
    }
}

LESkyBoxRenderer::~LESkyBoxRenderer() {

}

void LESkyBoxRenderer::render(VkCommandBuffer cmdBuffer, int materialIndex) {
    if (!this->m_pCamera) {
        return;
    }
    // prepare material
    glm::vec3 tempPosition = this->m_pCamera->getAttachedEntity()->getTransform()->getLocalPosition();
    this->m_material->beginSubPass(cmdBuffer, 0);
    this->m_pCamera->getAttachedEntity()->getTransform()->setLocalPosition(0);
    this->m_material->pushConstantMatrix("cameraView", 0, VK_SHADER_STAGE_VERTEX_BIT, this->m_pCamera->getViewMatrix());
    // prepare VBO and IBO
    const VkDeviceSize offsetsVertex[1] = { 0 };
    vkCmdBindVertexBuffers(cmdBuffer, 0, 1,
        &(ms_pMesh->getVertexBuffer()->get()->getHandle()),
        offsetsVertex
    );
    vkCmdBindIndexBuffer(cmdBuffer, ms_pMesh->getIndexBuffer()->get()->getHandle(), 0, VK_INDEX_TYPE_UINT32);
    LEVkDebugger::insert(cmdBuffer, "Draw \"" + this->m_pAttachedEntity->getName() + "\"", glm::vec4(0.0f));
    this->m_material->setTextureCube("mainTex", 0, VK_SHADER_STAGE_FRAGMENT_BIT, this->m_pCubeTexture);
    vkCmdDrawIndexed(cmdBuffer, ms_pMesh->getIndexBuffer()->getIndexCount(), 1, 0, 0, 0);
    this->m_material->endSubPass();
    this->m_pCamera->getAttachedEntity()->getTransform()->setLocalPosition(tempPosition.x, tempPosition.y, tempPosition.z);
}

LEMaterial* LESkyBoxRenderer::getDefaultMaterial() const {
    return nullptr;
}

LEAABB* LESkyBoxRenderer::getAABB() const {
    return nullptr;
}

void LESkyBoxRenderer::setSkyBoxTexture(LECubemap* pCubeTexture) {
    this->m_pCubeTexture = pCubeTexture;
}

LECubemap* LESkyBoxRenderer::getSkyBoxTexture() const {
    return this->m_pCubeTexture;
}

void LESkyBoxRenderer::setCamera(LECamera* pCamera) {
    this->m_pCamera = pCamera;
}
