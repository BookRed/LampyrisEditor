#include "le_AABB.h"

void LEAABB::updateCenter() {
	this->m_center = { (this->m_min.x + this->m_max.x) / 2.0f,
		(this->m_min.y + this->m_max.y) / 2.0f,
		(this->m_min.z + this->m_max.z) / 2.0f };
}

LEAABB::LEAABB() {}

LEAABB::LEAABB(const LEAABB& other) {
	this->m_min = other.m_min;
	this->m_max = other.m_max;
	this->updateCenter();
}

LEAABB::LEAABB(const glm::vec3& min, const glm::vec3& max)
	:m_min(min), m_max(max) {}

LEAABB::LEAABB(const std::vector<glm::vec3>& points) :
	m_min(INFINITY, INFINITY, INFINITY),
	m_max(-INFINITY, -INFINITY, -INFINITY) {

	if (points.empty()) {
		this->m_max = this->m_min = { 0,0,0 };
		return;
	}
	for (auto& pt : points) {
		this->m_min.x = std::min(this->m_min.x, pt.x);
		this->m_min.y = std::min(this->m_min.y, pt.y);
		this->m_min.z = std::min(this->m_min.z, pt.z);
		this->m_max.x = std::max(this->m_max.x, pt.x);
		this->m_max.y = std::max(this->m_max.y, pt.y);
		this->m_max.z = std::max(this->m_max.z, pt.z);
	}
}

LEAABB& LEAABB::operator = (const LEAABB& other) {
	this->m_min = other.m_min;
	this->m_max = other.m_max;
	this->updateCenter();
	return *this;
}

void LEAABB::setMin(const glm::vec3& min) {
	this->m_min = min;
	this->updateCenter();
}

glm::vec3 LEAABB::getMin() const {
	return this->m_min;
}

void LEAABB::setMax(const glm::vec3& max) {
	this->m_max = max;
	this->updateCenter();
}

glm::vec3 LEAABB::getMax() const {
	return this->m_max;
}

LEAABB LEAABB::merge(const LEAABB& other) {
	LEAABB aabb = *this;
	aabb.m_min.x = std::min(aabb.m_min.x, other.m_min.x);
	aabb.m_min.y = std::min(aabb.m_min.y, other.m_min.y);
	aabb.m_min.z = std::min(aabb.m_min.z, other.m_min.z);
	aabb.m_max.x = std::max(aabb.m_max.x, other.m_max.x);
	aabb.m_max.y = std::max(aabb.m_max.y, other.m_max.y);
	aabb.m_max.z = std::max(aabb.m_max.z, other.m_max.z);
	aabb.updateCenter();
	return aabb;
}

glm::vec3 LEAABB::getCenter() const {
	return this->m_center;
}

float LEAABB::squaredDistance(const glm::vec3& point) const {
	return 0.0f;
}

bool LEAABB::containsPoint(const glm::vec3& points) const {
	return false;
}

bool LEAABB::intersetRay(const LERay& ray) const {
	return false;
}

bool LEAABB::intersetAABB(const LEAABB& aabb) const {
	if ((this->m_min.x > aabb.m_max.x || this->m_max.x < aabb.m_min.x) ||
		(this->m_min.y > aabb.m_max.y || this->m_max.y < aabb.m_min.y) ||
		(this->m_min.z > aabb.m_max.z || this->m_max.z < aabb.m_min.z)) {
		return false;
	}
	else {
		return true;
	}
}
