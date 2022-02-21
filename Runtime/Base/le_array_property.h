#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_array_property.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_ARRAY_PROPERTY_H
#define LE_ARRAY_PROPERTY_H

// LE Includes
#include "le_property.h"
#include "../Serialization/le_data_stream.h"

// supported type: std::vector
template<typename T>
class LEArrayProperty:public LEProperty {
public:
	LEArrayProperty(LEClassInfo* belongedClass, const std::string& propertyName, unsigned fieldOffset, PropertyFlag flag) :
		LEProperty(belongedClass, propertyName, fieldOffset, flag)
		{ m_property_type = (PropertyType::PT_ARRAY);}

	std::vector<T>& getArrayObject(void* objectPtr) const {
		void* address = this->getPropertyPointer(objectPtr);
		std::vector<T>& vec = (*(std::vector<T>*)(address));
		return vec;
	}

	unsigned int getArraySize(void* objectPtr) const {
		return this->getArrayObject(objectPtr).size();
	}

	T& getElement(void* objectPtr, int index) const {
		void* address = this->getPropertyPointer(objectPtr);
		std::vector<T>& vec = (*(std::vector<T>*)(address));
		LE_ASSERT(index < vec.size() && index >= 0);
		return vec[index];
	}

	void setElement(void* objectPtr, int index, const T& value) const {
		this->getElement(objectPtr, index) = value;
	}

	void clear(void* objectPtr) {
		this->getArrayObject(objectPtr).clear();
	}

	void addElement(void* objectPtr, const T& value) {
		this->getArrayObject(objectPtr).push_back(value);
	}

	void resize(void* objectPtr, unsigned int size) {
		this->getArrayObject(objectPtr).resize(size);
	}

	void set(void* objectPtr, void* value) {
		void* address = this->getPropertyPointer(objectPtr);
		(*(std::vector<T>*)(address)) = (*(std::vector<T>*)(value));
	}

	void archive(LEDataStream& stream, void* objectPtr) {
		void* address = this->getPropertyPointer(objectPtr);
		std::vector<T>& vec = (*(std::vector<T>*)(address));
		stream.archive(vec);
	}

};
#endif // !LE_ARRAY_PROPERTY_H

