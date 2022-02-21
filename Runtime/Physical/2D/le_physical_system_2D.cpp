/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Physical2D
 * \File:    le_physical_system_2D.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "../../Base/le_entity.h"
#include "le_physical_system_2D.h"
#include "../../Math/le_transform.h"

void LEPhysicalSystem2D::init() {
	this->m_worldAABB.lowerBound.Set(-0xfffff, -0xfffff);
	this->m_worldAABB.upperBound.Set(+0xfffff, +0xfffff);
	b2Vec2 gravity(0.0f, -9.8f);
	bool doSleep = true;
	//在栈上创建world
	this->m_pWorld = new b2World(gravity);
	//this->m_pWorld->SetAllowSleeping(doSleep);
	this->m_pWorld->SetAllowSleeping(false);
	this->m_pWorld->SetContinuousPhysics(true);
	this->m_pWorld->SetWarmStarting(true);
}

void LEPhysicalSystem2D::update() {
	this->m_pWorld->Step(1 / 60.f, 10, 10);
	b2Body* pBody = this->m_pWorld->GetBodyList();
	for (int i = 0; i < this->m_pWorld->GetBodyCount(); i++) {
		LEEntity* pEnity = (LEEntity*)pBody->GetUserData().pointer;
		auto pTransform = pEnity->getTransform();
		auto angle = pTransform->getLocalEulerAngle();
		pTransform->setLocalPosition(pBody->GetPosition().x, pBody->GetPosition().y, pTransform->getWorldRotation().z);
		pTransform->setLocalEulerAngle(angle.x, angle.y, pBody->GetAngle() * 180 / b2_pi);
		pBody = pBody->GetNext();
	}
}

b2Body* LEPhysicalSystem2D::createGeneralBody() {
	b2Body* body       = nullptr;
	b2BodyDef* bodyDef = new b2BodyDef();
	bodyDef->type = b2_dynamicBody;
	
	return body;
}

void LEPhysicalSystem2D::createBoxCollider() {
	b2Body* body = createGeneralBody();
	if (!body) return;
	b2FixtureDef* fixtrueDef = new b2FixtureDef();
	b2PolygonShape* polugonShape = new b2PolygonShape();
  
	if (true/*gameObject->getRenderable() != nullptr*/) {
		// Rect rect = gameObject->getRenderable()->getBoundingRect();
		// polugonShape->SetAsBox(rect.getWidth() / 2.0f, rect.getHeight()/2.0f);
	}
	else
	{
		polugonShape->SetAsBox(1.0f, 1.0f);
	}
	fixtrueDef->shape = polugonShape;
	fixtrueDef->density = 1.0f;
	fixtrueDef->friction = 0.3f;
	LEBoxCollider2D* pCollider = new LEBoxCollider2D;
	pCollider->m_body = body;
	pCollider->m_fixture = body->CreateFixture(fixtrueDef);
	pCollider->m_shape = polugonShape;
}

void LEPhysicalSystem2D::createCircleCollider() {
	b2Body* body = createGeneralBody();
	if (!body) return;

	b2FixtureDef* fixtrueDef = new b2FixtureDef();
	b2CircleShape* circleShape = new b2CircleShape();

	if (true/*gameObject->getRenderable() != nullptr*/) {
		// auto a = gameObject->getRenderable();
		// Rect rect = gameObject->getRenderable()->getBoundingRect();
		// circleShape->m_radius = max(rect.getWidth() / 2.0f, rect.getHeight() / 2.0f);
	}
	else {
		circleShape->m_radius = 0.0f;
	}
	fixtrueDef->shape = circleShape;
	body->CreateFixture(fixtrueDef);
	LECircleCollider2D* pCollider = new LECircleCollider2D;
	pCollider->m_body = body;
	pCollider->m_fixture = body->CreateFixture(fixtrueDef);
	pCollider->m_shape = circleShape;
}

void LEPhysicalSystem2D::createEdgeCollider() {
	b2Body* body = createGeneralBody();
	if (!body) return;

	b2FixtureDef* fixtrueDef = new b2FixtureDef();
	b2EdgeShape* edgeShape = new b2EdgeShape();

	if (true/*gameObject->getRenderable() != nullptr*/) {
		// Rect rect = gameObject->getRenderable()->getBoundingRect();
		// edgeShape->Set(b2Vec2(-rect.getWidth() / 2.0f, -rect.getHeight() / 2.0f), b2Vec2(rect.getWidth() / 2.0f, rect.getHeight() / 2.0f));
	}
	else {
		// edgeShape->Set(b2Vec2(-1, -1), b2Vec2(1, 1));
	}
	fixtrueDef->shape = edgeShape;
	body->CreateFixture(fixtrueDef);
}

void LEPhysicalSystem2D::createPolygonCollider() {
	b2Body* body = createGeneralBody();
	if (!body) return;

	b2FixtureDef* fixtrueDef = new b2FixtureDef();
	b2PolygonShape* polygonShape = new b2PolygonShape();
	fixtrueDef->shape = polygonShape;
	body->CreateFixture(fixtrueDef);
}
