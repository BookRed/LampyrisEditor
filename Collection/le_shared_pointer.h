#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Collection
 * \File:    le_shared_pointer.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_SHARED_POINTER_H
#define LE_SHARED_POINTER_H

// LE Includes
#include "../Common/le_object.h"

template<class T>
class LESharedPointer:public LEObject {
	static_assert(std::is_base_of<LEObject, T>::value,
				  "T must be the sub class of 'LEObject'");
	typename typedef LESharedPointer<T> Self;
private:
	T* m_rawPointer;
public:
	LESharedPointer(T* rawPointer = nullptr) {
		this->m_rawPointer = rawPointer;
		if (this->m_rawPointer){
			this->m_rawPointer->incRef();
		}
	}
	LESharedPointer(const LESharedPointer& pointer) {
		this->m_rawPointer = pointer.m_rawPointer;
		if (this->m_rawPointer) {
			this->m_rawPointer->incRef();
		}
	}
	~LESharedPointer() {
		if (this->m_rawPointer) {
			this->m_rawPointer->decRef();
			this->m_rawPointer = nullptr;
		}
	}
	Self& operator=(T* rawPointer) {
		if (this->m_rawPointer != rawPointer){
			if (rawPointer!=nullptr) {
				rawPointer->incRef();
			}
			// replace old raw pointer if it exists
			if (this->m_rawPointer!=nullptr) {
				this->m_rawPointer->decRef();
			}
			this->m_rawPointer = rawPointer;
		}
		return *this;
	}
	Self& operator= (const Self& pointer) {
		if (this->m_rawPointer != pointer.m_rawPointer) {
			if (pointer.m_rawPointer) {
				pointer.m_rawPointer->incRef();
			}
			if (this->m_rawPointer) {
				this->m_rawPointer->decRef();
			}
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
#endif // !LE_SHARED_POINTER_H

