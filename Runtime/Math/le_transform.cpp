/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Math
 * \File:    le_transform.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_transform.h"
#include "../Base/le_entity.h"
#include "../Base/le_assert.h"
// GLM Includes
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/matrix_operation.hpp>
bool LETransform::hasScale(const glm::mat3& m) {
	// If the columns are orthonormal and form a right-handed system][then there is no scale
	float det = glm::determinant(m);
	if (!LE_FLOAT_EQUAL(det, 1.0f)) {
		return true;
	}
	det = m[0][0] * m[0][0] + m[1][0] * m[1][0] + m[2][0] * m[2][0];
	if (!LE_FLOAT_EQUAL(det, 1.0f)) {
		return true;
	}
	det = m[0][1] * m[0][1] + m[1][1] * m[1][1] + m[2][1] * m[2][1];
	if (!LE_FLOAT_EQUAL(det, 1.0f)) {
		return true;
	}
	det = m[0][2] * m[0][2] + m[1][2] * m[1][2] + m[2][2] * m[2][2];
	if (!LE_FLOAT_EQUAL(det, 1.0f)) {
		return true;
	}
	return false;
}

void LETransform::composeMatrix(const glm::vec3& position, 
	                            const glm::vec3& rotation, 
	                            const glm::vec3& scale, 
	                            glm::mat4& m) {
	m = glm::identity<glm::mat4>();
	m = glm::translate(m, position);
	m = glm::rotate(m, rotation.x, { 1,0,0 });
	m = glm::rotate(m, rotation.y, { 0,1,0 });
	m = glm::rotate(m, rotation.z,{0,0,1});
	m = glm::scale(m, scale);
	return;
	const glm::mat3 rot3x3(this->m_localRotation);
	// row 0
	m[0][0] = scale.x * rot3x3[0][0];
	m[0][1] = scale.y * rot3x3[0][1];
	m[0][2] = scale.z * rot3x3[0][2];
	m[0][3] = position.x;
	// row 1
	m[1][0] = scale.x * rot3x3[1][0];
	m[1][1] = scale.y * rot3x3[1][1];
	m[1][2] = scale.z * rot3x3[1][2];
	m[1][3] = position.y;
	// row 2
	m[2][0] = scale.x * rot3x3[2][0];
	m[2][1] = scale.y * rot3x3[2][1];
	m[2][2] = scale.z * rot3x3[2][2];
	m[2][3] = position.z;

	// no projection term
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

float LETransform::inverseFloat(float f) {
	if (std::abs(f) > 10e-5) {
		return 1.0f / f;
	}
	else {
		return 0;
	}
}

glm::vec3 LETransform::inverseVector(const glm::vec3& vec) {
	return glm::vec3(inverseFloat(vec.x),
		inverseFloat(vec.y),
		inverseFloat(vec.z));
}

LETransform* LETransform::getParent() {
	if (this->m_pAttachedEntity->getParent()) {
		return this->m_pAttachedEntity->getParent()->getTransform();
	}
	return nullptr;
}

LETransform::LETransform(){
	new (this)LETransform(0, 0, 0);
}

LETransform::LETransform(float x, float y, float z) :
	m_localPosition  (glm::vec3(x, y, z)),
	m_localRotation  (glm::qua<float>({0,0,0})),
	m_localScale     (glm::vec3(1, 1, 1)),
	m_worldPosition(glm::vec3(x, y, z)),
	m_worldRotation(glm::qua<float>({ 0,0,0 })),
	m_worldScale(glm::vec3(1, 1, 1)),
	m_worldMatrix    (glm::identity<glm::mat4>()),
	m_localMatrix    (glm::identity<glm::mat4>()),
	m_localEulerAngle(glm::vec3(0, 0, 0)),
	m_worldEulerAngle(glm::vec3(0,0,0)),
	m_isChanged(true) {}

LETransform::~LETransform() {}

// get locals
glm::vec3 LETransform::getLocalPosition(){
	return this->m_localPosition;
}

glm::qua<float> LETransform::getLocalRotation(){
	return this->m_localRotation;
}

glm::vec3 LETransform::getLocalScale(){
	return this->m_localScale;
}

glm::mat4 LETransform::getLocalMatrix() {
	if (this->m_isChanged) {
		this->m_localMatrix = glm::identity<glm::mat4>();
		this->m_localMatrix = glm::translate(this->m_localMatrix, this->m_localPosition);
		glm::vec3 eular = this->m_localEulerAngle;
		this->m_localMatrix = glm::rotate(this->m_localMatrix, glm::radians(eular.x), { 1,0,0 });
		this->m_localMatrix = glm::rotate(this->m_localMatrix, glm::radians(eular.y), { 0,1,0 });
		this->m_localMatrix = glm::rotate(this->m_localMatrix, glm::radians(eular.z), { 0,0,1 });
		this->m_localMatrix = glm::scale(this->m_localMatrix, this->m_localScale);
		this->m_isChanged = false;
	}
	return this->m_localMatrix;
}

glm::vec3 LETransform::getLocalEulerAngle() {
	return this->m_localEulerAngle;
}

glm::vec3 LETransform::getWorldEulerAngle() {
	return glm::degrees(this->m_worldEulerAngle);
}

void LETransform::setLocalPosition(glm::vec3 vec) {
	this->setLocalPosition(vec.x, vec.y, vec.z);
}

void LETransform::setLocalScale(glm::vec3 vec) {
	this->setLocalScale(vec.x, vec.y, vec.z);
}

void LETransform::setLocalEulerAngle(glm::vec3 vec) {
	this->setLocalEulerAngle(vec.x, vec.y, vec.z);
}

void LETransform::setWorldPosition(glm::vec3 vec) {
	if (vec != this->m_worldPosition) {
		this->m_worldPosition = vec;
		this->updateLocalProperties();
	}
}

void LETransform::setWorldPosition(float x, float y, float z) {
	if (glm::vec3(x, y, z) != this->m_worldPosition) {
		this->m_worldPosition = { x,y,z };
		this->updateLocalProperties();
	}
}

void LETransform::setWorldRotation(const glm::qua<float>& q) {
	if (q != this->m_worldRotation) {
		this->m_worldRotation = q;
		this->updateLocalProperties();
	}
}

void LETransform::setWorldScale(float x, float y, float z) {
	if (glm::vec3(x, y, z) != this->m_worldScale) {
		this->m_worldScale = { x,y,z };
		this->updateLocalProperties();
	}
}

void LETransform::setWorldScale(glm::vec3 vec) {
	if (this->m_worldScale != vec) {
		this->m_worldScale = vec;
		this->updateLocalProperties();
	}
}

void LETransform::setWorldEulerAngle(float x, float y, float z) {
	glm::vec3 radianEularAngle = glm::radians(glm::vec3(x,y,z));
	if (radianEularAngle != this->m_worldEulerAngle) {
		this->m_worldEulerAngle = radianEularAngle;
		this->m_worldRotation = glm::qua<float>(radianEularAngle);
		this->updateLocalProperties();
	}
}

void LETransform::setWorldEulerAngle(glm::vec3 vec) {
	glm::vec3 radianEularAngle = glm::radians(vec);
	if (radianEularAngle != this->m_worldEulerAngle) {
		this->m_worldEulerAngle = radianEularAngle;
		this->m_worldRotation = glm::qua<float>(radianEularAngle);
		this->updateLocalProperties();
	}
}

// get world's
glm::vec3 LETransform::getWorldPosition() {
	return this->m_worldPosition;
}

glm::qua<float> LETransform::getWorldRotation(){
	return this->m_worldRotation;
}

glm::vec3 LETransform::getWorldScale(){
	return this->m_worldScale;
}

glm::mat4& LETransform::getWorldMatrix() {
	if (this->m_isChanged) {
		glm::mat4 mat     = this->getLocalMatrix();
		LEEntity* pEntity = this->m_pAttachedEntity;
		while(pEntity->getParent() != nullptr) {
			mat = mat * pEntity->getParent()->getTransform()->getWorldMatrix();
		}
		if (pEntity != nullptr && pEntity->getParent() != nullptr) {
			mat = mat * pEntity->getParent()->getTransform()->getWorldMatrix();
		}
		this->m_worldMatrix = this->m_localMatrix;
	}
	return this->m_worldMatrix;
}

// setters
void LETransform::setLocalPosition(float x, float y, float z) {
	if (glm::vec3(x, y, z) != this->m_localPosition) {
		this->m_localPosition = glm::vec3(x, y, z);
		this->update(true);
	}
}

void LETransform::setLocalRotation(const glm::qua<float>& q) {
	if (q != this->m_localRotation) {
		this->m_localRotation = q;
		this->m_localEulerAngle = glm::eulerAngles(q);
		this->update(true);
	}
}

void LETransform::setLocalScale(float x, float y, float z) {
	if (glm::vec3(x, y, z) != this->m_localScale) {
		this->m_localScale = glm::vec3(x, y, z);
		this->update(true);
	}
}

void LETransform::setLocalEulerAngle(float x, float y, float z) {
	glm::vec3 radianEularAngle = glm::radians(glm::vec3(x, y, z));
	if (radianEularAngle != this->m_localEulerAngle) {
		this->m_localEulerAngle = radianEularAngle;
		this->m_localRotation = glm::qua<float>(radianEularAngle);
		this->update(true);
	}
}

// operations
void LETransform::translate(glm::vec3 translation) {
	this->m_localPosition += translation;
	this->update(true);
} 

void LETransform::rotateAroundSelf(glm::vec3 eulerAngles) {
	// create from eulerAngles
	glm::qua quaternion(eulerAngles/2.0f);
	// update local rotation
	// this->m_localRotation = quaternion * this->m_localRotation;
	// this->m_localEulerAngle = glm::eulerAngles(this->m_localRotation);

	this->m_localEulerAngle += eulerAngles;
	// this->m_localRotation = glm::qua(this->m_localEulerAngle);
	this->update(true);
}

void LETransform::rotateInWorld(glm::vec3 eulerAngles) {
	glm::qua q1(eulerAngles);
	glm::qua q2 = glm::inverse(q1);
	this->m_localRotation = q1 * this->m_localRotation* q2;
	this->m_localEulerAngle = glm::eulerAngles(this->m_localRotation);
	this->update();
}

void LETransform::updateLocalProperties() {
	// update world matrix.
	composeMatrix(this->m_worldPosition, this->m_worldEulerAngle, this->m_worldScale, this->m_worldMatrix);
	// calculate local matrix
	LEEntity* pEntity = this->m_pAttachedEntity;
	if (LEEntity* pParent = this->m_pAttachedEntity->getParent()) {
		this->m_localMatrix = glm::inverse(pParent->getTransform()->getWorldMatrix()) * this->m_worldMatrix;
	}
	else {
		this->m_localMatrix = this->m_worldMatrix;
	}
	// unused
	glm::vec3 skew;
	glm::vec4 projection;
	glm::decompose(this->m_localMatrix, this->m_localScale, this->m_localRotation, 
	  this->m_localPosition, skew, projection);
	this->m_localRotation = glm::conjugate(this->m_localRotation);
	this->m_localEulerAngle = glm::eulerAngles(this->m_localRotation);
	// recursively update children's transform
	for (int i = 0; i < pEntity->children().size(); i++) {
		pEntity->children().at(i)->getTransform()->update(1);
	}
}

void LETransform::update(bool isLocalChanged) {
	if (isLocalChanged) {
		// update local matrix.
		composeMatrix(this->m_localPosition, this->m_localEulerAngle, this->m_localScale, this->m_localMatrix);
	}
	LEEntity* pEntity = this->m_pAttachedEntity;
	if (LEEntity* pParent = this->m_pAttachedEntity->getParent()) {
		this->m_worldMatrix = pParent->getTransform()->getWorldMatrix() * this->m_localMatrix;
	}
	else {
		this->m_worldMatrix = this->m_localMatrix;
	}
	// unused
	glm::vec3 skew;
	glm::vec4 projection;
	glm::decompose(this->m_worldMatrix, this->m_worldScale, this->m_worldRotation,
		this->m_worldPosition, skew, projection);
	this->m_worldRotation = glm::conjugate(this->m_worldRotation);
	this->m_worldEulerAngle = glm::eulerAngles(this->m_worldRotation);
	// update worlds
	// recursively update children's transform
	for(int i = 0;i<pEntity->children().size();i++) {
		pEntity->children().at(i)->getTransform()->update(false);
	}
}

void LETransform::onParentChanged() {
	if(this->getParent() == nullptr) {
		this->m_localMatrix = this->m_worldMatrix;
	} else {
		this->m_localMatrix = glm::inverse(this->m_pAttachedEntity->getParent()->getTransform()->getWorldMatrix()) * this->m_worldMatrix;
	}
	// unused
	glm::vec3 skew;
	glm::vec4 projection;
	glm::decompose(this->m_localMatrix, this->m_localScale, this->m_localRotation,
	               this->m_localPosition, skew, projection);
}

glm::vec3 LETransform::getForwardDirection() const {
	glm::vec3 forward{ 0,0,1 };
	return glm::rotate(this->m_worldRotation, forward);
}

void LETransform::rotateAroundAxis(LETransform& centerTransform, glm::vec3& axis, float angle) {
	glm::vec3 axisLocal = inverseVector(axis);
	glm::qua q = glm::qua(glm::radians(angle), axisLocal);
	this->m_localRotation = glm::normalize(this->m_localRotation * q);
	this->update();
}
