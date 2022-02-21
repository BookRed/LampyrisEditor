/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Physical2D
 * \File:    le_collider_2D.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_collider_2D.h"

void LECollider2D::addForce(const glm::vec2& force, const glm::vec2& target_point) {
	if (target_point == glm::vec2(0,0)) {
		m_body->ApplyForceToCenter(b2Vec2(force[0], force[1]), true);
	}
	else {
		m_body->ApplyForce(b2Vec2(force[0], force[1]), b2Vec2(target_point[0], target_point[1]), true);
	}
}

void LECollider2D::setFictionFactor(float fiction) {
	m_fixture->SetFriction(fiction);
}

float LECollider2D::getFictionFactor() {
	return m_fixture->GetFriction();
}

void LECollider2D::setRestitutionFactor(float restitution) {
	m_fixture->SetRestitution(restitution);
}

float LECollider2D::getRestitutionFactor() {
	return m_fixture->GetRestitution();
}

void LECollider2D::setMass(float mass) {
	b2MassData massData;
	m_body->GetMassData(&massData);
	massData.mass = mass;
	m_body->SetMassData(&massData);
}

float LECollider2D::getMass() {
	b2MassData massData;
	m_body->GetMassData(&massData);
	return massData.mass;
}

void LECollider2D::setFixedPositionX(bool value) {
	m_fixedPosX = true;
}

bool LECollider2D::isFixedPositionX() {
	return m_fixedPosX;
}

void LECollider2D::setFixedPositionY(bool value) {
	m_fixedPosY = value;
}

bool LECollider2D::isFixedPositionY() {
	return m_fixedPosY;
}

void LECollider2D::setFixedRotationZ(bool value) {
	m_fixedRotation = value;
}

bool LECollider2D::isFixedRotationZ() {
	return m_fixedRotation;
}

void LECollider2D::setLinearVelocity(const glm::vec2& value) {
	m_body->SetLinearVelocity(b2Vec2(value[0], value[1]));
}

glm::vec2 LECollider2D::getLinearVelocity() {
	return glm::vec2(m_body->GetLinearVelocity().x, m_body->GetLinearVelocity().y);
}

void LECollider2D::setAngleVelocity(float value) {
	m_body->SetAngularVelocity(value);
}

float LECollider2D::getAngleVelocity() {
	return m_body->GetAngularVelocity();
}

void LECollider2D::setGravityScale(float value) {
	m_body->SetGravityScale(value);
}

bool LECollider2D::getGravityScale() {
	return m_body->GetGravityScale();
}

void LECollider2D::setInteria(float interia) {
	b2MassData massData;
	m_body->GetMassData(&massData);
	massData.I = interia;
	m_body->SetMassData(&massData);
}

float LECollider2D::getInteria() {
	b2MassData massData;
	m_body->GetMassData(&massData);
	return massData.I;
}

void LECollider2D::setSensorMode(bool value) {
	m_fixture->SetSensor(value);
}

bool LECollider2D::isSensorMode(bool value) {
	return m_fixture->IsSensor();
}

void LECollider2D::setStatic(bool value) {
	m_body->SetType(value ? b2BodyType::b2_staticBody : b2BodyType::b2_dynamicBody);
}
