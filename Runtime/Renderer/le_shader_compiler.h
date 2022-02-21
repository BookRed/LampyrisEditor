#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_shader_compiler.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_SHADER_COMPILER_H
#define LE_SHADER_COMPILER_H	

// LE Includes
#include "le_shader_meta_data_parser.h"

// STL Includes
#include <vector>
#include <sstream>

// vulkan Includes
#include <vulkan/vulkan.h>

// multi-thread compiling is not supported yet.
class LEShaderCompiler:public Singleton<LEShaderCompiler> {
private:
	typedef struct SpvDataStruct {
		int       size;
		uint32_t* data;
	} SpvData;

	LEExternalExecutor m_executor;
public:
	                  LEShaderCompiler();
			          
	                 ~LEShaderCompiler();
			          
	int               getFileSize(const std::string& fname);
			          
	SpvData           loadSPV(const std::string& fname);
			          
	LEShader*         compile(const std::string& filePath,VkShaderStageFlagBits shaderStage);
};
#endif // !LE_SHADER_COMPILER_H