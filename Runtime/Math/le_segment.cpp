/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Math
 * \File:    le_segment.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_segment.h"

LESegment::LESegment() :LERay() {

}

LESegment::LESegment(const glm::vec3& dir, 
	                 const glm::vec3& origin, 
	                 float length)
	: LERay(dir, origin) {}

LESegment::LESegment(const glm::vec3& origin, const glm::vec3& end)
	: LERay(glm::normalize(end - origin), origin),
	m_end_point(end),
	m_length(glm::distance(origin, end)) {}

LESegment::LESegment(const LESegment& other) {
	this->m_direction = other.m_direction;
	this->m_origin = other.m_origin;
	this->m_length = other.m_length;
	this->m_end_point = other.m_end_point;
}

LESegment& LESegment::operator = (const LESegment& other) {
	this->m_direction = other.m_direction;
	this->m_origin = other.m_origin;
	this->m_length = other.m_length;
	this->m_end_point = other.m_end_point;
	return *this;
}

float LESegment::getLength() const {
	return this->m_length;
}

glm::vec3 LESegment::getEndPoint() const {
	return this->m_end_point;
}
