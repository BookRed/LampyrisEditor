/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_shader_compiler.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_shader_compiler.h"

 LEShaderCompiler::LEShaderCompiler() :
	m_executor() {}

LEShaderCompiler::~LEShaderCompiler() {}

int LEShaderCompiler::getFileSize(const std::string& fname)
{
	FILE* fp;
	if ((fp = ::fopen(fname.c_str(), "r")) == NULL) {
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	return ftell(fp);
}

LEShaderCompiler::SpvData LEShaderCompiler::loadSPV(const std::string& fname) {
	size_t size  = (getFileSize(fname));
	SpvData spvData;
	spvData.size = size;
	spvData.data = static_cast<uint32_t*>(malloc(size));
	char* buf    = (char*)spvData.data;
	char c_file[4096];
	strcpy(c_file, fname.c_str());
	FILE* fpSPV;
	fpSPV = fopen(c_file, "rb");
	LE_ASSERT(fpSPV != NULL);
	fread(buf, size, 1, fpSPV);
	return spvData;
}

LEShader* LEShaderCompiler::compile(const std::string& filePath,VkShaderStageFlagBits shaderStage) {
	LEFile file(filePath);
	LE_ASSERT(file.isExists(filePath));
	// the format of command line is:
	// tools/glslangValidator.exe [shaderFilePath] -o [compiliedShaderSavePath] -V "
	std::string stageString;
	std::string shaderStageParam;
	std::string macroName;
	if (shaderStage == VK_SHADER_STAGE_VERTEX_BIT) {
		shaderStageParam = " -fshader-stage=vert ";
		macroName = " -DVERTEX ";
		stageString = ".vert";
	}
	else if (shaderStage == VK_SHADER_STAGE_FRAGMENT_BIT) {
		shaderStageParam = " -fshader-stage=frag ";
		macroName = " -DFRAGMENT ";
		stageString = ".frag";
	}
	std::string targetfileName = (std::stringstream()
		<< file.getFileName()
		<< ".spv").str();
	std::string targetfilePath = (std::stringstream()
		<< "shaders\\"
		<< file.getFilePrefix()
		<< stageString
		<< ".spv").str();


	this->m_executor.setCommandLine((std::stringstream()
		<< "tools/glslc.exe " 
		<< shaderStageParam
		<< filePath
		<< " -o "
		<< targetfilePath
		<< macroName
		<< " -I " // include path
		<< "shaders/include"
		).str());

	//!LEFile::isExists(targetfilePath)) 
 	this->m_executor.execute();
	// success
	if (this->m_executor.getResult().empty()) {
		LEShader* pShader = new LEShader(filePath);
		// SPV file
		pShader->m_pMetaData = LEShaderMetaDataParser::getInstance()->parse(targetfilePath);
		auto spvData = loadSPV(targetfilePath);
		pShader->createShaderModule(spvData.size, spvData.data);
		return pShader;
	}
	// get compile error
	else {
		// this->m_executor.getResult();
		return nullptr;
	}
}
