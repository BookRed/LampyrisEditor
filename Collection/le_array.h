#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Collection
 * \File:    le_array.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_ARRAY_H
#define LE_ARRAY_H

// LE Includes
#include "le_collection_base.h"
#include "../Debug/le_assert.h"

LE_NAMESPACE_BEGIN(Collection)

template<class T>
class LEArray :public LECollectionBase<T> {
private:
	// you can not change the capacity
	void setCapacity(uint32_t size) {
		LECollectionBase<T>::setCapacity(size);
	}
public:
	// the capacity must be set,and can not be changed
	LEArray(uint32_t size):extend LECollectionBase<T> {

	}

	T& operator[] (int index) const {
		LE_ASSERT(Math::abs(index) < this->m_length);
		if (index >= 0) {
			return this->m_pData[index];
		}
		else {
			return this->m_pData[this->m_length + index];
		}
	}

	T& getFirst() const {
		LE_ASSERT(this->m_length != 0);
		return this->operator[](0);
	}

	T& getLast() const {
		LE_ASSERT(this->m_length != 0);
		return this->operator[](this->m_length-1);
	}
};

LE_NAMESPACE_END
#endif // !LE_ARRAY_H

