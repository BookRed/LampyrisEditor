#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Common
 * \File:    le_serializable.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_SERIALIZABLE_H
#define LE_SERIALIZABLE_H

// LE Includes
#include "le_global.h"
#include "../Serialization/le_serialization.h"

interface Serializable {
	LE_DECLARE_SERIALIZABLE
};
#endif // !LE_SERIALIZABLE_H
