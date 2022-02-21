#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Collection
 * \File:    le_vector.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_VECTOR_H
#define LE_VECTOR_H

// LE Includes
#include "le_collection_base.h"
#include "../Debug/le_assert.h"

LE_NAMESPACE_BEGIN(Collection)

template<class T>
class LEVector :public LECollectionBase<T> {
public:
	LEVector(uint32_t size = 0,const T& value= T()):
		LECollectionBase<T>(size,value) {}

	LEVector(const LEVector& other) {
		this->m_capacity = other.m_capacity;
		this->m_length = other.m_length;
		this->m_pData = new T[this->m_capacity];
		::memcpy(this->m_pData, other.m_pData, sizeof(T) * this->m_capacity);
	}

	LEVector(LEVector<T>&& other) {
		this->swap(other);
	}

	T& operator[] (int index) const{
		LE_ASSERT(index< this->m_length);
		return this->m_pData[index];
	}

	void appendFront(const T& value) {
		if (this->m_length >= this->m_capacity) {
			this->expandIfNeeded(this->m_capacity * LE_COLLECTION_EXPAND_FACTOR);
		}
		for (uint32_t i = this->m_length - 1; i >= 0; i--) {
			this->m_pData[i + 1] = this->m_pData[i];
		}
		this->m_pData[0] = value;
	}

	void appendBack(const T& value) {
		if (this->m_length >= this->m_capacity) {
			this->expandIfNeeded(this->m_capacity * LE_COLLECTION_EXPAND_FACTOR);
		}
		this->m_pData[this->m_length++] = value;
	}

	template<typename ...Args>
	void emplaceFront(Args... args) {
		if (this->m_length >= this->m_capacity) {
			this->expandIfNeeded(this->m_capacity * LE_COLLECTION_EXPAND_FACTOR);
		}
		for (uint32_t i = this->m_length - 1; i >= 0; i--) {
			this->m_pData[i + 1] = this->m_pData[i];
		}
		this->m_pData[0] = T(args...);
	}

	template<typename ...Args>
	void emplaceBack(Args... args) {
		if (this->m_length >= this->m_capacity) {
			this->expandIfNeeded(this->m_capacity * LE_COLLECTION_EXPAND_FACTOR);
		}
		this->m_pData[this->m_length++] = T(args...);
	}

	T& getFirst() const {
		LE_ASSERT(this->m_length != 0);
		return this->operator[](0);
	}

	T& getLast() const {
		LE_ASSERT(this->m_length != 0);
		return this->operator[](-1);
	}

	LEVector<T>& operator=(const LEVector<T>& other) {
		this->m_capacity = other.m_capacity;
		this->m_length = other.m_length;
		this->m_pData = new T[this->m_capacity];
		::memcpy(this->m_pData, other.m_pData, sizeof(T) * this->m_capacity);
	}

	virtual std::string toString() const {
		std::string returnValue;
		returnValue = "object of <class 'LEVector'>, length = " + 
			          std::to_string(this->m_length) + 
			          ", capacity =" + 
			          std::to_string(this->m_capacity) + "\n";
		returnValue = returnValue + "[";
		for (int i = 0; i < this->m_length; i++) {
			// returnValue = std::to_string(this->m_pData[i]);
			if (i != this->m_length - 1) {
				returnValue += ',';
			}
		}
		returnValue = returnValue + "]";
		return returnValue;
	}

};

LE_NAMESPACE_END

#endif // !LE_VECTOR_H

