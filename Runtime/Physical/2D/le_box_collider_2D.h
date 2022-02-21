#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Physical2D
 * \File:    le_box_collider_2D.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_collider_2D.h"

class LEBoxCollider2D:public LECollider2D {
public:
	LEBoxCollider2D();

	virtual ~LEBoxCollider2D();
};
