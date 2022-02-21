#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Math
 * \File:    le_AABB.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_AABB_H
#define LE_AABB_H

// STL Includes
#include <vector>
#include <array>
// GLM Includes
#include <glm/glm.hpp>

class LERay;

class LEAABB {
private:
	glm::vec3 m_min;
	glm::vec3 m_max;
	glm::vec3 m_center;

	void      updateCenter();
public:
	          LEAABB();
	          
	          LEAABB(const LEAABB& other);

	          LEAABB& operator = (const LEAABB& other);
	          
	          LEAABB(const glm::vec3& min, const glm::vec3& max);
	          
	          LEAABB(const std::vector<glm::vec3>& points);

	void      setMin(const glm::vec3& min);

	void      setMax(const glm::vec3& max);

	glm::vec3 getMin() const;

	glm::vec3 getMax() const;

	LEAABB    merge(const LEAABB& other);

	glm::vec3 getCenter() const;

	float     squaredDistance(const glm::vec3& point) const;

	bool      containsPoint(const glm::vec3& points) const;
		      
	bool      intersetRay(const LERay& ray) const;
		      
	bool      intersetAABB(const LEAABB& aabb) const;

	std::array<glm::vec3,8> getVertices() const {
		std::array<glm::vec3, 8> vertices;
		// + X + Y + Z
		vertices[0] = { this->m_max.x,this->m_max.y,this->m_max.z };
		// + X + Y - Z
		vertices[1] = { this->m_max.x,this->m_max.y,this->m_min.z };
		// + X - Y + Z
		vertices[2] = { this->m_max.x,this->m_min.y,this->m_max.z };
		// + X - Y - Z
		vertices[3] = { this->m_max.x,this->m_min.y,this->m_min.z };
		// - X + Y + Z
		vertices[4] = { this->m_min.x,this->m_max.y,this->m_max.z };
		// - X + Y - Z
		vertices[5] = { this->m_min.x,this->m_max.y,this->m_min.z };
		// - X - Y + Z
		vertices[6] = { this->m_min.x,this->m_min.y,this->m_max.z };
		// - X - Y - Z
		vertices[7] = { this->m_min.x,this->m_min.y,this->m_min.z };
		return vertices;
	}
};

#endif // !LE_AABB_H