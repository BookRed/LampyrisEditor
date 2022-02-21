/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Resource
 * \File:    le_shader.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_shader.h"
#include "../Base/le_initial.h"
#include "../Base/le_class_info.h"
#include "../Renderer/le_vk_device.h"

// LEShaderIOInfo
LE_INITIAL_IMPL_BEGIN(LEShaderIOInfo)
{
	LE_PROPERTY_REGISTER_METHOD(&LEShaderIOInfo::initProperty);
	LE_PROPERTY_DESTORY_METHOD(&LEShaderIOInfo::terminateProperty);
}
LE_INITIAL_IMPL_END(LEShaderIOInfo)

LE_ADD_PROPERTY_BEGIN(LEShaderIOInfo)
LE_ADD_PROPERTY_RANGE_NOUSE(LEShaderIOInfo, location);
LE_ADD_PROPERTY_STRING(LEShaderIOInfo, type);
LE_ADD_PROPERTY_STRING(LEShaderIOInfo, name);
LE_ADD_PROPERTY_END

// LEShaderMemberInfo
LE_INITIAL_IMPL_BEGIN(LEShaderMemberInfo)
{
	LE_PROPERTY_REGISTER_METHOD(&LEShaderMemberInfo::initProperty);
	LE_PROPERTY_DESTORY_METHOD(&LEShaderMemberInfo::terminateProperty);
}
LE_INITIAL_IMPL_END(LEShaderMemberInfo)

LE_ADD_PROPERTY_BEGIN(LEShaderMemberInfo)
LE_ADD_PROPERTY_STRING(LEShaderMemberInfo, name);
LE_ADD_PROPERTY_STRING(LEShaderMemberInfo, type);
LE_ADD_PROPERTY_RANGE_NOUSE(LEShaderMemberInfo, offset);
LE_ADD_PROPERTY_RANGE_NOUSE(LEShaderMemberInfo, arraySize);
LE_ADD_PROPERTY_BOOL(LEShaderMemberInfo, isArray);
LE_ADD_PROPERTY_BOOL(LEShaderMemberInfo, arraySizeLiteral);
LE_ADD_PROPERTY_END

// LEShaderPushConstantInfo
LE_INITIAL_IMPL_BEGIN(LEShaderPushConstantInfo)
{
	LE_PROPERTY_REGISTER_METHOD(&LEShaderPushConstantInfo::initProperty);
	LE_PROPERTY_DESTORY_METHOD(&LEShaderPushConstantInfo::terminateProperty);
}
LE_INITIAL_IMPL_END(LEShaderPushConstantInfo)

LE_ADD_PROPERTY_BEGIN(LEShaderPushConstantInfo)
LE_ADD_PROPERTY_RANGE_NOUSE(LEShaderPushConstantInfo, blockSize);
LE_ADD_PROPERTY_STRING(LEShaderPushConstantInfo, name);
LE_ADD_PROPERTY_ARRAY(LEShaderPushConstantInfo, members);
LE_ADD_PROPERTY_END

// LEShaderSpecializationConstantInfo
LE_INITIAL_IMPL_BEGIN(LEShaderSpecializationConstantInfo)
{
	LE_PROPERTY_REGISTER_METHOD(&LEShaderSpecializationConstantInfo::initProperty);
	LE_PROPERTY_DESTORY_METHOD(&LEShaderSpecializationConstantInfo::terminateProperty);
}
LE_INITIAL_IMPL_END(LEShaderSpecializationConstantInfo)

LE_ADD_PROPERTY_BEGIN(LEShaderSpecializationConstantInfo)
LE_ADD_PROPERTY_STRING(LEShaderSpecializationConstantInfo, name);
LE_ADD_PROPERTY_STRING(LEShaderSpecializationConstantInfo, type);
LE_ADD_PROPERTY_RANGE_NOUSE(LEShaderSpecializationConstantInfo, id);
LE_ADD_PROPERTY_OBJECT(LEShaderSpecializationConstantInfo, defaultValue);
LE_ADD_PROPERTY_END

// LEShaderBufferInfo
LE_INITIAL_IMPL_BEGIN(LEShaderBufferInfo)
{
	LE_PROPERTY_REGISTER_METHOD(&LEShaderBufferInfo::initProperty);
	LE_PROPERTY_DESTORY_METHOD(&LEShaderBufferInfo::terminateProperty);
}
LE_INITIAL_IMPL_END(LEShaderBufferInfo)

LE_ADD_PROPERTY_BEGIN(LEShaderBufferInfo)
LE_ADD_PROPERTY_RANGE_NOUSE(LEShaderBufferInfo, blockSize);
LE_ADD_PROPERTY_RANGE_NOUSE(LEShaderBufferInfo, set);
LE_ADD_PROPERTY_RANGE_NOUSE(LEShaderBufferInfo, binding);
LE_ADD_PROPERTY_STRING(LEShaderBufferInfo, name);
LE_ADD_PROPERTY_STRING(LEShaderBufferInfo, type);
LE_ADD_PROPERTY_ARRAY(LEShaderBufferInfo, members);
LE_ADD_PROPERTY_END

// LEShaderTextureInfo
LE_INITIAL_IMPL_BEGIN(LEShaderTextureInfo)
{
	LE_PROPERTY_REGISTER_METHOD(&LEShaderTextureInfo::initProperty);
	LE_PROPERTY_DESTORY_METHOD(&LEShaderTextureInfo::terminateProperty);
}
LE_INITIAL_IMPL_END(LEShaderTextureInfo)

LE_ADD_PROPERTY_BEGIN(LEShaderTextureInfo)
LE_ADD_PROPERTY_RANGE_NOUSE(LEShaderTextureInfo, set);
LE_ADD_PROPERTY_RANGE_NOUSE(LEShaderTextureInfo, binding);
LE_ADD_PROPERTY_STRING(LEShaderTextureInfo, name);
LE_ADD_PROPERTY_STRING(LEShaderTextureInfo, type);
LE_ADD_PROPERTY_END

// LEShaderMetaData
LE_INITIAL_IMPL_BEGIN(LEShaderMetaData)
{
	LE_PROPERTY_REGISTER_METHOD(&LEShaderMetaData::initProperty);
	LE_PROPERTY_DESTORY_METHOD(&LEShaderMetaData::terminateProperty);
}
LE_INITIAL_IMPL_END(LEShaderMetaData)

LE_ADD_PROPERTY_BEGIN(LEShaderMetaData)
LE_ADD_PROPERTY_RANGE_NOUSE(LEShaderMetaData, shaderStage);
LE_ADD_PROPERTY_ARRAY(LEShaderMetaData, inputInfo);
LE_ADD_PROPERTY_ARRAY(LEShaderMetaData, inputInfo);
LE_ADD_PROPERTY_ARRAY(LEShaderMetaData, outputInfo);
LE_ADD_PROPERTY_ARRAY(LEShaderMetaData, uboInfo);
LE_ADD_PROPERTY_ARRAY(LEShaderMetaData, ssboInfo);
LE_ADD_PROPERTY_ARRAY(LEShaderMetaData, textureInfo);
LE_ADD_PROPERTY_ARRAY(LEShaderMetaData, specializationConstantInfo);
LE_ADD_PROPERTY_END


LE_INITIAL_IMPL_BEGIN(LEShader)
{
	LE_PROPERTY_REGISTER_METHOD(&LEShader::initProperty);
	LE_PROPERTY_DESTORY_METHOD(&LEShader::terminateProperty);
}
LE_INITIAL_IMPL_END(LEShader)

LE_ADD_PROPERTY_BEGIN(LEShader)
LE_ADD_PROPERTY_POINTER(LEShader, m_pMetaData);
LE_ADD_PROPERTY_END

void LEShader::createShaderModule(int codeSize, uint32_t* pCode) {
	this->m_shaderModule = LEVkDevice::createShaderModule(codeSize, pCode);
}
