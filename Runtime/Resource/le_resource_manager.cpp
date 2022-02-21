/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Resource
 * \File:    le_resource_manager.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_resource_manager.h"
#include "importer/le_font_importer.h"
#include "importer/le_image_importer.h"
#include "importer/le_obj_importer.h"
#include "../Renderer/le_shader_compiler.h"
#include "../Serialization/le_data_stream.h"
#include "../Utility/le_crc_check.h"
#include "../Utility/le_random.h"
#include "../Base/le_class_info.h"

LE_INITIAL_IMPL_BEGIN(LEResourceManager)
{
	LE_PROPERTY_REGISTER_METHOD(&LEResourceManager::initProperty);
	LE_PROPERTY_DESTORY_METHOD(&LEResourceManager::terminateProperty);
	LE_CREATE_INSTANCE_METHOD(&LEResourceManager::createInstance);
}
LE_INITIAL_IMPL_END(LEResourceManager)

LE_ADD_PROPERTY_BEGIN(LEResourceManager)
LE_ADD_PROPERTY_END

LE_INITIAL_IMPL_BEGIN(LEResourceNode)
{

}
LE_INITIAL_IMPL_END(LEResourceNode)

LE_ADD_PROPERTY_BEGIN(LEResourceNode)
LE_ADD_PROPERTY_END
LEResourceManager::LEResourceManager() {

}

LEResourceManager::~LEResourceManager() {

}

LEResourceProxyBase* LEResourceManager::load(const std::string& path) {
	LE_ASSERT(LEFile::isExists(path));
		
	LEResourceProxyBase* pResourceProxy = new LEResourceProxyBase;
	return pResourceProxy;
}

LEResourceProxyBase* LEResourceManager::load(LEResourceID id) {
	LEResourceProxyBase* pResourceProxy = new LEResourceProxyBase;
	return pResourceProxy;
}

LEResourceProxyBase* LEResourceManager::loadAsync(const std::string& path) {
	LEResourceProxyBase* pResourceProxy = new LEResourceProxyBase;
	return pResourceProxy;
}

LEResourceProxyBase* LEResourceManager::loadAsync(LEResourceID id) {
	LEResourceProxyBase* pResourceProxy = new LEResourceProxyBase;
	return pResourceProxy;
}

void LEResourceManager::unload(LEResourceProxyBase* pResourceProxy) {
		
}

void LEResourceManager::unloadAllUnused() {
		
}

#if defined(LE_EDITOR)
void LEResourceManager::onFolderDeleted() {
		
}

void LEResourceManager::onFileDeleted() {
		
}

void LEResourceManager::onFolderAdded() {

}

void LEResourceManager::onFileAdded(const std::string& path) {
	const std::string suffix = LEPathUtility::getSuffix(path);
	const std::string fileName = LEPathUtility::getFileName(path);
	const std::string filePath = LEPathUtility::getFileDirectoryPath(path);
	const std::string serializedPath = LEApplication::getInstance()->getCurrentWorkDirectory() + "\\data\\res\\" + fileName;
	if(!ms_supportedSuffix.contains(suffix)) {
		return;
	}
	if (suffix == "ttf" || suffix == "cs") {
		return;
	}
	LEObject* pResourceObject = nullptr;
	if(suffix == "obj") {
		LEObjImporter importer(path);
		importer.loadModelData();
		pResourceObject = importer.registerAsset();
	}
	else if(suffix == "png" || suffix == "jpg" || suffix == "bmp" || suffix == "tag" || suffix == "psd") {
		LEImageImporter importer(path);
		importer.loadImageData();
		pResourceObject = importer.registerAsset();
	}
	else if(suffix == "glsl") {
		std::string errorString;
		LEShader* pShader = LEShaderCompiler::getInstance()->compile(path,VK_SHADER_STAGE_VERTEX_BIT);
	}
	else if(suffix == "wav") {
			
	}
	// update resource map
	LEResourceNode* pNode = new LEResourceNode;
	LEResourceID    id    = le_random_int();
	pNode->ID             = le_random_int();
	pNode->serializedPath = serializedPath;
	pNode->assetPath      = path;
	pNode->CRC            = LECRCCheck::calculate(path);
	LE_ASSERT(this->m_resourcePathIDMap.contains(filePath));
	uint32_t parentID = this->m_resourcePathIDMap[filePath];
	LEResourceNode* pParent = this->m_resourceIDMap[id];
	pParent->children.push_back(id);
	this->m_resourceIDMap[parentID] = pNode;
	
	// get serialized object and save
	if(pResourceObject) {
		LEDataStream stream;
		stream.setArchiveFilePath(serializedPath);
		stream.saveObject(pResourceObject);
		LE_SAFE_RELEASE(pResourceObject);
	}
}

void LEResourceManager::onFolderRenamed() {

}

void LEResourceManager::onFileRenamed() {

}

void LEResourceManager::checkResourceFileChanged() {
		
}
#endif // !LE_EDITOR
