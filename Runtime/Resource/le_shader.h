#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Resource
 * \File:    le_shader.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_SHADER_H
#define LE_SHADER_H

// LE Includes 
#include "le_resource.h"
#include "../Renderer/le_vk_variable_manager.h"

// STL Includes
#include <unordered_set>
#include <unordered_map>

// input/output info

struct LEShaderIOInfo:public LEObject {
	LE_INITIAL_DECLARE
	int          location;
	std::string  type;
	std::string  name;
};

struct LEShaderMemberInfo:public LEObject {
	LE_INITIAL_DECLARE
	std::string name;
	std::string type;
	int         offset;
	bool        isArray;
	int         arraySize;
	bool        arraySizeLiteral;
};

struct LEShaderPushConstantInfo :public LEObject {
	LE_INITIAL_DECLARE
	int          blockSize;
	std::string  name;
	std::vector<LEShaderMemberInfo> members;
};

struct LEShaderSpecializationConstantInfo :public LEObject {
	LE_INITIAL_DECLARE
	std::string  name;
	std::string  type;
	int          id;
	union {
		bool          boolValue;
		int           intValue;
		unsigned int  uintValue;
		float         floatValue;
		double        doubleValue;
	}defaultValue;
};
struct LEShaderBufferInfo:public LEObject {
	LE_INITIAL_DECLARE
	int          blockSize;
	int          set;
	int          binding;
	std::string  name;
	std::string  type;
	std::vector<LEShaderMemberInfo> members;
};
struct LEShaderTextureInfo :public LEObject {
	LE_INITIAL_DECLARE
	std::string  name;
	std::string  type;
	int          set;
	int          binding;
};
struct LEShaderMetaData :public LEObject {
	LE_INITIAL_DECLARE
	uint32_t                                        shaderStage;
	LEShaderPushConstantInfo                        pushConstantInfo;
	std::vector<LEShaderIOInfo>                     inputInfo;
	std::vector<LEShaderIOInfo>                     outputInfo;
	std::vector<LEShaderBufferInfo>                 uboInfo;
	std::vector<LEShaderBufferInfo>                 ssboInfo;
	std::vector<LEShaderTextureInfo>                textureInfo;
	std::vector<LEShaderSpecializationConstantInfo> specializationConstantInfo;
};

static std::unordered_map<std::string, int> s_glslTypeByteCountMap = {
	{"bool"  ,1 },{"int"   ,4 },{"uint"  ,4 },{"float" ,4 },{"double",8 },
	{"vec2"  ,8 },{"vec3"  ,12},{"vec4"  ,16},
	{"ivec2" ,8 },{"ivec3" ,12},{"ivec4" ,16},
	{"uvec2" ,8 },{"uvec3" ,12},{"uvec4" ,16},
	{"bvec2" ,8 },{"bvec3" ,12},{"bvec4" ,16},
	{"mat2x2",16},{"mat2x3",24},{"mat2x4",32},{"mat2"  ,16},
	{"mat3x2",24},{"mat3x3",36},{"mat3x4",48},{"mat3"  ,36},
	{"mat4x2",32},{"mat4x3",48},{"mat4x4",64},{"mat4"  ,64}
};
static std::unordered_set<std::string> s_glslSamplerTypeSet{
	"sampler1D","sampler2D","sampler3D","samplerCube",
	"sampler2DRect","sampler1DShadow","sampler2DShadow",
	"sampler2DRectShadow ","sampler1DArray","sampler2DArray",
	"sampler1DArrayShadow","sampler2DArrayShadow","samplerBuffer",
	"isampler1D","isampler2D","isampler3D","isamplerCube",
	"isampler2DRect","isampler1DArray","isampler2DArray","isamplerBuffer",
	"usampler1D","usampler2D","usampler3D","usamplerCube",
	"usampler2DRect","usampler1DArray","usampler2DArray","usamplerBuffer"
};
static std::unordered_map<std::string, VkFormat> s_glslTypeFormatMap = {
	{"bool"  ,VK_FORMAT_R8_UINT },
	{"int"   ,VK_FORMAT_R32_SINT },
	{"uint"  ,VK_FORMAT_R32_UINT },
	{"float" ,VK_FORMAT_R32_SFLOAT },
	{"double",VK_FORMAT_R64_SFLOAT },
	{"vec2"  ,VK_FORMAT_R32G32_SFLOAT },
	{"vec3"  ,VK_FORMAT_R32G32B32_SFLOAT },
	{"vec4"  ,VK_FORMAT_R8G8B8A8_UNORM },
	{"ivec2" ,VK_FORMAT_R32G32_SINT },
	{"ivec3" ,VK_FORMAT_R32G32B32_SINT },
	{"ivec4" ,VK_FORMAT_R32G32B32A32_SINT },
	{"uvec2" ,VK_FORMAT_R32G32_SINT },
	{"uvec3" ,VK_FORMAT_R32G32B32_SINT },
	{"uvec4" ,VK_FORMAT_R32G32B32A32_SINT },
	{"bvec2" ,VK_FORMAT_R8G8_UINT },
	{"bvec3" ,VK_FORMAT_R8G8B8_UINT },
	{"bvec4" ,VK_FORMAT_R8G8B8A8_UINT }
	// matrix types will not be regarded as valid vertex input
};

class LEShader:public LEResource,public LEObject {
	LE_INITIAL_DECLARE
private:
	LEShaderMetaData* m_pMetaData;
	VkShaderModule    m_shaderModule;
	std::string       m_compiledCode;

	// called by shader compiler
	void createShaderModule(int codeSize, uint32_t* pCode);
public:
	LEShader():m_pMetaData(nullptr),
		       m_shaderModule(VK_NULL_HANDLE),
		       LEResource(){}

	LEShader(const std::string& filePath):
		m_pMetaData(nullptr),
		m_shaderModule(VK_NULL_HANDLE),LEResource(filePath) {}

	virtual ~LEShader() {

	}

	ResourceType getType() const {
		return LEResource::RT_SHADER;
	}

	const LEShaderMetaData* getMetaData() const {
		return this->m_pMetaData;
	}

	VkShaderModule getShaderModule() const {
		// must be created by LEShaderCompiler
		return this->m_shaderModule;
	}

	void onLoaded() override {
		
	}

	void onUnloaded() override {
		
	}

	friend class LEShaderCompiler;
};
#endif // !LE_SHADER_H