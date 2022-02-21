/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Primitive
 * \File:    le_transform_handle_renderer.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "../Base/le_entity.h"
#include "../Resource/le_mesh.h"
#include "../Math/le_transform.h"
#include "le_transform_handle_renderer.h"
#include "../Renderer/le_render_target.h"
// GLM Includes
#include <glm/glm.hpp>

#include "../Math/le_ray.h"
#include "../Math/le_line.h"

void LETransformHandle::createHandleMesh() {
	// cylinder variables
	const float            cylinderRadius    = 0.5f;
	const float            cylinderHeight    = 10.0f;
	const int              circleSegmentSize = 360;
	const int              cylinderVerticesCount = circleSegmentSize * 2;
	std::vector<glm::vec3> cylinderVertices(cylinderVerticesCount);
	std::vector<int>       cylinderindices(3 * cylinderVerticesCount);

	// cone variables
	const float            coneRadius = 2.0f;
	const float            coneHeight = 3.0f;
	const int              coneVerticesCount = circleSegmentSize + 1;
	std::vector<glm::vec3> coneVertices(coneVerticesCount);
	std::vector<int>       coneindices(3 * circleSegmentSize);

	for (int i = 0; i < circleSegmentSize; i++) {
		// calculate position 
		float x = cos(i * 0.0174532925);
		float z = sin(i * 0.0174532925);

		// cylinder
		// vertices
		cylinderVertices[i] = { x * cylinderRadius,cylinderHeight,z* cylinderRadius };
		cylinderVertices[i + circleSegmentSize] = { x* cylinderRadius,0.0f,z* cylinderRadius };

		// indices
		cylinderindices[i + 0] = i % circleSegmentSize;
		cylinderindices[i + 1] = (i + 1) % circleSegmentSize;
		cylinderindices[i + 2] = circleSegmentSize + i % circleSegmentSize;
		cylinderindices[i + 3] = circleSegmentSize + i % circleSegmentSize;
		cylinderindices[i + 4] = circleSegmentSize + (i + 1) % circleSegmentSize;
		cylinderindices[i + 5] = (i + 1) % circleSegmentSize;

		// cone
		// vertices
		coneVertices[i] = { x * coneRadius,cylinderHeight,z * coneRadius };

		// indices
		coneindices[i + 0] = i % circleSegmentSize;
		coneindices[i + 1] = (i+1) % circleSegmentSize;
		coneindices[i + 2] = circleSegmentSize;
	}
	// the last vertex of cone
	coneVertices[circleSegmentSize] = { 0,cylinderHeight + coneHeight,0 };

	// the position handle consists of a cylinder, and a cone 
	std::vector<glm::vec3> positionHandleVertices(cylinderVerticesCount + coneVerticesCount);
	std::vector<int>       positionHandleindices(3 * (cylinderVerticesCount + circleSegmentSize));

	// copy cylinder's and cone's vertices 
	positionHandleVertices.insert(positionHandleVertices.end(), cylinderVertices.begin(), cylinderVertices.end());
	positionHandleVertices.insert(positionHandleVertices.end(), coneVertices.begin(), coneVertices.end());

	// copy cylinder's indices
	positionHandleindices.insert(positionHandleindices.end(), cylinderindices.begin(), cylinderindices.end());
	// copy cone's indices
	for (int i = 0; i < circleSegmentSize; i++) {
		positionHandleindices[3 * (cylinderVerticesCount + i) + 0] = 3 * cylinderVerticesCount + cylinderindices[3 * i + 0];
		positionHandleindices[3 * (cylinderVerticesCount + i) + 1] = 3 * cylinderVerticesCount + cylinderindices[3 * i + 1];
		positionHandleindices[3 * (cylinderVerticesCount + i) + 2] = 3 * cylinderVerticesCount + cylinderindices[3 * i + 2];
	}
	this->m_pMeshPositionHandle = LEMesh::fromPosition(positionHandleVertices, positionHandleindices);
	// the rotation handle consists of a circle
	this->m_pMeshRotationHandle;
	// the scale handle consists of a cylinder, and a cube
	this->m_pMeshScaleHandle;
}

LETransformHandle::LETransformHandle() :m_viewMode(HVM_3D), m_operation(HO_POSITION),
m_pMeshPositionHandle(nullptr), m_pMeshRotationHandle(nullptr), m_pMeshScaleHandle(nullptr) {
	for (int i = 0; i < 3; i++) {
		m_pPositionHandleRenderer[i] = new LEPositionHandleRenderer;
		m_pRotationHandleRenderer[i] = new LERotationHandleRenderer;
		m_pScaleHandleRenderer[i] = new LEScaleHandleRenderer;
	}
}

LETransformHandle::~LETransformHandle() {
	for (int i = 0; i < 3; i++) {
		LE_SAFE_RELEASE(m_pPositionHandleRenderer[i]);
		LE_SAFE_RELEASE(m_pRotationHandleRenderer[i]);
		LE_SAFE_RELEASE(m_pScaleHandleRenderer[i]);
	}
}

// LE Includes
#include "../Renderer/le_camera.h"

void LEPositionHandleRenderer::render(VkCommandBuffer cmdBuffer, int materialIndex) 
{
}

LEMaterial* LEPositionHandleRenderer::getDefaultMaterial() const {
	return nullptr;
}

LEAABB* LEPositionHandleRenderer::getAABB() const {
	return nullptr;
}

void LEPositionHandleRenderer::updateTransfrom(LECamera* pCamera, glm::vec2 mousePosition) {
	if (!this->m_pAttachedEntity|| !pCamera || !pCamera->getRenderTarget()) {
		return;
	}
	float screenWidth  = pCamera->getRenderTarget()->getWidth();
	float screenHeight = pCamera->getRenderTarget()->getHeight();
	float aspect = screenWidth / screenHeight;
	float nearPlaneWidthHalf  = pCamera->getNear()*glm::tan(pCamera->getFOV()/2.0f);
	float nearPlaneHeightHalf = nearPlaneWidthHalf / aspect;

	glm::vec2 dir = { this->m_orientation.x,this->m_orientation.y };
	dir = glm::normalize(dir);

	glm::vec2 mouseVec = mousePosition - this->m_mouseStartPosition;
	float projectLength = glm::dot(dir, mouseVec);

	glm::vec2 targetPointInScreen = this->m_mouseStartPosition + projectLength * dir;
	// shoot a ray started from targetPointInScreen, denoted as 'ray'
	float xNear = nearPlaneWidthHalf * (targetPointInScreen.x - screenWidth / 2.0f) / (screenWidth / 2.0f);
	float yNear = nearPlaneHeightHalf * (targetPointInScreen.y - screenHeight / 2.0f) / (screenHeight / 2.0f);
	glm::vec4 targetPointInNearPlane        = glm::vec4(xNear, yNear, pCamera->getNear(),1.0f);
	glm::vec4 targetPointInNearPlaneInWorld = glm::inverse(pCamera->getViewMatrix()) * targetPointInNearPlane;
	glm::vec3 cameraPos = pCamera->getAttachedEntity()->getTransform()->getWorldPosition();
	glm::vec3 dirFromCamera = glm::normalize(glm::vec3(targetPointInNearPlaneInWorld) - cameraPos);
	float A1 = dirFromCamera.x;
	float A2 = this->m_orientation.x;
	float x1 = targetPointInNearPlaneInWorld.x;
	float x2 = cameraPos.x;

	float B1 = dirFromCamera.y;
	float B2 = this->m_orientation.y;
	float y1 = targetPointInNearPlaneInWorld.y;
	float y2 = cameraPos.y;

	float C1 = dirFromCamera.z;
	float C2 = this->m_orientation.z;
	float z1 = targetPointInNearPlaneInWorld.z;
	float z2 = cameraPos.z;

	float x = (A1 * x2 + A2 * x1) / (A1 + A2);
	float y = (B1 * y2 + B2 * y1) / (B1 + B2);
	float z = (C1 * z2 + C2 * z1) / (C1 + C2);
}

void LERotationHandleRenderer::render(VkCommandBuffer cmdBuffer, int materialIndex){ 
}

LEMaterial* LERotationHandleRenderer::getDefaultMaterial() const {
	return nullptr;
}

LEAABB* LERotationHandleRenderer::getAABB() const {
	return nullptr;
}

void LEScaleHandleRenderer::render(VkCommandBuffer cmdBuffer, int materialIndex) {
}

LEMaterial* LEScaleHandleRenderer::getDefaultMaterial() const{
	return nullptr;
}

LEAABB* LEScaleHandleRenderer::getAABB() const {
	return nullptr;
}
