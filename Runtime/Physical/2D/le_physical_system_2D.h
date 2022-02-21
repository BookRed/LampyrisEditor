#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Physical2D
 * \File:    le_physical_system_2D.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "../../Base/le_global.h"

#include "le_box_collider_2D.h"
#include "le_circle_collider_2D.h"
#include "le_edge_collider_2D.h"
#include "le_rigidbody_2D.h"

#if defined(LE_DEBUG)
  #if defined(LE_PLATFORM_WINDOWS)
    #pragma comment(lib,"Box2D_d.lib")
  #else 
  #endif 
#else
  #if defined(LE_PLATFORM_WINDOWS)
    #pragma comment(lib,"Box2D.lib")
  #endif
#endif 
class LEEntity;

class LEPhysicalSystem2D {
private:
	b2AABB   m_worldAABB;
	b2World* m_pWorld;
public:
	void init();
	
	void update();

	b2Body* createGeneralBody();

	void createBoxCollider();

	void createCircleCollider();

	void createEdgeCollider();

	void createPolygonCollider();
};
