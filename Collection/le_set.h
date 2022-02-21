#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Collection
 * \File:    le_map.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_SET_H
#define LE_SET_H

#define LE_SET_TYPENAMES  typename Key, class HashFunction
#define LE_SET_TYPENAMES_WITH_DEFAULT LE_SET_TYPENAMES = LEHash<Key>
#define LE_SET_TYPES Key,HashFunction


// LE-Includes
#include "le_pair.h"
#include "le_node.h"
#include "le_vector.h"
#include "../common/le_global.h"
#include "../common/le_hash.h"

LE_NAMESPACE_BEGIN(Collection)

template<LE_SET_TYPENAMES_WITH_DEFAULT>
class LESetBucket;

template<LE_SET_TYPENAMES_WITH_DEFAULT>
class LESet;

LE_USING_DETAIL_NAMESPACE(Collection);

template<LE_SET_TYPENAMES>
class LESetIterator {
	typename typedef Key                             NodeElement;
	typename typedef LENode<NodeElement>             Node;
	typename typedef LESetIterator<LE_SET_TYPES>     Self;
	typename typedef LESetBucket<LE_SET_TYPES>       Bucket;
private:				
	Node* m_pNode;
	Bucket* m_pBucket;
public:
	LESetIterator(Node* pNode = nullptr,
		Bucket* pBucket = nullptr) :
		m_pNode(pNode),
		m_pBucket(pBucket) {}

	// prefix operator ++;
	Self& operator++() {
		this->moveToNext();
		return *this;
	}
	// suffix operator ++;
	Self operator++(int) {
		Self returnValue(*this);
		this->moveToNext();
		return returnValue;
	}

	LEBool operator!=(Self& object) const {
		return this->m_pNode != object.m_pNode;
	}

	LEBool operator==(Self& object) const {
		return this->m_pNode == object.m_pNode;
	}

	void moveToNext() {
		if (this->m_pNode->next) {
			this->m_pNode = this->m_pNode->next;
		}
		else {
			// get next non-null bucket
			uint32_t bucketNo = HashFunction()(this->m_pNode->data.first) + 1;
			for (; bucketNo < this->m_pBucket->m_nodes.getCapacity(); bucketNo++) {
				if (this->m_pBucket[bucketNo] != nullptr) {
					this->m_pNode = m_pBucket->m_nodes[bucketNo];
					return;
				}
			}
			m_pBucket = nullptr;
		}
	}
};

template<LE_SET_TYPENAMES>
class LESetBucket :extend Serializable {
	typename typedef Key                          NodeElement;
	typename typedef LENode<NodeElement>          Node;
	typename typedef LESetBucket<LE_SET_TYPES>    Self;
private:
	uint32_t             m_length;
	LEVector<Node*>    m_nodes;

	void expandIfNeeded() {											
		if (this->m_length == this->m_nodes.getCapacity()) {
			uint32_t size = getNextPrime(this->m_length);
			Self bucket(size); // a new bucket
			for (uint32_t bucketNo = 0; bucketNo < this->m_nodes.getCapacity(); bucketNo++) {
				Node* pNode = this->m_nodes[bucketNo];
				while (pNode != nullptr) {
					// re-hash
					uint32_t newBucketNo = HashFunction()(pNode->data) % size;
					// for each nodes, we remove it from old bucket,and insert it into new bucket.
					this->m_nodes[bucketNo] = pNode->next;
					pNode->next = bucket.m_nodes[newBucketNo];
					bucket.m_nodes[newBucketNo] = pNode;
					bucket.m_length++;
					pNode = this->m_nodes[bucketNo];
				}
			}
			this->swap(bucket);
		}
	}
public:
	typename typedef LESetIterator<LE_SET_TYPES>  Iterator;
	LESetBucket(uint32_t capacity = 16) :m_length(0), m_nodes(LEVector<Node*>(capacity)) {

	}
	~LESetBucket() {

	}
	LEBool contains(const Key& key) {
		return this->locateNode(key) != nullptr;
	}
	LEPair<Iterator, LEBool> insert(const Key& key) {
		this->expandIfNeeded();
		uint32_t bucketNo = HashFunction()(key) % this->m_nodes.getCapacity();;
		Node* pNode = this->locateNode(key);
		while (pNode != nullptr) {
			if (pNode->data == key) {
				// already exists
				return LEPair<Iterator, LEBool>(Iterator(pNode, this), false);
			}
			pNode = pNode->next;
		}
		pNode = new Node(key);
		pNode->next = m_nodes[bucketNo];
		this->m_nodes[bucketNo] = pNode;
		this->m_length++;
		return LEPair<Iterator, LEBool>(Iterator(pNode, this), true);
	}

	Node* locateNode(const Key& key) {
		uint32_t bucketNo = HashFunction()(key) % this->m_nodes.getCapacity();
		Node* pNode = this->m_nodes[bucketNo];
		while (pNode != nullptr) {
			if (pNode->data == key)
				return pNode;
			pNode = pNode->next;
		}
		return nullptr;
	}

	// can not be const method
	Iterator at(const Key& key) {
		uint32_t bucketNo = HashFunction()(key) % this->m_nodes.getCapacity();
		Node* pNode = this->m_nodes[bucketNo];
		while (pNode != nullptr) {
			if (pNode->data == key) {
				Iterator iter(pNode, this);
				return iter;
			}
			pNode = pNode->next;
		}
		return this->end();
	}

	LE_FORCEINLINE uint32_t getLength() const {
		return this->m_length;
	}

	LE_FORCEINLINE LEBool isEmpty() const {
		return this->m_length == 0;
	}

	LEBool erase(const Key& key) {
		uint32_t bucketNo = HashFunction()(key) % this->m_nodes.getCapacity();;
		Node* pNode = this->m_nodes[bucketNo];
		Node* pPrev = nullptr;
		while (pNode) {
			if (pNode->data == key) {
				if (pPrev == nullptr) {
					// delete the only node
					this->m_nodes[bucketNo] = pPrev->next;
				}
				else {
					pPrev->next = pNode->next;
				}
				LE_SAFE_RELEASE(pNode);
				this->m_length--;
				return true;
			}
			else {
				pPrev = pNode;
				pNode = pNode->next;
			}
		}
		return false;
	}

	void clear() {
		for (uint32_t bucketNo = 0; bucketNo < this->m_nodes.getCapacity(); bucketNo++) {
			Node* pNode = this->m_nodes[bucketNo];
			while (pNode != nullptr) {
				this->m_nodes[bucketNo] = pNode->next;
				LE_SAFE_RELEASE(pNode);
				pNode = this->m_nodes[bucketNo];
			}
		}
		this->m_length = 0;
	}

	Iterator begin() {
		for (uint32_t i = 0; i < this->m_nodes.getCapacity(); i++) {
			if (this->m_nodes[i] != nullptr)
				return Iterator(this->m_nodes[i], this);
		}
		return this->end();
	}

	Iterator end() {
		return Iterator(nullptr, this);
	}

	void swap(Self& bucket) {
		this->m_nodes.swap(bucket.m_nodes);
		std::swap(this->m_length, bucket.m_length);
	}

	template<class Archive>
	void serialize(Archive& ar, const unsigned int file_version) {
		ar& this->m_length;
		ar& this->m_nodes;
	}
};

template<LE_SET_TYPENAMES>
class LESet:extend Serializable {
	typename typedef LESetBucket<LE_SET_TYPES>::Iterator Iterator;
	typename typedef LESet<LE_SET_TYPES>                Self;
private:
	LESetBucket<LE_SET_TYPES> m_bucket;
public:
	LESet() : m_bucket() { }

	LE_FORCEINLINE Iterator begin() const {
		return this->m_bucket.begin();
	}

	LE_FORCEINLINE Iterator  end() const {
		return this->m_bucket.end();
	}

	LE_FORCEINLINE LEBool isEmpty()const {
		return this->m_bucket.isEmpty();
	}

	LE_FORCEINLINE uint32_t getLength() {
		return this->m_bucket.getLength();
	}

	LE_FORCEINLINE LEBool contains(const Key& key) {
		return this->m_bucket.contains(key);
	}

	LE_FORCEINLINE LEPair<Iterator, LEBool> insert(const Key& key) {
		return this->m_bucket.insert(key);
	}

	LE_FORCEINLINE LEBool erase(const Key& key) {
		return this->m_bucket.erase(key);
	}

	LE_FORCEINLINE void clear() {
		this->m_bucket.clear();
	}

	LE_FORCEINLINE void swap(LESet<Key>& set) {
		this->m_bucket.swap(set.m_bucket);
	}

	LE_FORCEINLINE uint32_t getBucketCount()const {
		return this->m_bucket.getLength();
	}

	template<class Archive>
	void serialize(Archive& ar, const unsigned int file_version) {
		ar& this->m_bucket;
	}
};

LE_NAMESPACE_END

#endif // !LE_SET_H

