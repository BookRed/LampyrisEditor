/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  LevelManager
 * \File:    le_light.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_level.h"
#include "../Base/le_entity.h"
#include "../Renderer/le_camera.h"
#include "../Primitive/le_primitive_renderer.h"
#include "../Resource/le_material.h"
#include "../Utility/le_random.h"
#include "../Base/le_class_info.h"
#include "../Primitive/Light/le_light.h"
#include "Editor/le_editor_manager.h"
#include "Editor/le_interface_entity_tree_widget.h"
#include "Runtime/Primitive/le_mesh_renderer.h"
#include "Runtime/Resource/importer/le_obj_importer.h"

LE_INITIAL_IMPL_BEGIN(LELevel)
	{
	LE_PROPERTY_REGISTER_METHOD(&LELevel::initProperty);
	LE_PROPERTY_DESTORY_METHOD(&LELevel::terminateProperty);
	LE_CREATE_INSTANCE_METHOD(&LELevel::createInstance);
}
LE_INITIAL_IMPL_END(LELevel)

LE_ADD_PROPERTY_BEGIN(LELevel)
{
	LE_ADD_PROPERTY_ARRAY(LELevel, m_pEntities);
	LE_ADD_PROPERTY_ARRAY(LELevel, m_pLights);
	LE_ADD_PROPERTY_ARRAY(LELevel, m_pCameras);
}
LE_ADD_PROPERTY_END

LEEntity* LELevel::createEntity(const std::string& name) {
	LEEntity* pEntity = new LEEntity(name);
	pEntity->setInstanceID(le_random_int());
	this->m_pEntities.push_back(pEntity);
#if defined(LE_EDITOR)
	// notify entity tree
	LampyrisEditorManager::getInstance()->getEntityTreeWidget().onAddEntity(pEntity);
#endif // !LE_EDITOR
	return pEntity;
}

void LELevel::destoryEntity(LEEntity* pEntity) {
	this->m_pEntities.erase(std::find(this->m_pEntities.begin(), this->m_pEntities.end(), pEntity));
#if defined(LE_EDITOR)
	// notify entity tree
	LampyrisEditorManager::getInstance()->getEntityTreeWidget().onDeleteEntity(pEntity);
#endif // !LE_EDITOR
}

void LELevel::computeVisibleRenderer(LECamera* pCamera) {
	std::vector<LEPrimitiveRenderer*> visibleRenderers;
	// The renderer that meets all the following conditions will enter the rendering queue:
	// 1. the attached entity is active
	// 2. the renderer is enabled
	// 3. the bounding box of renderer is inside the frustum.
	for (auto &pEntity:this->m_pEntities) {
		if (pEntity->isActive()) {
			for (auto& pComponent : pEntity->getAllComponents()) {
				if (pComponent->isRenderer()) {
					LEPrimitiveRenderer* pRenderer = (LEPrimitiveRenderer*)pComponent;
					if (pRenderer->isEnabled() && pRenderer->isVisible()) {
						if (pCamera->testVisiable(pRenderer)) {
							visibleRenderers.push_back(pRenderer);
						}
					}
				}
			}
		}
	}
	// dispatch renderers to different rendering queue.
	for (int i = 0; i < visibleRenderers.size(); i++) {
		LEPrimitiveRenderer* pRenderer = visibleRenderers[i];;
		pCamera->addRenderContext(pRenderer);
	}
}

std::vector<LECamera*>& LELevel::getCameras() {
	return this->m_pCameras;
}

std::vector<LELight*>& LELevel::getLights() {
	return this->m_pLights;
}

void LELevel::addCamera(LECamera* pCamera) {
	if (std::find(this->m_pCameras.begin(), this->m_pCameras.end(), pCamera) != this->m_pCameras.end()) {
		return;
	}
	this->m_pCameras.push_back(pCamera);
}

void LELevel::removeCamera(LECamera* pCamera) {
	auto iter = std::find(this->m_pCameras.begin(), this->m_pCameras.end(), pCamera);
	this->m_pCameras.erase(iter);
}

void LELevel::addLight(LELight* pLight) {
	if (std::find(this->m_pLights.begin(), this->m_pLights.end(), pLight) != this->m_pLights.end()) {
		return;
	}
	this->m_pLights.push_back(pLight);
}

void LELevel::removeLight(LELight* pLight) {
	auto iter = std::find(this->m_pLights.begin(), this->m_pLights.end(), pLight);
	this->m_pLights.erase(iter);
}

#if defined(LE_EDITOR)
const std::vector<LEBuildInPrimitiveInfo>& LELevel::getBuildInPrimitiveInfoList() {
	static std::vector<LEBuildInPrimitiveInfo> info;
	if (info.empty()) {
		// empty
		info.push_back({ "","Empty Entity",LE_PRIMITIVE_ID_EMPTY });
		// other
		info.push_back({ "","Camera",LE_PRIMITIVE_ID_CAMERA });
		// 2d objects
		info.push_back({ "2D Objects","Sprite",LE_PRIMITIVE_ID_SPRITE });
		// 3d objects
		info.push_back({ "3D Objects","Cube",LE_PRIMITIVE_ID_CUBE });
		info.push_back({ "3D Objects","Plane",LE_PRIMITIVE_ID_PLANE });
		info.push_back({ "3D Objects","Sphere",LE_PRIMITIVE_ID_SPHERE });
		info.push_back({ "3D Objects","Cylinder",LE_PRIMITIVE_ID_CYLINDER });
		info.push_back({ "3D Objects","Teapot",LE_PRIMITIVE_ID_TEAPOT });
		// lights
		info.push_back({ "Light","Directional Light",LE_PRIMITIVE_ID_DIRECTIONAL_LIGHT });
		info.push_back({ "Light","Point Light",LE_PRIMITIVE_ID_POINT_LIGHT });
		info.push_back({ "Light","Spot Light",LE_PRIMITIVE_ID_SPOT_LIGHT });
	}
	return info;
}

LEEntity* LELevel::createSandBoxEntity() {
	LEEntity* pEntity = new LEEntity("SandBoxEntity");
	pEntity->setInstanceID(le_random_int());
	return pEntity;
}
#endif // !LE_EDITOR
void LELevel::createPrimitiveEntity(int ID) {
	LEEntity* pEntity = nullptr;
	switch(ID) {
	case LE_PRIMITIVE_ID_EMPTY: {
		pEntity = this->createEntity("Entity");
	}break;
	case LE_PRIMITIVE_ID_SPRITE: {
		pEntity = this->createEntity("Sprite");
	}break;
	case LE_PRIMITIVE_ID_CUBE: {
		pEntity = this->createEntity("Cube");
		LEMeshRenderer* pRenderer = new LEMeshRenderer;
		pEntity->addComponent(pRenderer);
		pRenderer->setMesh(LEMesh::getCubeMesh());
	}break;
	case LE_PRIMITIVE_ID_PLANE: {
		pEntity = this->createEntity("Plane");
		LEMeshRenderer* pRenderer = new LEMeshRenderer;
		pEntity->addComponent(pRenderer);
		pRenderer->setMesh(LEMesh::getPlaneMesh());
	}break;
	case LE_PRIMITIVE_ID_SPHERE: {
		pEntity = this->createEntity("Sphere");
		LEMeshRenderer* pRenderer = new LEMeshRenderer;
		pEntity->addComponent(pRenderer);
		pRenderer->setMesh(LEMesh::getSphereMesh());
	}break;
	case LE_PRIMITIVE_ID_CYLINDER: {
		pEntity = this->createEntity("Cylinder");
	}break;
	case LE_PRIMITIVE_ID_TEAPOT: {
		pEntity = this->createEntity("Teapot");
		LEMeshRenderer* pRenderer = new LEMeshRenderer;
		LEObjImporter importer = LEObjImporter("testData\\teapot.obj");
		importer.loadModelData();
		LEMesh* pmesh = importer.registerAsset();
		pRenderer->setMesh(pmesh);
		pEntity->addComponent(pRenderer);
	}break;
	case LE_PRIMITIVE_ID_DIRECTIONAL_LIGHT: {
		pEntity = this->createEntity("Directional Light");
		LELight* pLight = new LELight;
		pLight->setLightMode(LELight::LM_RealTime);
		pLight->setLightType(LELight::Directional);
		pEntity->addComponent(pLight);

	}break;
	case LE_PRIMITIVE_ID_POINT_LIGHT: {
		pEntity = this->createEntity("Point Light");
	}break;
	case LE_PRIMITIVE_ID_SPOT_LIGHT: {
		pEntity = this->createEntity("Spot Light");
	}break;
	case LE_PRIMITIVE_ID_CAMERA: {
		pEntity = this->createEntity("Camera");
		LECamera* pCamera = new LECamera;
		pEntity->addComponent(pCamera);
	}break;
	}
}
