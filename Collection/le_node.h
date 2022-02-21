#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Collection
 * \File:    le_node.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_NODE_H
#define LE_NODE_H

// LE-Includes
#include "../common/le_global.h"
#include "../common/le_serializable.h"

LE_DETAIL_NAMESPACE_BEGIN(Collection)

LE_USING_NAMESPACE(Common);
template<typename T>
class LENode:extend Serializable{
public:
	LENode(const T& _data = T()):next(nullptr),
		                         data(_data) {}
	LENode<T> *next;
	T          data;

	template <class Archive>
	void serialize(Archive& ar, const unsigned int file_version) {
		ar& this->next;
		ar& this->data;
	}
};

LE_DETAIL_NAMESPACE_END
#endif // !LE_NODE_H

