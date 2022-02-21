#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Collection
 * \File:    le_map.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_MAP_H
#define LE_MAP_H

#define LE_MAP_TYPENAMES  typename Key, typename Value, class HashFunction
#define LE_MAP_TYPENAMES_WITH_DEFAULT LE_MAP_TYPENAMES = LEHash<Key>
#define LE_MAP_PAIR  Key,Value
#define LE_MAP_TYPES Key,Value,HashFunction


// LE-Includes
#include "le_pair.h"
#include "le_node.h"
#include "le_vector.h"
#include "../common/le_global.h"
#include "../common/le_hash.h"

LE_NAMESPACE_BEGIN(Collection)

template<LE_MAP_TYPENAMES_WITH_DEFAULT>
class LEHashBucket;

template<LE_MAP_TYPENAMES_WITH_DEFAULT>
class LEMap;

LE_USING_DETAIL_NAMESPACE(Collection);

template<LE_MAP_TYPENAMES>
class LEMapIterator {

	typename typedef LEPair<Key, Value>              NodeElement;
	typename typedef LENode<NodeElement>             Node;
	typename typedef LEMapIterator<LE_MAP_TYPES>     Self;
	typename typedef LEHashBucket<LE_MAP_TYPES>      Bucket;
private:
	Node*              m_pNode;
	Bucket*            m_pBucket;
public:
	LEMapIterator(Node* pNode = nullptr, 
		          Bucket* pBucket = nullptr):
		          m_pNode(pNode),
			      m_pBucket(pBucket){}
	
	Value& operator*() {
		return this->m_pNode->data.second;
	}

	Value* operator->() {
		return &(this->m_pNode->data.second);
	}
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
				if (this->m_pBucket[bucketNo]!=nullptr) {
					this->m_pNode = m_pBucket->m_nodes[bucketNo];
					return;
				}
			}
			m_pBucket = nullptr;
		}
	}
};

template<LE_MAP_TYPENAMES>
class LEHashBucket :extend Serializable {
	typename typedef LEPair<LE_MAP_PAIR>          NodeElement;
	typename typedef LENode<NodeElement>          Node;
	typename typedef LEHashBucket<LE_MAP_TYPES>   Self;
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
					uint32_t newBucketNo = HashFunction()(pNode->data.first)% size;
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
	typename typedef LEMapIterator<LE_MAP_TYPES>  Iterator;
	LEHashBucket(uint32_t capacity = 16) :m_length(0), m_nodes(LEVector<Node*>(capacity)) {
	
	}
	~LEHashBucket() {

	}
	LEBool contains(const Key& key) {
		return this->locateNode(key) != nullptr;
	}
	LEPair<Iterator,LEBool> insert(const Key& key, const Value& value) {
		this->expandIfNeeded();
		uint32_t bucketNo = HashFunction()(key) % this->m_nodes.getCapacity();;
		Node* pNode = this->locateNode(key);
		while (pNode != nullptr) {
			if (pNode->data.first == key) {
				// already exists
				return LEPair<Iterator, LEBool>(Iterator(pNode, this), false);
			}
			pNode = pNode->next;
		}
		pNode = new Node(LEPair(key, value));
		pNode->next = m_nodes[bucketNo];
		this->m_nodes[bucketNo] = pNode;
		this->m_length++;
		return LEPair<Iterator, LEBool>(Iterator(pNode, this), true);
	}

	Node* locateNode(const Key& key) {
		uint32_t bucketNo = HashFunction()(key) % this->m_nodes.getCapacity();
		Node* pNode = this->m_nodes[bucketNo];
		while (pNode != nullptr) {
			if (pNode->data.first == key)
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
			if (pNode->data.first == key) {
				Iterator iter(pNode,this);
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
		LENode<LEPair<Key, Value>>* pNode = this->m_nodes[bucketNo];
		LENode<LEPair<Key, Value>>* pPrev = nullptr;
		while (pNode) {
			if (pNode->data.first == key) {
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
		return Iterator(nullptr,this);
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

template<LE_MAP_TYPENAMES>
class LEMap :extend Serializable {
	typename typedef LEHashBucket<LE_MAP_TYPES>::Iterator Iterator;
	typename typedef LEMap<LE_MAP_TYPES>                  Self;
private:
	LEHashBucket<LE_MAP_TYPES> m_bucket;
public:
	LEMap() : m_bucket() { }

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

	LE_FORCEINLINE LEPair<Iterator,LEBool> insert(const Key& key, const Value& value) {
		return this->m_bucket.insert(key, value);
	}

	LE_FORCEINLINE LEBool erase(const Key& key) {
		return this->m_bucket.erase(key);
	}

	LE_FORCEINLINE void clear() {
		this->m_bucket.clear();
	}

	LE_FORCEINLINE void swap(LEMap<Key, Value>& map) {
		this->m_bucket.swap(map.m_bucket);
	}
	Value& operator[](const Key& key) {
		return *(this->insert(key, Value())).first;
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

#endif // !LE_MAP_H

