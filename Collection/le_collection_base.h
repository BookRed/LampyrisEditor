#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Collection
 * \File:    le_collection_base.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_COLLECTION_BASE_H
#define LE_COLLECTION_BASE_H

// LE Includes
#include "le_collection_global.h"
#include "../Common/le_serializable.h"

LE_NAMESPACE_BEGIN(Collection)

LE_USING_NAMESPACE(Math);

template<class T>
class LECollectionBase :public LEObject,public Serializable {
protected:
	T*     m_pData;
	uint32_t m_capacity;
	uint32_t m_length;

	// this method will be called when element is appended, or manually 
	virtual void expandIfNeeded(uint32_t size) {
		if (size <= this->m_length) {
			return;
		}
		uint32_t oldCapacity = this->m_capacity;
		if (isPow2(size) == 0) {
			this->m_capacity = size;
		}
		else {
			this->m_capacity = getNextPow2(size);;
		}

		T* pData = new T[this->m_capacity];
		::memcpy(pData, this->m_pData, oldCapacity * sizeof(T));
		LE_SAFE_RELEASE_ARRAY(this->m_pData);
		this->m_pData = pData;
		// this->m_length = this->m_capacity;
	}
public:
	LECollectionBase() :m_pData(nullptr),
					    m_capacity(0),
						m_length(0) {}

    LECollectionBase(uint32_t size,T element) :m_capacity(getMax(LE_COLLECTION_MIMIMUM_CAPACITY, size)),
										     m_pData(new T[getMax(LE_COLLECTION_MIMIMUM_CAPACITY, size)]),
											 m_length(size) {
		::memset(this->m_pData, 0, sizeof(T) * this->m_capacity);
	}

	~LECollectionBase() {
		LE_SAFE_RELEASE_ARRAY(this->m_pData);
	}
	LE_FORCEINLINE uint32_t getCapacity() const {
		return this->m_capacity;
	}

	LE_FORCEINLINE uint32_t isEmpty() const {
		return this->m_length == 0;
	}

	LE_FORCEINLINE uint32_t getLength() const {
		return this->m_length;
	}

	LE_FORCEINLINE const T* getData() const {
		return this->m_pData;
	}


	// manually call this method to reserve capacity.
	virtual void setCapacity(uint32_t size) {
		this->expandIfNeeded(size);
		this->m_length = this->m_capacity;
	}
	void swap(LECollectionBase<T>& collection) {
		/* Swap m_pData,capacity,and length */
		{
			auto t = collection.m_pData;
			collection.m_pData = this->m_pData;
			this->m_pData = t;
		}
		{
			auto t = collection.m_length;
			collection.m_length = this->m_length;
			this->m_length = t;
		}
		{
			auto t = collection.m_capacity;
			collection.m_capacity = this->m_capacity;
			this->m_capacity = t;
		}
	}

	void shrink() {
		if (this->m_capacity >= this->m_length) {
			return;
		}
		else {
			this->m_capacity = this->m_length;
			T* pData = new T[this->m_length];
			::memcpy(pData, this->m_pData, sizeof(T) * this->m_capacity);
			LE_SAFE_RELEASE_ARRAY(this->m_pData);
		}
	}

	void clear() {
		this->m_length = 0;
		this->m_capacity = 0;
		LE_SAFE_RELEASE_ARRAY(this->m_pData);
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER();

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		ar<< this->m_capacity;
		ar<< this->m_length;
		for (uint32_t i = 0; i < this->m_capacity; i++) {
			ar<< this->m_pData[i];
		}
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		ar>> this->m_capacity;
		ar>> this->m_length;
		LE_SAFE_RELEASE_ARRAY(this->m_pData);
		this->m_pData = new T[this->m_capacity];
		for (uint32_t i = 0; i < this->m_capacity; i++) {
			ar>> this->m_pData[i];
		}
	}
};

LE_NAMESPACE_END
#endif // !LE_COLLECTION_BASE_H
