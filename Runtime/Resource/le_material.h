#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Resource
 * \File:    le_material.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_MATERIAL_H
#define LE_MATERIAL_H

// LE Includes
#include "le_shader.h"
#include "../Renderer/le_vk_graphics_pipeline.h"

class LETexture2D;
class LECubemap;

struct LEPerPassInfo:public LEObject {
	LE_INITIAL_DECLARE
	LEVkGraphicsPipeline* pipeline;
	// unused when 'vk_ext_push_descriptor' is enabled.
	uint32_t              descriptorID{ 0x7ffffff };
};
class LEMaterial:public LEResource,public LEObject {
	LE_INITIAL_DECLARE
private:
	std::vector<LEPerPassInfo*>        m_passes;
	int                                m_renderQueueValue;
	LEVkGraphicsPipeline*              m_pRenderingPipeline;

	bool                               checkPassIndex(int pass) const;
public:					               
	                                   LEMaterial();
				                       
	virtual                           ~LEMaterial();
				                       
	ResourceType                       getType() const;

	uint32_t                           getDescriptorIndex(uint32_t passIndex) const;

	LEVkGraphicsPipeline*              getGraphicsPipeline(uint32_t passIndex) const;

	bool                               addSubPass(LEShader* pVsShader, LEShader* pFsShader);
				                       
	int                                getPassCount() const;
						               
	LEVkGraphicsPipeline*              getPipeline(int pass = 0) const;
						               
	bool                               setTexture2D(const std::string& name, int pass, VkShaderStageFlagBits state, LETexture2D* pTexture);
						               
	bool                               setTextureCube(const std::string& name, int pass, VkShaderStageFlagBits state, LECubemap* pTexture);
						               
	bool                               setMatrix(const std::string& name, int pass, VkShaderStageFlagBits state, glm::mat4 matrix);
		                               
	bool                               setVector2(const std::string& name, int pass, VkShaderStageFlagBits state, glm::vec2 vec);
						               
	bool                               setVector3(const std::string& name, int pass, VkShaderStageFlagBits state, glm::vec3 vec);
						               
	bool                               setVector4(const std::string& name, int pass, VkShaderStageFlagBits state, glm::vec4 vec);
		          		               
	bool				               pushConstantMatrix(const std::string& name, int pass, VkShaderStageFlagBits state, glm::mat4 matrix);
						               
	bool                               pushConstantVector4(const std::string& name, int pass, VkShaderStageFlagBits state, glm::vec4 vec);
						               
	bool                               pushConstantUInt(const std::string& name, int pass, VkShaderStageFlagBits state, unsigned int value);
						               
	int                                getRenderQueueValue() const;
						               
	void                               setRenderQueueValue(int renderQueue);
						               
	void                               beginSubPass(VkCommandBuffer cmdBuffer, int passID);
						               
	void                               endSubPass();
						               
	uint32_t                           getInputCountAtPass(uint32_t passID);
						               
	void                               getInputVertexOffsetAtPass(uint32_t passID, VkDeviceSize offsets[]);

	void                               onLoaded() override;

	void                               onUnloaded() override;

	static bool                        loadDefaultResource() { return true; };
				                       
	static bool                        destoryDefaultResource() { return true; };
};

#endif // !LE_MATERIAL_H

