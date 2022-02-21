#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Common
 * \File:    le_cloneable.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_CLONEABLE_H
#define LE_CLONEABLE_H

#include "le_global.h"

LE_NAMESPACE_BEGIN(Common)

template <class T>
interface Cloneable {
public:
	virtual T clone() = 0;
};
LE_NAMESPACE_END
#endif // !LE_CLONEABLE_H
