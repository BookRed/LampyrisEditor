#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Collection
 * \File:    le_weak_pointer.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_WEAK_POINTER_H
#define LE_WEAK_POINTER_H

// LE Includes
#include "../Common/le_object.h"

template<class T>
class LEWeakPointer :public LEObject {
	static_assert(std::is_base_of<LEObject, T>::value,
		          "T must be the sub class of 'LEObject'");
	typename typedef LEWeakPointer<T> Self;
private:
	T* m_rawPointer;
public:
	LEWeakPointer(T* rawPointer = nullptr) {
		this->m_rawPointer = rawPointer;
	}
	LEWeakPointer(const LEWeakPointer& pointer) {
		this->m_rawPointer = pointer.m_rawPointer;
	}
	~LEWeakPointer() {
		if (this->m_rawPointer) {
			this->m_rawPointer = nullptr;
		}
	}
	Self& operator=(T* rawPointer) {
		if (this->m_rawPointer != rawPointer) {
			this->m_rawPointer = rawPointer;
		}
		return *this;
	}
	Self& operator= (const Self& pointer) {
		if (this->m_rawPointer != pointer.m_rawPointer) {
			this->m_rawPointer = pointer.m_rawPointer;
		}
		return *this;
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
};
#endif // !LE_WEAK_POINTER_H

