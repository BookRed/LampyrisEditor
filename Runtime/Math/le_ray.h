#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Math
 * \File:    le_ray.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_RAY_H
#define LE_RAY_H

// LE Includes
#include "le_line.h"

// GLM Includes
#include <glm/glm.hpp>

class LERay:public LELine {
public:
	          LERay();
	          
	          LERay(const glm::vec3& dir, 
		            const glm::vec3& origin);

	glm::vec3 getPoint(float distance) const;
};

#endif // !LE_RAY_H

