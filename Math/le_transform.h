#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  GamePlay
 * \File:    le_game_object.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_TRANSFORM_H
#define LE_TRANSFORM_H
// LE Includes
#include "Runtime/Base/le_component.h"
// GLM Includes
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_common.hpp>

class LEEntity; 
class LETransform:public LEComponent {
private:
	inline float inverseFloat(float f) {
		if (std::abs(f) > 10e-5) {
			return 1.0f / f;
		}
		else {
			return 0;
		}
	}

	inline glm::vec3 inverseVector(const glm::vec3& vec) {
		return glm::vec3(inverseFloat(vec.x), 
			             inverseFloat(vec.y), 
			             inverseFloat(vec.z));
	}
public:
	glm::vec3       m_localPosition;
	glm::qua<float> m_localRotation;
	glm::vec3       m_localScale;

	LETransform& getParent() {
		return *(this->m_pAttachedEntity->getTransform());
	}
	LETransform(): m_localPosition(glm::vec3(0, 0, 0)),
		           m_localRotation(glm::qua<float>()),
		           m_localScale   (glm::vec3(1, 1, 1)) {}

	LETransform(float x, float y, float z): 
				   m_localPosition(glm::vec3(x,y,z)),
		           m_localRotation(glm::qua<float>()),
		           m_localScale   (glm::vec3(1, 1, 1)) {}

	void   setLocalPosition(float x, float y = 0.0f, float z = 0.0f) {
		this->m_localPosition = glm::vec3(x, y, z);
	}

	glm::vec3  getLocalPosition() const {
		return this->m_localPosition;
	}

	void  setLocalRotation(const glm::qua<float>& q) {
		this->m_localRotation = q;
	}
	
	void   rotateAroundSelf(glm::vec3 &eulerAngles) {
		this->m_localRotation = eulerAngles;
		// create from eulerAngles
		glm::qua quaternion(eulerAngles); 
		// update local rotation
		this->m_localRotation = quaternion * this->m_localRotation;
	}
	void   rotateInWorld(glm::vec3 & eulerAngles) {
		glm::qua q1(eulerAngles);
		glm::qua q2 = glm::inverse(q1);
		this->m_localRotation = q1 * q2 * this->m_localRotation;
	}
	void   rotateAroundAxis(LETransform& centerTransform, glm::vec3& axis, float angle) {
		glm::vec3 axisLocal = inverseVector(axis);
		glm::qua q = glm::qua(glm::radians(angle), axisLocal);
		this->m_localRotation = glm::normalize(this->m_localRotation * q);
	}
};
#endif // !LE_TRANSFORM_H
