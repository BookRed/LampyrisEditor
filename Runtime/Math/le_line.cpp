/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Math
 * \File:    le_line.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#include "le_line.h"
#include "le_ray.h"
#include "le_segment.h"
#include "le_AABB.h"

LELine::LELine() {

}

LELine::LELine(const glm::vec3& dir, const glm::vec3& origin) :
	m_direction(glm::normalize(dir)),
	m_origin(origin) {

}

LELine::LELine(const LELine& other) {
	this->m_direction = other.m_direction;
	this->m_origin = other.m_origin;
}

bool LELine::getParameter(const glm::vec3& point, float t) {
	glm::vec3 vec = point - this->m_origin;
	// is parallel?
	if (LE_FLOAT_ZERO(glm::dot(vec, this->m_direction))) {
		t = vec.length();
		return true;
	}
	else {
		return false;
	}
}

LELine& LELine::operator = (const LELine& other) {
	this->m_direction = other.m_direction;
	this->m_origin = other.m_origin;
	return *this;
}

glm::vec3 LELine::getPoint(float t) {
	return this->m_origin + t * this->m_direction;
}

float LELine::squaredDistance(const glm::vec3& point, float& t) const {
	glm::vec3 vec = point - this->m_origin;
	// projection
	t = glm::dot(vec, this->m_direction);
	vec -= this->m_direction * t;
	return glm::dot(vec, vec);
}

float LELine::squaredDistance(const LELine& line, float& t1, float& t2) const {
	float result;
	glm::vec3 vec = this->m_origin - line.m_origin;

	float v0 = -glm::dot(this->m_direction, line.m_direction);  // a01
	float v1 = glm::dot(vec, this->m_direction);                 // b0
	float v2 = glm::dot(vec, vec);                               // c
	float v3 = 1 - v0 * v0;                                      // det

	if (!LE_FLOAT_ZERO(v3)) {
		float v4 = -glm::dot(vec, line.m_direction); // b1
		float v5 = 1 / v3;                            // invDet
		t1 = (v0 * v4 - v1) * v5;               // fLine1P
		t2 = (v0 * v1 - v4) * v5;               // fLine2P
		result = t1 * (t1 + v0 * t2 + 2 * v1) + t2 * (v0 * t1 + t2 + 2 * v4) + v2;
	}
	else {
		t1 = -v1;            // fLine1P
		t2 = 0;              // fLine2P
		result = v1 * t1 + v2;
	}
	return result;
}

float LELine::squaredDistance(const LERay& ray, float& t1, float& t2) {
	LELine line = ray;
	float dist = this->squaredDistance(line, t1, t2);
	if (t2 < 0) {
		t2 = 0;
		dist = this->squaredDistance(ray.m_origin, t1);
	}
	return dist;
}

float LELine::squaredDistance(const LESegment& segment, float& t1, float& t2) {
	const LERay ray = segment;
	float dist = this->squaredDistance(ray, t1, t2);
	if (t2 > segment.getLength()) {
		glm::vec3 endPoint = segment.getEndPoint();
		t2 = segment.getLength();
		dist = this->squaredDistance(endPoint, t1);
	}
	return dist;
}

float LELine::squaredDistance(const LEAABB& aabb, float& t1, float tAABB[3]) {
	return 0.0f;//aabb.squaredDistanceWith(aabb, t1, tAABB);
}
