#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_inherent_ubo.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_INHERENT_UBO
#define LE_INHERENT_UBO

// LE Includes
#include "le_vk_buffer.h"
#include "../Base/le_singleton.h"

// GLM Includes
#include <glm/glm.hpp>
// Vulkan Includes
#include <unordered_set>
#include <vulkan/vulkan.h>

class LECamera;

struct LECameraUBO {
	glm::vec3 cameraPos;
	char      __pad[4];
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	float     nearValue;
	float     farValue;
};

struct LEMouseUBO {
	glm::vec2 pos;
	glm::vec2 delta;
};

struct LELightUBO {
	glm::vec4  ambient;
	glm::uvec4  count;
	struct {
		glm::vec3  position;
		uint32_t   type; // directional or spot(point light is not supported)

		glm::vec4  color;

		glm::vec3  direction;
		float      constant;

		float      linear;
		float      exp;
		float      spanAngle;
		float      intensity;

		int        lightMode;
		int        shadowMode;
		uint32_t   _pad[2];
	}light[1];
};

struct LEUBOContext {
	VkDescriptorBufferInfo uboInfo;
	VkWriteDescriptorSet   uboWriteDescSet;
};

struct LEInherentUBOContext {
	LEVkBuffer*  pVkBuffer;
	LEUBOContext spatialUBOContext;
	LEUBOContext lightUBOContext;
};

class LECamera;
class LELight;

class LEInherentUBO:public Singleton<LEInherentUBO> {
private:
	constexpr static uint32_t     ms_UBOCount = 3;
	// camera
	LECameraUBO                   m_cameraUBO;
	LEVkBuffer*                   m_cameraUBOBuffer;
	LEUBOContext                  m_cameraUBOContext;

	// light
	std::unordered_set<LELight*>  m_lights;
	LEVkBuffer*                   m_lightUBOBuffer;
	LELightUBO                    m_lightUBO;
	LEUBOContext                  m_lightUBOContext;

	// mouse
	LEMouseUBO                    m_mouseUBO;
	LEVkBuffer*                   m_mouseUBOBuffer;
	LEUBOContext                  m_mouseUBOContext;
public:
	                              LEInherentUBO();
		                         
	virtual                      ~LEInherentUBO();
		                         
	void                          updateRenderingCamera(LECamera* pCamera);
		                          
	void                          updateLightInfo();
		                          
	void                          flushDescriptorSet();

	VkDescriptorBufferInfo&       getCameraUBOInfo();

	void                          updateLight();
		                          
	void                          updateDescriptorSet(VkDescriptorSet descriptorSet);
};
#endif  // !LE_INHERENT_UBO