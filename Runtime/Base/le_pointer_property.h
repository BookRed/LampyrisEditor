#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_pointer_property.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_POINTER_PROPERTY_H
#define LE_POINTER_PROPERTY_H

// LE Includes
#include "le_property.h"
#include "../Serialization/le_data_stream.h"

/* only valid for classes derived from LEObject */

class LEObject;
class LEPointerProperty :public LEProperty {
public:
	LEPointerProperty(LEClassInfo*       belongedClass,
		              const std::string& propertyName,
		              unsigned           fieldOffset,
					  PropertyFlag       flag) :
		              LEProperty(belongedClass, propertyName, fieldOffset,flag) {}

	void set(void* objectPtr, void* value) {
		void* address = this->getPropertyPointer(objectPtr);
		(*(LEObject**)(address)) = (*(LEObject**)(value));
	}

	void archive(LEDataStream& stream, void* objectPtr) {
		void* address = this->getPropertyPointer(objectPtr);
		LEObject*& pObject = *(LEObject**)address;
		stream.archive(pObject);
	}

};
#endif // !LE_POINTER_PROPERTY_H
