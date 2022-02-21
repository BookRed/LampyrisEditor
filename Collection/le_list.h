#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Collection
 * \File:    le_list.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_LIST_H
#define LE_LIST_H

// LE Includes
#include "le_collection_global.h"
#include "../Common/le_serializable.h"

LE_NAMESPACE_BEGIN(Collection)

template<typename T>
class LEList :public LEObject, public Serializable {
	typedef LENode<T> Node;
private:
	Node*     m_pHead;
	Node*     m_pRear;
	uint32_t    m_length;
public:
	LEList(): m_pHead(nullptr), 
		      m_pRear(nullptr), 
		      m_length(0) {}

	~LEList() {
		Node* pCurr = this->m_pHead;
		for (uint32_t i = 0; i < this->m_length; i++) {
			Node* pNext = this->m_pHead->next;
			LE_SAFE_DELETE(pCurr);
			pCurr = pNext;
		}
	}
 	void insertFront(const T &t) {
		if (!this->m_pHead) {
			this->m_pHead = new Node(t);
			this->m_pRear = this->m_pHead;
		}
		else {
			Node * pHead = new Node(t);
			pHead->next = this->m_pHead;
			this->m_pHead = pHead;
		}
	}

	void insertBack(const T &t) {
		if (!this->m_pHead) {
			this->m_pHead = new Node(t);
			this->m_pRear = this->m_pHead;
		}
		else {
			LE_ASSERT(this->m_pRear != nullptr);
			Node* pRear = new Node(t);
			this->m_pRear->next = pRear;
			this->m_pRear = pRear;
		}
	}

	void remove(const T &t) {
		if (!this->m_pHead) {
			return;
		}
		Node* pPrev = nullptr;
		Node* pCurr = this->m_pHead;
		for (uint32_t i = 0; i < this->m_length; i++) {
			if (pCurr->data == t) {
				pPrev->next = pCurr->next;
				LE_SAFE_DELETE(pCurr);
				return;
			}
			pPrev = pCurr;
			pCurr = pCurr->next;
		}
	}

	Node* getHead() {
		return this->m_pHead;
	}

	Node* getRear() {
		return this->m_pRear;
	}

	uint32_t getLength() const {
		return this->m_length;
	}
};

LE_NAMESPACE_END
#endif // !LE_LIST_H