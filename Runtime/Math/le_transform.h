#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Math
 * \File:    le_transform.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_TRANSFORM_H
#define LE_TRANSFORM_H

// LE Includes
#include <glm/gtx/matrix_decompose.hpp>

#include "../Base/le_component.h"
#include "../Math/le_math_global.h"
#include "Runtime/Base/le_entity.h"

class LETransform :public LEComponent {
	LE_COMPONENT_NAME(Transform);
private:
	// local properties
	glm::vec3       m_localPosition;
	glm::qua<float> m_localRotation;
	glm::vec3       m_localScale;
	glm::vec3       m_localEulerAngle;
	glm::mat4       m_localMatrix;

	// world properties
	// we cache all the world properties to reduce repeated world coordinate calculation
	glm::vec3       m_worldPosition;
	glm::qua<float> m_worldRotation;
	glm::vec3       m_worldScale;
	glm::vec3       m_worldEulerAngle;
	glm::mat4       m_worldMatrix;

	bool            m_isChanged;

	bool            hasScale(const glm::mat3& m);

	void            composeMatrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, glm::mat4& m);
private:
	float           inverseFloat(float f);
			        
	glm::vec3       inverseVector(const glm::vec3& vec);
public:		        
	                LETransform* getParent();
	                
	                LETransform();
	                
	                LETransform(float x, float y, float z);
			        
	virtual        ~LETransform();
	
	// local properties getters
	glm::vec3       getLocalPosition();

	glm::qua<float> getLocalRotation();

	glm::vec3       getLocalScale();

	glm::mat4       getLocalMatrix();

	glm::vec3       getLocalEulerAngle();

	// world properties getters
	glm::vec3       getWorldPosition();

	glm::qua<float> getWorldRotation();

	glm::vec3       getWorldScale();

	glm::mat4&      getWorldMatrix();

	glm::vec3       getWorldEulerAngle();

	// properties setters
	void            setLocalPosition(glm::vec3 vec);

	void            setLocalPosition(float x, float y = 0.0f, float z = 0.0f);
			        
	void            setLocalRotation(const glm::qua<float>& q);
			        
	void            setLocalScale(float x, float y, float z);

	void            setLocalScale(glm::vec3 vec);

	void            setLocalEulerAngle(float x, float y, float z);

	void            setLocalEulerAngle(glm::vec3 vec);

	void            setWorldPosition(glm::vec3 vec);

	void            setWorldPosition(float x, float y = 0.0f, float z = 0.0f);

	void            setWorldRotation(const glm::qua<float>& q);

	void            setWorldScale(float x, float y, float z);

	void            setWorldScale(glm::vec3 vec);

	void            setWorldEulerAngle(float x, float y, float z);

	void            setWorldEulerAngle(glm::vec3 vec);
	// operations
	void            translate(glm::vec3 translation);

	void            rotateAroundSelf(glm::vec3 eulerAngles);
			        
	void            rotateInWorld(glm::vec3 eulerAngles);
			        
	void            rotateAroundAxis(LETransform& centerTransform, glm::vec3& axis, float angle);

	void            updateLocalProperties();

	// It will be called after transform is changed
	void            update(bool isLocalChanged = false);

	void            onParentChanged();

	glm::vec3       getForwardDirection() const;
};

#endif // !LE_TRANSFORM_H
