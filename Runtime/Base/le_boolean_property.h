#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_boolean_property.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_BOOLEAN_PROPERTY_H
#define LE_BOOLEAN_PROPERTY_H

// LE Includes
#include "le_property.h"

class LEDataStream;

class LEBooleanProperty:public LEProperty {
public:
	LEBooleanProperty(LEClassInfo*          belongedClass,
				      const std::string&    propertyName,
				      unsigned              fieldOffset,
					  PropertyFlag          flag):
				      LEProperty(belongedClass, propertyName, fieldOffset,flag) {}

	void set(void* objectPtr, void* value);

	void archive(LEDataStream& stream, void* objectPtr);
};

#endif // !LE_BOOLEAN_PROPERTY_H

