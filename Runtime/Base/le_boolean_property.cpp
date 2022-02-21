#pragma once
/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Base
 * \File:    le_boolean_property.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_boolean_property.h"
#include "../Serialization/le_data_stream.h"

void LEBooleanProperty::set(void* objectPtr, void* value) {
	void* address = this->getPropertyPointer(objectPtr);
	(*(bool*)(address)) = (*(bool*)(value));
}

void LEBooleanProperty::archive(LEDataStream& stream, void* objectPtr) {
	void* address = this->getPropertyPointer(objectPtr);
	stream.archive((*(bool*)address));
}
