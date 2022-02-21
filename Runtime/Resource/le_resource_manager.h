#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Resource
 * \File:    le_resource_manager.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_RESOURCE_MANAGER_H
#define LE_RESOURCE_MANAGER_H

// LE Includes
#include "Runtime/Base/le_initial.h"
#include "Runtime/Base/le_object.h"
#include "Runtime/Base/le_singleton.h"
// STL Includes
#include <unordered_map>
#include <unordered_set>

#include "le_resource.h"

typedef uint32_t     LEResourceID;

class LEResourceNode:public LEObject {
	LE_INITIAL_DECLARE
public:
	LEResourceID                 ID;
	uint32_t                     CRC;
	std::string                  assetPath;
	std::string                  serializedPath;
	std::vector<int>             children;
};

class LEResource;
class LEResourceManager: public LEObject,public Singleton<LEResourceManager> {
	LE_INITIAL_DECLARE
private:
	inline static const std::unordered_set<std::string> ms_supportedSuffix = { "png","jpg","ttf","obj" };

	std::unordered_map<LEResourceID, LEResourceNode*> m_resourceIDMap;
	std::unordered_map<std::string, LEResourceID>     m_resourcePathIDMap;
	std::unordered_set<LEResourceProxyBase*>          m_loadedResources;
public:
	LEResourceManager();

	~LEResourceManager();

	LEResourceProxyBase* load(const std::string& path);

	LEResourceProxyBase* load(LEResourceID id);

	LEResourceProxyBase* loadAsync(const std::string& path);

	LEResourceProxyBase* loadAsync(LEResourceID id);

	void unload(LEResourceProxyBase* pResourceProxy);

	void unloadAllUnused();

#if defined(LE_EDITOR)
	void onFolderDeleted();

	void onFileDeleted();

	void onFolderAdded();

	void onFileAdded(const std::string& path);

	void onFolderRenamed();

	void onFileRenamed();

	void checkResourceFileChanged();
#endif // !LE_EDITOR
};
#endif // !LE_RESOURCE_MANAGER_H