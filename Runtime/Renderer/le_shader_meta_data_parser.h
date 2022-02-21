#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_shader_meta_data_parser.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_SHADER_META_DATA_PARSER_H
#define LE_SHADER_META_DATA_PARSER_H	

// LE Includes
#include "../Utility/le_file.h"
#include "../Utility/le_json.h"
#include "../Base/le_singleton.h"
#include "../Resource/le_shader.h"
#include "../Utility/le_external_executor.h"

struct LEPipelineState {
	uint32_t           lod;
	VkCullModeFlagBits cullMode = VK_CULL_MODE_NONE;
	VkFrontFace        frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	bool               ZWriteEnabled = true;
	bool               ZTestEnabled = true;
	bool               ZBiasEnabled = true;
	bool               blendEnabled = true;
	bool               stencilTestEnabled = false;
	bool               stencilWriteEnabled = false;
	VkCompareOp        ZCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	VkCompareOp        stencilCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	VkStencilOp        stencilFailOp = VK_STENCIL_OP_KEEP;
	VkStencilOp        stencilPassOp = VK_STENCIL_OP_KEEP;
	VkBlendOp          alphaBlendOp = VK_BLEND_OP_ADD;
	VkBlendOp          colorBlendOp = VK_BLEND_OP_ADD;
	VkBlendFactor      srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	VkBlendFactor      dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	VkBlendFactor      srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	VkBlendFactor      dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	struct {
		bool red;
		bool green;
		bool blue;
		bool alpha;
	}ColorMask{ true,true,true,true };

	VkColorComponentFlags makeColorWriteMask() const {
		return (ColorMask.red ? VK_COLOR_COMPONENT_R_BIT : 0) |
			(ColorMask.green ? VK_COLOR_COMPONENT_G_BIT : 0) |
			(ColorMask.blue ? VK_COLOR_COMPONENT_B_BIT : 0) |
			(ColorMask.alpha ? VK_COLOR_COMPONENT_A_BIT : 0);
	}
};
class LEShaderMetaDataParser :public Singleton<LEShaderMetaDataParser> {
private:
	LEExternalExecutor m_executor;

	void parserBufferInfo(LEJsonReader* reader, const char* pBufferTypeString,
	                      std::vector<LEShaderBufferInfo>& bufferInfo);

	void parsePushConstant(LEJsonReader* reader, LEShaderMetaData* pMetaData);

	void parseSpecializationConstants(LEJsonReader* reader, LEShaderMetaData* pMetaData);

	void parseTextureCombinedSampler(LEJsonReader* reader, LEShaderMetaData* pMetaData);

	void parseShaderStage(LEJsonReader* reader, LEShaderMetaData* pMetaData);

	void parseInputOutput(LEJsonReader* reader, const char* pTypeString, std::vector<LEShaderIOInfo>& info);

	LEShaderMetaData* parseInternal(LEJsonReader* reader);
public:
	                  LEShaderMetaDataParser();

	virtual          ~LEShaderMetaDataParser();

	LEShaderMetaData* parse(const std::string& filePath);
};

#endif // !LE_SHADER_META_DATA_PARSER_H
