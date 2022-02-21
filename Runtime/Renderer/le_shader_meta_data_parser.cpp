/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_shader_meta_data_parser.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_shader_meta_data_parser.h"

void LEShaderMetaDataParser::parserBufferInfo(LEJsonReader* reader, const char* pBufferTypeString, std::vector<LEShaderBufferInfo>& bufferInfo) {
	if (reader->contains(pBufferTypeString)) {
		int uboSize = reader->getArraySize(pBufferTypeString);
		bufferInfo.resize(uboSize);
		for (int i = 0; i < uboSize; i++) {
			LEShaderBufferInfo buffer;
			buffer.blockSize = reader->readInt(pBufferTypeString, i, "block_size");
			buffer.set       = reader->readInt(pBufferTypeString, i, "set");
			buffer.binding   = reader->readInt(pBufferTypeString, i, "binding");
			buffer.name      = reader->readString(pBufferTypeString, i, "name");
			buffer.type      = reader->readString(pBufferTypeString, i, "type");
			bufferInfo[i] = buffer;
			int uboMemberSize = reader->getArraySize("types", bufferInfo[i].type, "members");
			for (int j = 0; j < uboMemberSize; j++) {
				LEShaderMemberInfo memberInfo;
				memberInfo.name = reader->readString("types", bufferInfo[i].type, "members", i, "name");
				memberInfo.type = reader->readString("types", bufferInfo[i].type, "members", i, "type");
				memberInfo.offset = reader->readInt("types", bufferInfo[i].type, "members", i, "offset");
				// uboMemberSize += s_glslTypeByteCountMap[memberInfo.type];
				bufferInfo[i].members.push_back(memberInfo);
			}
		}
	}
}

void LEShaderMetaDataParser::parsePushConstant(LEJsonReader* reader, LEShaderMetaData* pMetaData) {
	if (reader->contains("push_constants")) {
		std::string pushConstantType = reader->readString("push_constants", 0, "type");
		std::string pushConstantName = reader->readString("push_constants", 0, "name");
		int pushConstantMemberCount = reader->getArraySize("types", pushConstantType, "members");
		int pushConstantBlockSize = 0;
		for (int i = 0; i < pushConstantMemberCount; i++) {
			LEShaderMemberInfo memberInfo;
		    memberInfo.name = reader->readString("types", pushConstantType, "members", i, "name");
		    memberInfo.type = reader->readString("types", pushConstantType, "members", i, "type");
		    memberInfo.offset = reader->readInt("types", pushConstantType, "members", i, "offset");
			pushConstantBlockSize += s_glslTypeByteCountMap[memberInfo.type];
			pMetaData->pushConstantInfo.members.push_back(memberInfo);
		}
		pMetaData->pushConstantInfo.blockSize = pushConstantBlockSize;
		pMetaData->pushConstantInfo.name = pushConstantName;
	}
}

void LEShaderMetaDataParser::parseSpecializationConstants(LEJsonReader* reader, LEShaderMetaData* pMetaData) {
	if (reader->contains("specialization_constants")) {
		int specializationConstantsCount = reader->getArraySize("specialization_constants");
		pMetaData->specializationConstantInfo.resize(specializationConstantsCount);
		for (int i = 0; i < specializationConstantsCount; i++) {
			pMetaData->specializationConstantInfo[i].name = reader->readString("specialization_constants", i, "name");
			pMetaData->specializationConstantInfo[i].type = reader->readString("specialization_constants", i, "type");
			pMetaData->specializationConstantInfo[i].id = reader->readInt("specialization_constants", i, "id");
			const std::string& type = pMetaData->specializationConstantInfo[i].type;
			if (type == "int") {
				pMetaData->specializationConstantInfo[i].defaultValue.intValue = reader->readInt("specialization_constants", i, "default_value");
			}
			else if (type == "uint") {
				pMetaData->specializationConstantInfo[i].defaultValue.uintValue = reader->readUInt("specialization_constants", i, "default_value");
			}
			else if (type == "float") {
				pMetaData->specializationConstantInfo[i].defaultValue.floatValue = reader->readFloat("specialization_constants", i, "default_value");
			}
			else if (type == "double") {
				pMetaData->specializationConstantInfo[i].defaultValue.doubleValue = reader->readDouble("specialization_constants", i, "default_value");
			}
			else if (type == "bool") {
				pMetaData->specializationConstantInfo[i].defaultValue.doubleValue = reader->readBool("specialization_constants", i, "default_value");
			}
		}
	}
}

void LEShaderMetaDataParser::parseTextureCombinedSampler(LEJsonReader* reader, LEShaderMetaData* pMetaData) {
	if (reader->contains("textures")) {
		int textureSize = reader->getArraySize("textures");
		for (int i = 0; i < textureSize; i++) {
			LEShaderTextureInfo textureInfo;
			textureInfo.name = reader->readString("textures", i, "name");
			textureInfo.type = reader->readString("textures", i, "type");
			textureInfo.set  = reader->readInt("textures", i, "set");
			textureInfo.binding = reader->readInt("textures", i, "binding");
			pMetaData->textureInfo.push_back(textureInfo);
		}
	}
}

void LEShaderMetaDataParser::parseShaderStage(LEJsonReader* reader, LEShaderMetaData* pMetaData) {
	std::string entryPointMode = reader->readString("entryPoints", 0, "mode");
	if (entryPointMode == "vert") {
		pMetaData->shaderStage = VK_SHADER_STAGE_VERTEX_BIT;
	}
	else if (entryPointMode == "frag") {
		pMetaData->shaderStage = VK_SHADER_STAGE_FRAGMENT_BIT;
	}
	else if (entryPointMode == "comp") {
		pMetaData->shaderStage = VK_SHADER_STAGE_COMPUTE_BIT;
	}
	else if (entryPointMode == "geom") {
		pMetaData->shaderStage = VK_SHADER_STAGE_GEOMETRY_BIT;
	}
	else if (entryPointMode == "tesc") {
		pMetaData->shaderStage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	}
	else if (entryPointMode == "tese") {
		pMetaData->shaderStage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	}
	else {
		pMetaData->shaderStage = VK_SHADER_STAGE_ALL;
	}
}

void LEShaderMetaDataParser::parseInputOutput(LEJsonReader* reader, const char* pTypeString, std::vector<LEShaderIOInfo>& info) {
	if(reader->contains(pTypeString)) {
		int inputSize = reader->getArraySize(pTypeString);
		info.resize(inputSize);
		for (int i = 0; i < inputSize; i++) {
			info[i].location = reader->readInt(pTypeString, i, "location");
			info[i].type = reader->readString(pTypeString, i, "type");
			info[i].name = reader->readString(pTypeString, i, "name");
		}
	}
}

LEShaderMetaData* LEShaderMetaDataParser::parseInternal(LEJsonReader* reader) {
	// Lampyris limit rules temporary £º
	// ONE: input type must be scalar or 'vec2','vec3','vec4', matrix types are not allow.
	// TWO: 

	// TODO:
	// parse 'array','array_size_is_literal','input_attachment_id'

	// array_size_is_literal is relevant with specialization constant.

	LEShaderMetaData* pMetaData = new LEShaderMetaData;
	// read entryPoint info
	this->parseShaderStage(reader, pMetaData);
	// read inputs/outputs
	this->parseInputOutput(reader, "inputs", pMetaData->inputInfo);
	this->parseInputOutput(reader, "outputs", pMetaData->outputInfo);

	// read uniform buffer objects (UBOs) and storage buffer objects(SSBOs)
	this->parserBufferInfo(reader, "ubos", pMetaData->uboInfo);
	this->parserBufferInfo(reader, "ssbos", pMetaData->ssboInfo);
	
	// read push constant info
	this->parsePushConstant(reader, pMetaData);
	// specialization constants
	this->parseSpecializationConstants(reader, pMetaData);
	// read textures (uniform buffer with type likes 'SamplerXXX')
	this->parseTextureCombinedSampler(reader, pMetaData);
	// read subpassInput

	return pMetaData;
}

LEShaderMetaDataParser::LEShaderMetaDataParser() {}

LEShaderMetaDataParser::~LEShaderMetaDataParser() {}

LEShaderMetaData* LEShaderMetaDataParser::parse(const std::string& filePath) {
	this->m_executor.setCommandLine((std::stringstream()
		<< "tools\\spirv-cross.exe "
		<< filePath
		<< " --reflect").str());
	this->m_executor.execute();
	// LE_ASSERT(this->m_executor.getExitCode() == 0);
	auto& metaDataString = this->m_executor.getResult();
	LEJsonReader* reader = LEJsonReader::openFromString(metaDataString);
	LE_ASSERT(reader != nullptr);
	LEShaderMetaData* metaData = this->parseInternal(reader);
	LE_SAFE_RELEASE(reader);
	return metaData;
}
