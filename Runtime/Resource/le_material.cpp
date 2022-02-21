/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Resource
 * \File:    le_material.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_material.h"
#include "../Base/le_class_info.h"

LE_INITIAL_IMPL_BEGIN(LEMaterial)
{
	LE_INITIAL_METHOD(&LEMaterial::loadDefaultResource);
	LE_TERMINATE_METHOD(&LEMaterial::destoryDefaultResource);
	LE_PROPERTY_REGISTER_METHOD(&LEMaterial::initProperty);
	LE_PROPERTY_DESTORY_METHOD(&LEMaterial::terminateProperty);
	LE_CREATE_INSTANCE_METHOD(&LEMaterial::createInstance);
}
LE_INITIAL_IMPL_END(LEMaterial)

LE_ADD_PROPERTY_BEGIN(LEMaterial)
LE_ADD_PROPERTY_END

LE_INITIAL_IMPL_BEGIN(LEPerPassInfo)
{

}
LE_INITIAL_IMPL_END(LEPerPassInfo)

LE_ADD_PROPERTY_BEGIN(LEPerPassInfo)
LE_ADD_PROPERTY_END
bool LEMaterial::checkPassIndex(int pass) const {
	return !(this->m_passes.empty() || pass < 0 || pass >= this->m_passes.size());
}

LEMaterial::LEMaterial() {

}

LEMaterial::~LEMaterial() {

}

LEMaterial::ResourceType LEMaterial::getType() const {
	return LEResource::RT_MATERIAL;
}

uint32_t LEMaterial::getDescriptorIndex(uint32_t passIndex) const {
	return this->m_passes[passIndex]->descriptorID;
}

LEVkGraphicsPipeline* LEMaterial::getGraphicsPipeline(uint32_t passIndex) const {
	return this->m_passes[passIndex]->pipeline;
}

bool LEMaterial::addSubPass(LEShader* pVsShader, LEShader* pFsShader) {
	LEVkGraphicsPipeline* pPipeline = LEVkGraphicsPipeline::createFromShader(pVsShader, pFsShader);
	uint32_t descriptorID = pPipeline->requestDescriptorIDForMaterial();
	LEPerPassInfo* pPassInfo = new LEPerPassInfo;
	pPassInfo->pipeline = pPipeline;
	pPassInfo->descriptorID = descriptorID;
	this->m_passes.push_back(pPassInfo);
	return true;
}

int LEMaterial::getPassCount() const {
	return this->m_passes.size();
}

LEVkGraphicsPipeline* LEMaterial::getPipeline(int pass) const {
	if (!this->checkPassIndex(pass)) {
		return nullptr;
	}
	return this->m_passes[pass]->pipeline;
}

bool LEMaterial::setTexture2D(const std::string& name, int pass, VkShaderStageFlagBits state,LETexture2D* pTexture) {
	if (!this->checkPassIndex(pass)) {
		return false;
	}
	this->m_passes[pass]->pipeline->setTexture2D(name, this->m_passes[pass]->descriptorID,state,pTexture);
	return true;
}

bool LEMaterial::setTextureCube(const std::string& name, int pass, VkShaderStageFlagBits state, LECubemap* pTexture) {
	if (!this->checkPassIndex(pass)) {
		return false;
	}
	this->m_passes[pass]->pipeline->setTextureCube(name, this->m_passes[pass]->descriptorID,state, pTexture);
	return true;
}

bool LEMaterial::setMatrix(const std::string& name, int pass, VkShaderStageFlagBits state, glm::mat4 matrix) {
	if (!this->checkPassIndex(pass)) {
		return false;
	}
}

bool LEMaterial::setVector2(const std::string& name, int pass, VkShaderStageFlagBits state, glm::vec2 vec) {
	if (!this->checkPassIndex(pass)) {
		return false;
	}
	this->m_passes[pass]->pipeline->setUniformVector2(name, this->m_passes[pass]->descriptorID,state, vec);
	return true;
}

bool LEMaterial::setVector3(const std::string& name, int pass, VkShaderStageFlagBits state, glm::vec3 vec) {
	if (!this->checkPassIndex(pass)) {
		return false;
	}
	this->m_passes[pass]->pipeline->setUniformVector3(name, this->m_passes[pass]->descriptorID, state, vec);
	return true;
}
bool LEMaterial::setVector4(const std::string& name, int pass, VkShaderStageFlagBits state, glm::vec4 vec) {
	if (!this->checkPassIndex(pass)) {
		return false;
	}
	this->m_passes[pass]->pipeline->setUniformVector4(name, this->m_passes[pass]->descriptorID, state, vec);
	return true;
}

bool LEMaterial::pushConstantMatrix(const std::string& name, int pass, VkShaderStageFlagBits state, glm::mat4 matrix) {
	this->m_passes[pass]->pipeline->pushConstantMatrix(name, state, matrix);
	return true;
}

bool LEMaterial::pushConstantUInt(const std::string& name, int pass, VkShaderStageFlagBits state, unsigned int value) {
	this->m_passes[pass]->pipeline->pushConstantUInt(name, state, value);
	return true;
}

bool LEMaterial::pushConstantVector4(const std::string& name, int pass, VkShaderStageFlagBits state, glm::vec4 vec) {
	this->m_passes[pass]->pipeline->pushConstantVector4(name, state, vec);
	return true;
}

int LEMaterial::getRenderQueueValue() const {
	return this->m_renderQueueValue;
}

void LEMaterial::setRenderQueueValue(int renderQueue) {
	m_renderQueueValue = renderQueue;
}

void LEMaterial::beginSubPass(VkCommandBuffer cmdBuffer, int passID) {
	LE_ASSERT(checkPassIndex(passID));
	this->m_pRenderingPipeline = this->m_passes[passID]->pipeline;
	this->m_pRenderingPipeline->buildPipeline();
	this->m_pRenderingPipeline->updateInherentUBODescriptor();
}

void LEMaterial::endSubPass() {
	this->m_pRenderingPipeline = nullptr;
}

uint32_t LEMaterial::getInputCountAtPass(uint32_t passID) {
	return this->m_passes[passID]->pipeline->getInputCount();
}

void LEMaterial::getInputVertexOffsetAtPass(uint32_t passID, VkDeviceSize offsets[]) {
	VkDeviceSize offsetTotal = 0;
	auto& inputInfo = this->m_passes[passID]->pipeline->getVSShader()->getMetaData()->inputInfo;
	for (uint32_t i = 0; i < inputInfo.size();i++) {
		offsets[i] = offsetTotal;
		offsetTotal += s_glslTypeByteCountMap[inputInfo[i].type];
	}
}

void LEMaterial::onLoaded() {
}

void LEMaterial::onUnloaded() {
}
