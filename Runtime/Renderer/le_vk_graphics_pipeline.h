#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_vk_graphics_pipeline.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_VK_GRAPHICS_PIPELINE_H
#define LE_VK_GRAPHICS_PIPELINE_H

// LE Includes
#include "le_vk_buffer.h"
#include "../Base/le_hash.h"
#include "le_shader_compiler.h"
#include "le_vk_variable_manager.h"
#include "../Resource/le_texture.h"

// GLM Includes
#include <glm/glm.hpp>

#include "le_vk_frame_buffer.h"
#include "Runtime/Resource/le_mesh.h"

class LETexture2D;
class LECubemap;
class LEVkRenderPass;
class LERenderTarget;

#define DESCRIPTOR_COUNT 4096

enum LightMode {
	LM_FORWARD,
	LM_DEFERRED,
	LM_SHADOW_CASTER,
	LM_SHADOW_COLLECT,
	LM_ALWAYS,
	LM_MAX_ENUM = 0x7ffffff
};

class LEVkGraphicsPipeline:public LEVulkanObject<VkPipeline> {
private:
	// shader set
	LEShader*                            m_pVShader; // vertex
	LEShader*                            m_pFShader; // fragment
	LEShader*                            m_pGShader; // geometry, not supported yet
	LEShader*                            m_pTShader; // tessellation, not supported yet

	// vulkan handles
	VkDescriptorSetLayout                m_descriptorSetLayout;
	VkPipeline                           m_pipeline;
	VkPipelineCache                      m_pipelineCache;
	VkPipelineLayout                     m_pipelineLayout;

	VkPipelineShaderStageCreateInfo*     m_pShaderState;

	// descriptor-relative variables
	VkDescriptorPool                     m_descriptorPool;
	VkDescriptorSet                      m_inherentUBODescriptor;
	std::vector<VkDescriptorSet>         m_descriptorSets;
	int                                  m_shaderStateCount;
	std::vector<VkWriteDescriptorSet>    m_writeDescriptorSet;
	VkDescriptorBufferInfo*              m_pBufferDesc;
	int                                  m_descriptorSetUseCount{ 0 };

	LEVkRenderPass*                      m_pRenderPass;
	LERenderTarget*                      m_pRenderTarget;
									     
	LEVkBuffer*                          m_pUniformBuffer; 
	LEVkBuffer*                          m_pStorageBuffer; 
									     
	VkCommandBuffer                      m_vkCommandBuffer;

	std::unordered_map<std::string, int> m_writeDescSetIndexMap;

	std::unordered_set<LERenderTarget*>  m_pRecordedRenderTargets;
	// state variables
	LEPipelineState                      m_pipelineState;
	LightMode                            m_lightMode;
	mutable uint32_t                     m_requiredVertexFormatMask{0};

	void setLayoutBindingForBuffer(const std::vector<LEShaderBufferInfo> &uboInfo,
		                           VkDescriptorSetLayoutBinding          *pBindings,
						           int                                   arrayOffset,
		                           VkShaderStageFlagBits                 stage,
		                           VkDescriptorType                      bufferType);

	void setLayoutBindingForTex(const std::vector<LEShaderTextureInfo>       &texInfo,
		                        VkDescriptorSetLayoutBinding                 *pBindings,
						        int                                          arrayOffset,
		                        VkShaderStageFlagBits                        stage);

	void setUniformInternal(const std::string& name, const char* typeName, int descriptorID, VkShaderStageFlagBits stage, void* pData);

public:
	LEVkGraphicsPipeline(LEPipelineState pipelineState = LEPipelineState());

	virtual ~LEVkGraphicsPipeline();

	// no uniform buffer created here, but created in material instances
	// [Step1]: create pipeline layout according to shader meta data. 
	void createDescriptorSetInfo(const std::vector<LEShaderBufferInfo >&uboInfo,
		                         VkDescriptorSetLayoutBinding*                pBindings,
		                         VkDescriptorPoolSize*                        pPoolSize,
		                         int                                          arrayOffset);

	// [Step2]: allocate descriptors set for material instances which use this pipeline.
	void createPipelineLayout();

	// [Step3]: create shader
	void createShader(VkShaderStageFlagBits            shaderStage,
		              VkPipelineShaderStageCreateInfo* pShaderInfo,
		              LEShader*                        pShader);

	void createRenderPass();

	void createFrameBuffer() {
		
	}

	void createDescriptorSet() {
		
	}

	LEVkRenderPass* getRenderPass() const {
		return this->m_pRenderPass;
	}

	void createShader();

	void createPipeline();

	void destroyShader();

	void destroyPipeline();

	void destroyUniformBuffer();

	void destroyPipelineLayout();

	void setVertexShader(LEShader* pShader);

	void setFragmentShader(LEShader* pShader);

	void buildPipeline();

	void pushConstantInternal(const std::string& name, const char* typeName, VkShaderStageFlagBits stage, void* pData);

	bool locateMemberByteCountAndOffset(const std::string& name, const char* typeName, const std::vector<LEShaderMemberInfo>& memberInfo, int& byteCount, int& offset);

	void pushConstantVector4(const std::string& name, VkShaderStageFlagBits stage, glm::vec4 vec);

	void pushConstantMatrix(const std::string& name, VkShaderStageFlagBits stage, glm::mat4 mat);

	void pushConstantUInt(const std::string& name, VkShaderStageFlagBits stage, unsigned int value);

	void setUniformMatrix(const std::string& name, uint32_t descriptorID, VkShaderStageFlagBits stage, glm::mat4 mat);

	void setUniformVector2(const std::string& name, uint32_t descriptorID, VkShaderStageFlagBits stage, glm::vec2 vec);

	void setUniformVector3(const std::string& name, uint32_t descriptorID, VkShaderStageFlagBits stage, glm::vec3 vec);

	void setUniformVector4(const std::string& name, uint32_t descriptorID, VkShaderStageFlagBits stage, glm::vec4 vec);

	void setTexture2D(const std::string& name, uint32_t descriptorID,VkShaderStageFlagBits stage, LETexture2D* pTexture2D);

	void setTextureCube(const std::string& name, uint32_t descriptorID, VkShaderStageFlagBits stage, LECubemap* pTexture2D);

	int requestDescriptorIDForMaterial();

	int calculateBufferByteCount(const std::vector<LEShaderBufferInfo>& uboInfo);

	void updateInherentUBODescriptor();

	uint32_t getInputCount() const;

	void prepareFrameBuffer() {

	}

	static LEVkGraphicsPipeline* createFromShader(LEShader* pVSShader, LEShader* pFSShader);

	// setters and getters
	LightMode getLightMode() const { return this->m_lightMode; }

	void      setLightMode(LightMode lightMode) { this->m_lightMode = lightMode;}

	LEShader* getVSShader() const { return this->m_pVShader; }

	LEShader* getFSShader() const { return this->m_pFShader; }

	LEShader* getGSShader() const { return this->m_pGShader; }

	LEShader* getTSShader() const { return this->m_pTShader; }

	uint32_t  getRequiredVertexFormatMask() const;

	VkPipelineLayout getPipelineLayout() const { return this->m_pipelineLayout; }

	VkDescriptorSet& getDescriptorSet(uint32_t index) {
		return this->m_descriptorSets[index];
	}
};

class LEVkDepthPipeline:public LEVkGraphicsPipeline {
public:
	// using LEVkGraphicsPipeline::LEVkGraphicsPipeline;
	// using LEVkGraphicsPipeline::~LEVkGraphicsPipeline;

};
#endif // !LE_VK_GRAPHICS_PIPELINE_H