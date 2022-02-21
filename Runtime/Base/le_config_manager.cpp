/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Base
 * \File:    le_config_manager.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_config_manager.h"
#include "../Utility/le_file.h"

LEConfigManager::LEConfigManager(const std::string& searchPath) :
	m_searchPath(searchPath) {
	if (searchPath.empty()) {
		std::string appPath = LEApplication::getInstance()->getApplicationDirectory();
		LEFile file(appPath);
		this->m_searchPath =  file.getFileDirectoryPath() + "\\cfgs\\";
	}
}

LEConfigManager::~LEConfigManager() {}

LEConfig* LEConfigManager::getConfig(const std::string& name) {
	if (this->m_configMap.contains(name)) {
		return this->m_configMap[name];
	}
	else {
		std::string path = m_searchPath + name + ".ini";
		if (!LEFile::isExists(path)) {
			return nullptr;
		}
		INIReader* reader = new INIReader(path);
		LEConfig* cfg = new LEConfig(reader);
		this->m_configMap[name] = cfg;
		return cfg;
	}
}
