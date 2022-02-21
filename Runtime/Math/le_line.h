#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Math
 * \File:    le_line.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_LINE
#define LE_LINE

// LE Includes
#include "le_math_global.h"

class LELine;
class LERay;
class LESegment;
class LEAABB;

// the equation of line is;
// P = P0 + dir * t
class LELine {
protected:
	glm::vec3 m_direction; 
	glm::vec3 m_origin; // P0
public:
	          LELine();
	          
	          LELine(const glm::vec3& dir, const glm::vec3& origin);
	          
	          LELine(const LELine& other);
	          
	          LELine& operator = (const LELine& other);

	// Given a point P, find the value of the parameter t 
	// if P is not on the line, return false 
	bool      getParameter(const glm::vec3& point, float t);

	glm::vec3 getPoint(float t);

	float     squaredDistance(const glm::vec3& point, float& t) const;
		      
	float     squaredDistance(const LELine& line, float& t1, float& t2) const;
		      
	float     squaredDistance(const LERay& ray, float& t1, float& t2);
		      
	float     squaredDistance(const LESegment& segment, float& t1, float& t2);
		      
	float     squaredDistance(const LEAABB& aabb, float& t1, float tAABB[3]);
};
#endif // !LE_LINE