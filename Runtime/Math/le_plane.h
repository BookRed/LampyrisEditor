#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Math
 * \File:    le_plane.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_PLANE_H
#define LE_PLANE_H

// GLM Includes
#include <glm/glm.hpp>

/*
   There are two ways to determine a plane:
1) The coordinates of the three points
2) The normal vector of the plane and one point
*/

// Ax + By + Cz + D = 0
class LEPlane {
private:
	glm::vec3 m_normal;
	glm::vec3 m_point;
	float     m_D;        // the distance to the origin point
public:
	      LEPlane();

	      LEPlane(const glm::vec3& normal, const glm::vec3& point);
	      
	      LEPlane(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);

	float distance(const glm::vec3& point);

	bool  isPointInUpperSide(const glm::vec3& point) const{
		const float a = this->m_normal.x;
		const float b = this->m_normal.y;
		const float c = this->m_normal.z;
		const float x0 = this->m_point.x;
		const float y0 = this->m_point.y;
		const float z0 = this->m_point.z;
		const float d = -(a * x0 + b * y0 + c * z0);
		return (a * point.x + b * point.y + c * point.z + d) > 0;
	}
};

#endif // !LE_PLANE_H