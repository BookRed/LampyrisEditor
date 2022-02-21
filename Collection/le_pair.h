#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Collection
 * \File:    le_pair.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_PAIR_H
#define LE_PAIR_H

// LE-Includes
#include "../common/le_global.h"
#include "../common/le_serializable.h"
LE_NAMESPACE_BEGIN(Collection)

LE_USING_NAMESPACE(Common);
template<typename T1, typename T2>
struct LEPair:extend Serializable{
	using FirstType  = T1;
	using SecondType = T2;

	T1    first;
	T2    second;

	LEPair(T1 _first = T1(), T2 _second = T2()):first(_first),
		                                        second(_second) {}

	template<class Archive>
	void serialize(Archive& ar, const unsigned int file_version) {
		ar& this->first;
		ar& this->second;
	}
};

LE_NAMESPACE_END

#endif // !LE_PAIR_H

