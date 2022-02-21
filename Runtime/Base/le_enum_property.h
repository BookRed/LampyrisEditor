#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_enum_property.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_ENUM_PROPERTY_H
#define LE_ENUM_PROPERTY_H

// LE Includes
#include "le_property.h"

class LEDataStream;

template<EnumType T>
class LEEnumProperty :public LEProperty {
private:
	std::string m_string;
public:
	LEEnumProperty(LEClassInfo*       belongedClass,
				   std::string&       propertyName,
				   unsigned int       fieldOffset,
		           const std::string& enum_name,
		           PropertyFlag       flag):
		m_string(enum_name),
		LEProperty(belongedClass, propertyName, fieldOffset,flag) {}

	void set(void* objectPtr, void* value) {
		void* address = this->getPropertyPointer(objectPtr);
		(*(*T)(address)) = (*(*T)(value));
	}

	void archive(LEDataStream& stream, void* object);
};

#endif // !LE_RANGE_PROPERTY_H

