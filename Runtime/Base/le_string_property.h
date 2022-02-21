#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_string_property.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_STRING_H
#define LE_STRING_H

// LE Includes
#include "le_property.h"
#include "../Serialization/le_data_stream.h"

class LEDataStream;

class LEStringProperty :public LEProperty {
public:
	LEStringProperty(LEClassInfo*          belongedClass,
				     const std::string&    propertyName,
				     unsigned              fieldOffset,
				     PropertyFlag          flag):
				     LEProperty(belongedClass, propertyName, fieldOffset,flag) {
		this->m_property_type = LEProperty::PT_STRING;
	}

	void set(void* objectPtr, void* value) {
		void* address = this->getPropertyPointer(objectPtr);
		std::string& str   = *(std::string*)(address);
		std::string& strValue = (*(std::string*)(value));
		str.assign(strValue);
	}

	unsigned int getSize(void* objectPtr) const {
		void* address = this->getPropertyPointer(objectPtr);
		std::string& str = *(std::string*)(address);
		return str.size();
	}

	char& getCharAt(void* objectPtr,unsigned int index) const {
		void* address = this->getPropertyPointer(objectPtr);
		std::string& str = *(std::string*)(address);
		return str[index];
	}

	void archive(LEDataStream& stream, void* objectPtr) {
		void* address = this->getPropertyPointer(objectPtr);
		std::string& str = *(std::string*)(address);
		stream.archive(str);
	}
};
#endif // !LE_STRING_H