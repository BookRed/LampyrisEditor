/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_inherent_ubo.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
 * 
*/
// LE Includes
#include "le_camera.h"
#include "le_inherent_ubo.h"
#include "../Base/le_entity.h"
#include "../Math/le_transform.h"
#include "../Primitive/Light/le_light.h"
#include "../LevelManage/le_level.h"
#include "../LevelManage/le_world.h"

LEInherentUBO::LEInherentUBO() {
	int bufferUsageBits = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	// init camera ubo
	this->m_cameraUBOBuffer = new LEVkBuffer(sizeof(LECameraUBO), bufferUsageBits);
	this->m_cameraUBOContext.uboInfo.buffer = this->m_cameraUBOBuffer->getHandle();
	this->m_cameraUBOContext.uboInfo.offset = 0;
	this->m_cameraUBOContext.uboInfo.range  = this->m_cameraUBOBuffer->getSize();

	this->m_cameraUBOContext.uboWriteDescSet = {};
	this->m_cameraUBOContext.uboWriteDescSet.sType           = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	this->m_cameraUBOContext.uboWriteDescSet.pNext           = nullptr;
	this->m_cameraUBOContext.uboWriteDescSet.dstBinding      = 0;
	this->m_cameraUBOContext.uboWriteDescSet.descriptorCount = 1;
	this->m_cameraUBOContext.uboWriteDescSet.pBufferInfo     = &(this->m_cameraUBOContext.uboInfo);
	this->m_cameraUBOContext.uboWriteDescSet.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	// init light ubo
	this->m_lightUBOBuffer = new LEVkBuffer(sizeof(LELightUBO) + sizeof(uint32_t), bufferUsageBits);
	this->m_lightUBOContext.uboInfo.buffer = this->m_lightUBOBuffer->getHandle();
	this->m_lightUBOContext.uboInfo.offset = 0;
	this->m_lightUBOContext.uboInfo.range = this->m_lightUBOBuffer->getSize();

	this->m_lightUBOContext.uboWriteDescSet = {};
	this->m_lightUBOContext.uboWriteDescSet.sType           = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	this->m_lightUBOContext.uboWriteDescSet.pNext           = nullptr;
	this->m_lightUBOContext.uboWriteDescSet.dstBinding      = 1;
	this->m_lightUBOContext.uboWriteDescSet.descriptorCount = 1;
	this->m_lightUBOContext.uboWriteDescSet.pBufferInfo     = &(this->m_lightUBOContext.uboInfo);
	this->m_lightUBOContext.uboWriteDescSet.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	// init mouse ubo
	this->m_mouseUBOBuffer = new LEVkBuffer(sizeof(LEMouseUBO), bufferUsageBits);
	this->m_mouseUBOContext.uboInfo.buffer = this->m_mouseUBOBuffer->getHandle();
	this->m_mouseUBOContext.uboInfo.offset = 0;
	this->m_mouseUBOContext.uboInfo.range = this->m_mouseUBOBuffer->getSize();
	this->m_mouseUBOContext.uboWriteDescSet = {};
	this->m_mouseUBOContext.uboWriteDescSet.sType           = VkStructureType::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	this->m_mouseUBOContext.uboWriteDescSet.pNext           = nullptr;
	this->m_mouseUBOContext.uboWriteDescSet.dstBinding      = 2;
	this->m_mouseUBOContext.uboWriteDescSet.descriptorCount = 1;
	this->m_mouseUBOContext.uboWriteDescSet.pBufferInfo     = &(this->m_mouseUBOContext.uboInfo);
	this->m_mouseUBOContext.uboWriteDescSet.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
}

LEInherentUBO::~LEInherentUBO() {
	// destroy camera ubo
	LE_SAFE_RELEASE(this->m_cameraUBOBuffer);
	// destroy light ubo
	LE_SAFE_RELEASE(this->m_lightUBOBuffer);
	// destroy mouse ubo
	LE_SAFE_RELEASE(this->m_mouseUBOBuffer);
}

void LEInherentUBO::updateRenderingCamera(LECamera* pCamera) {
	this->m_cameraUBOContext.uboInfo.buffer                  = this->m_cameraUBOBuffer->getHandle();
	this->m_cameraUBOContext.uboInfo.offset                  = 0;
	this->m_cameraUBOContext.uboInfo.range                   = sizeof(LECameraUBO);


	LECameraUBO cameraUBO;
	cameraUBO.cameraPos        = glm::vec4(pCamera->getAttachedEntity()->getTransform()->getWorldPosition(), 1.0f);
	cameraUBO.projectionMatrix = pCamera->getProjectionMatrix();
	cameraUBO.viewMatrix       = pCamera->getViewMatrix();
	cameraUBO.nearValue        = pCamera->getNear();
	cameraUBO.farValue         = pCamera->getFar();

	this->m_cameraUBOBuffer->map();
	this->m_cameraUBOBuffer->writeByte((unsigned char*)&cameraUBO, sizeof(LECameraUBO), 0);
	this->m_cameraUBOBuffer->unmap();
}

void LEInherentUBO::updateLightInfo() {
	uint32_t index = 0;
	for(LELight* pLight:this->m_lights) {
		if(!pLight->isEnabled() || !pLight->getAttachedEntity() || pLight->getAttachedEntity()->isActive()) {
			continue;
		}
		index++;
	}
	this->m_lightUBOBuffer->map();
	// this->m_lightUBOBuffer->writeByte(reinterpret_cast<unsigned char*>(&this->m_lightUBO.light), index*sizeof(LELightUBO::light), 0);
	this->m_lightUBOBuffer->unmap();
}

void LEInherentUBO::flushDescriptorSet() {
	
}

VkDescriptorBufferInfo& LEInherentUBO::getCameraUBOInfo() {
	return this->m_cameraUBOContext.uboInfo;
}

void LEInherentUBO::updateLight() {
	auto lights = LEWorld::getInstance()->getActiveLevel()->getLights();
	this->m_lightUBOBuffer->map();
	this->m_lightUBOBuffer->write(LELight::getAmbient(), 0);
	this->m_lightUBOBuffer->write(lights.size(), 16);
	uint32_t index = 0;
	uint32_t byteCount = sizeof(LELightUBO::light[0]);

	for(LELight* pLight: lights) {
		if(!pLight->isEnabled() || !pLight->getAttachedEntity() || !pLight->getAttachedEntity()->isActive()) {
			continue;
		}
		this->m_lightUBO.light[index].type       = pLight->getLightType();
		this->m_lightUBO.light[index].color      = pLight->getColor();
		this->m_lightUBO.light[index].intensity  = pLight->getIlluminance();
		this->m_lightUBO.light[index].direction  = pLight->getAttachedEntity()->getTransform()->getForwardDirection();
		this->m_lightUBO.light[index].position   = pLight->getAttachedEntity()->getTransform()->getWorldPosition();
		this->m_lightUBO.light[index].spanAngle  = pLight->getSpanAngle();
		this->m_lightUBO.light[index].lightMode  = static_cast<int>(pLight->getLightMode());
		this->m_lightUBO.light[index].shadowMode = static_cast<int>(pLight->getShadowMode());

		this->m_lightUBOBuffer->writeByte(reinterpret_cast<unsigned char*>(&(this->m_lightUBO.light[index])), byteCount, 32+index*byteCount);

		index++;
	}
	this->m_lightUBOBuffer->unmap();
}

void LEInherentUBO::updateDescriptorSet(VkDescriptorSet descriptorSet) {
	std::array<VkWriteDescriptorSet, 2*ms_UBOCount> array = { this->m_cameraUBOContext.uboWriteDescSet,
		                                                      this->m_lightUBOContext.uboWriteDescSet,
		                                                      this->m_mouseUBOContext.uboWriteDescSet,
		                                                      this->m_cameraUBOContext.uboWriteDescSet,
		                                                      this->m_lightUBOContext.uboWriteDescSet,
		                                                      this->m_mouseUBOContext.uboWriteDescSet};
	array[0].dstSet = descriptorSet;
	array[1].dstSet = descriptorSet;
	array[2].dstSet = descriptorSet;
	array[3].dstSet = descriptorSet;
	array[4].dstSet = descriptorSet;
	array[5].dstSet = descriptorSet;

	array[0].dstBinding = 0;
	array[1].dstBinding = 1;
	array[2].dstBinding = 2;
	array[3].dstBinding = 16;
	array[4].dstBinding = 17;
	array[5].dstBinding = 18;
	LEVkDevice::updateDescriptorSets(2* ms_UBOCount, array.data());
}
