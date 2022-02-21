#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Common
 * \File:    le_destoryable.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_DESTORYABLE_H
#define LE_DESTORYABLE_H

#include "le_global.h"

LE_NAMESPACE_BEGIN(Common)

interface Destoryable {
public:
	virtual void destoryNow() = 0;
	
	virtual void destoryAfter(LEFloat waitTimeMilliSecond) = 0;
};
LE_NAMESPACE_END
#endif // !LE_DESTORYABLE_H
