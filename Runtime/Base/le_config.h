#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  File
 * \File:    le_config.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_CONFIG_H
#define LE_CONFIG_H

// LE Includes
#include "le_singleton.h"
#include "le_subsystem.h"
#include "le_application.h"

// 3rd-party Includes
#include <inih/INIReader.h>

#if defined(LE_DEBUG)
#pragma comment(lib,"ini_util_d.lib")
#else
#pragma comment(lib,"ini_util.lib")
#endif // !LE_DEBUG
class LEConfig {
	LE_DECLARE_FRIEND_CLASS(ConfigManager);
private:
	INIReader* m_reader;
	LEConfig(INIReader* reader) :m_reader(reader) {}
public:
	~LEConfig() { LE_SAFE_RELEASE(m_reader); }

	LE_FORCEINLINE int readInt(const std::string& section, const std::string& field) {
		return this->m_reader->GetInteger(section, field, -1);
	}

	LE_FORCEINLINE bool readBool(const std::string& section, const std::string& field) {
		return this->m_reader->GetBoolean(section, field, false);
	}

	LE_FORCEINLINE float readFloat(const std::string& section, const std::string& field) {
		return this->m_reader->GetReal(section, field, -1.0f);
	}

	LE_FORCEINLINE std::string readString(const std::string& section, const std::string& field) {
		return this->m_reader->GetString(section, field, std::string());
	}

	friend class LEConfigManager;
};
#endif // !LE_CONFIG_H