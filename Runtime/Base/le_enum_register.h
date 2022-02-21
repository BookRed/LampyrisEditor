#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_enum_register.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_ENUM_REGISTER_H
#define LE_ENUM_REGISTER_H

// LE Includes
#include "le_singleton.h"
#include "le_assert.h"

// STL Includes
#include <string>
#include <vector>
#include <unordered_map>

#define LE_ENUM_ADD(enum_name,value_name) \
	LEEnumRegister::getInstance()->addEnumValue(enum_name, \
											    value_name,  \
											    static_cast<int>(enum_name::value_name)

#define LE_ENUM_GET_INFO(enum_name) LEEnumRegister::getInstance()->getEnumInfo(#enum_name)

class LEEnumRegister:public Singleton<LEEnumRegister> {
public:
	struct EnumInfo {
		std::string value_name;
		int         value;
	};
private:
	bool repeatCheck(const std::string &enum_name, 
		             const std::string &value_name, 
		             int value) {
		auto enumInfo = this->m_enum_info_map[enum_name];
		for (int i = 0; i < enumInfo.size(); i++) {
			if (enumInfo[i].value_name == value_name) {
				return false;
			}
			else if (enumInfo[i].value == value) {
				return false;
			}
		}
		return true;
	}
	std::unordered_map<std::string, std::vector<EnumInfo>> m_enum_info_map;
public:
	void addEnumValue(const std::string &enum_name, 
		              const std::string &value_name, 
		              int value) {
		LE_ASSERT(this->repeatCheck(enum_name, value_name, value));
		this->m_enum_info_map[enum_name].push_back( EnumInfo { value_name, value });
	}

	std::vector<EnumInfo>& getEnumInfo(const std::string &enum_name) {
		LE_ASSERT(this->m_enum_info_map.contains(enum_name));
		return this->m_enum_info_map[enum_name];
	}

};
#endif // !LE_ENUM_REGISTER_H