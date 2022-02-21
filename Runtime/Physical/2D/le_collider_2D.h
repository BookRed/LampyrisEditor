/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Physical2D
 * \File:    le_collider_2D.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_COLLIDER_2D_H
#define LE_COLLIDER_2D_H

// BOX2D Includes
#include <Box2D/Box2D.h>
// LE Includes
#include "../../Math/le_math_global.h"

class LECollider2D {
protected:
	// Box2D-Based Physical Variables
	b2Body*    m_body;
	b2Shape*   m_shape;
	b2Fixture* m_fixture;

	// Transform's constraints
	bool       m_fixedPosX;
	bool       m_fixedPosY;
	bool       m_fixedRotation;
public:
	void       addForce(const glm::vec2& force, const glm::vec2& target_point = glm::vec2());

	void       setFictionFactor(float fiction);

	float      getFictionFactor();

	void       setRestitutionFactor(float restitution);

	float      getRestitutionFactor();

	void       setMass(float mass);

	float      getMass();

	void       setFixedPositionX(bool value);

	bool       isFixedPositionX();

	void       setFixedPositionY(bool value);

	bool       isFixedPositionY();

	void       setFixedRotationZ(bool value);

	bool       isFixedRotationZ();

	void       setLinearVelocity(const glm::vec2& value);

	glm::vec2   getLinearVelocity();

	void	   setAngleVelocity(float value);

	float	   getAngleVelocity();

	void	   setGravityScale(float value);

	bool	   getGravityScale();

	void	   setInteria(float interia);

	float	   getInteria();

	void	   setSensorMode(bool value);

	bool	   isSensorMode(bool value);

	void	   setStatic(bool value);

	friend class LEPhysicalSystem2D;
};
#endif // !LE_COLLIDER_2D_H