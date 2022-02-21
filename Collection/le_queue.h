#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Collection
 * \File:    le_queue.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_QUEUE_H
#define LE_QUEUE_H

// LE Includes
#include "le_collection_base.h"
#include "../Debug/le_assert.h"

LE_NAMESPACE_BEGIN(Collection)

template<class T>
class LeQueue :public LECollectionBase<T> {
private:
	FEUInt m_front;
	FEUInt m_rear;

	void expandIfNeeded(uint32_t size) final {
		if (size <= this->m_length) {
			return;
		}
		uint32_t oldCapacity = this->m_capacity;
		if (Math::isPow2(size) == 0) {
			this->m_capacity = size;
		}
		else {
			this->m_capacity = Math::getNextPow2(size);;
		}
		// readjust the elements of queue
		T* pData = new T[this->m_capacity];
		::memcpy(pData, this->m_pData + this->m_front, (oldCapacity - this->m_front) * sizeof(T));
		::memcpy(pData + this->m_front, this->m_pData, this->m_front) * sizeof(T));
		LE_SAFE_RELEASE_ARRAY(this->m_pData);
		this->m_pData = pData;
	}

	LEBool isFull() const {
		return (this->m_rear + 1) % this->m_capacity == this->m_front;
	}
public:
	LeQueue() {

	}
	LeQueue(uint32_t size = 0, const T& value = T()) :
		LECollectionBase<T>(size, value) {

	}
	LeQueue(const LEVector& other) {
		this->m_capacity = other.m_capacity;
		this->m_length = other.m_length;
		this->m_pData = new T[this->m_capacity];
		::memcpy(this->m_pData, other.m_pData, sizeof(T) * this->m_capacity);
	}

	LeQueue(const LEVector&& other) {
		this->swap(other);
	}

	LEBool isEmpty() const {
		return this->m_rear == this->m_front;
	}

	void push(const T& value) {
		// expand the capacity if the queue is full
		if (this->isFull()) {
			this->expandIfNeeded(this->m_capacity * LE_COLLECTION_EXPAND_FACTOR);
		}
		// Insert an element at the rear of the queue 
		this->m_pData[this->m_rear] = value;
		this->m_rear = (this->m_rear + 1) % this->m_capacity;
	}

	T& pop(const T& value) {
		LE_ASSERT(!this->isEmpty());
		T& value = this->m_pData[this->front]; ;
		this->m_front = (this->m_front + 1) % this->m_capacity;
		return value;
	}

	T& getFront() {
		LE_ASSERT(!this->isEmpty());
		return this->m_pData[this->m_front];
	}


	T& getRear() {
		LE_ASSERT(!this->isEmpty());
		return this->m_pData[this->m_rear];
	}

	LeQueue<T>& operator=(const LeQueue<T>& other) {
		this->m_capacity = other.m_capacity;
		this->m_length = other.m_length;
		this->m_pData = new T[this->m_capacity];
		::memcpy(this->m_pData, other.m_pData, sizeof(T) * this->m_capacity);
	}

	virtual std::string toString() const {
		std::string returnValue;
		returnValue = "object of <class 'LEQueue'>, length = " +
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

#endif // !LE_QUEUE_H

