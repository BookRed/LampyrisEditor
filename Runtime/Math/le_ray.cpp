/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Math
 * \File:    le_ray.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_ray.h"

LERay::LERay():LELine(glm::vec3{ 0,0,0 }, 
	                  glm::vec3{ 0,0,0 }) {}

LERay::LERay(const glm::vec3& dir, const glm::vec3& origin) :
	LELine(glm::normalize(dir), origin) {}

glm::vec3 LERay::getPoint(float distance) const {
	return this->m_origin + this->m_direction * distance;
}
