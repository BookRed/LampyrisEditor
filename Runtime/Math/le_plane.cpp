/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Math
 * \File:    le_plane.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_plane.h"

LEPlane::LEPlane():m_normal(0,0,0),m_point(0,0,0) {}

LEPlane::LEPlane(const glm::vec3& normal, const glm::vec3& point) :
	m_normal(normal),
	m_point(point) {}

LEPlane::LEPlane(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2) {
	glm::vec3 v10 = p1 - p0;
	glm::vec3 v21 = p2 - p1;

	glm::vec3 normal = glm::cross(v10, v21);
	normal = glm::normalize(normal);

	this->m_normal = normal;
	this->m_point = p0;
}

float LEPlane::distance(const glm::vec3& point) {
	float numerator = glm::abs(this->m_normal.x * point.x +
		this->m_normal.y * point.y +
		this->m_normal.z * point.z +
		this->m_D);

	// because of m_normal is normalized, so the denominator is always one
	return numerator;
}
