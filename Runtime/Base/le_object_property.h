#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_object_property.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_OBJECT_PROPERTY_H
#define LE_OBJECT_PROPERTY_H

// LE Includes
#include "le_property.h"
#include "le_assert.h"
#include "../Serialization/le_data_stream.h"

template<typename T> requires ClassType<T> || UnionType<T>
class LEObjectProperty:public LEProperty {
private:
	static constexpr unsigned int ms_byte_count = sizeof(T);
public:
	LEObjectProperty(LEClassInfo*          belongedClass,
		             const std::string&    propertyName,
		             unsigned              fieldOffset,
					 PropertyFlag          flag):
		LEProperty(belongedClass, propertyName, fieldOffset,flag) {}

	void set(void* objectPtr, void* value) {
		void* address = this->getPropertyPointer(objectPtr);
		(*(T*)(address)) = (*(T*)(value));
	}

	unsigned int getByteCount() const {
		return ms_byte_count;
	}

	void archive(LEDataStream& stream, void* objectPtr) {
		void* address = this->getPropertyPointer(objectPtr);
		stream.archive(*(T*)address);
	}
};

#endif // !LE_OBJECT_PROPERTY_H