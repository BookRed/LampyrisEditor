#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Collection
 * \File:    le_unique_pointer.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_UNIQUE_POINTER_H
#define LE_UNIQUE_POINTER_H

// LE Includes
#include "../Common/le_object.h"

template<class T>
class LEUniquePointer :public LEObject {
	static_assert(std::is_base_of<LEObject, T>::value,
			"T must be the sub class of 'LEObject'");
	typename typedef LEUniquePointer<T> Self;
private:
	T* m_rawPointer;
public:
	LEUniquePointer(T* rawPointer = nullptr) {
		this->m_rawPointer = rawPointer;
		if (this->m_rawPointer) {
			this->m_rawPointer->incRef();
		}
	}
	
	~LEUniquePointer() {
		if (this->m_rawPointer) {
			this->m_rawPointer->decRef();
			this->m_rawPointer = nullptr;
		}
	}

	LEBool operator== (T* rawPointer) const
	{
		return this->m_rawPointer == rawPointer;
	}
	LEBool operator!= (T* rawPointer) const
	{
		return this->m_rawPointer != rawPointer;
	}

	LEBool operator== (const Self& pointer) const
	{
		return this->m_rawPointer == pointer->m_rawPointer;
	}
	LEBool operator!= (const Self& pointer) const
	{
		return this->m_rawPointer != pointer->m_rawPointer;
	}
	T& operator*() const
	{
		return *this->m_rawPointer;
	}
	T* operator->() const
	{
		return this->m_rawPointer;
	}

	// Deleted methods
	LEUniquePointer(const LEUniquePointer& pointer) = delete;

	Self& operator=(T* rawPointer) = delete;

	Self& operator= (const Self& pointer) = delete;
};
#endif // !LE_UNIQUE_POINTER_H

