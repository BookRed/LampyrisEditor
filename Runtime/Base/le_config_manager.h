#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_config_manager.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_CONFIG_MANAGER_H
#define LE_CONFIG_MANAGER_H

// LE Includes
#include "le_config.h"
#include "le_application.h"
#include "../Utility/le_file.h"

// STL Includes
#include <unordered_map>

class LEConfigManager:public Singleton<LEConfigManager> {
private:
	std::string                                m_searchPath;
	std::unordered_map<std::string, LEConfig*> m_configMap;
public:
	LEConfigManager(const std::string& searchPath = "");

	virtual ~LEConfigManager();

	LEConfig* getConfig(const std::string& name);
};
#endif // !LE_CONFIG_MANAGER_H