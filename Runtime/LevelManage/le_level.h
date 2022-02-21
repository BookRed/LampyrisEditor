#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  LevelManager
 * \File:    le_level.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_LEVEL_H
#define LE_LEVEL_H
// LE Includes
#include "../Base/le_object.h"

// STL Includes
#include <vector>

class LELight;
class LECuller;
class LEEntity;
class LECamera;
class LEPrimitiveRenderer;

#ifndef LE_RRIMITIVE_ID
// empty
#define LE_PRIMITIVE_ID_EMPTY             ( 1<<1 )
// 2d object							  
#define LE_PRIMITIVE_ID_SPRITE            ( 1<<2 )
// 3d objects							  
#define LE_PRIMITIVE_ID_CUBE              ( 1<<3 )
#define LE_PRIMITIVE_ID_PLANE             ( 1<<4 )
#define LE_PRIMITIVE_ID_SPHERE            ( 1<<5 )
#define LE_PRIMITIVE_ID_CYLINDER          ( 1<<6 )
#define LE_PRIMITIVE_ID_TEAPOT            ( 1<<11)
// lights
#define LE_PRIMITIVE_ID_DIRECTIONAL_LIGHT ( 1<<7 )
#define LE_PRIMITIVE_ID_POINT_LIGHT       ( 1<<8 )
#define LE_PRIMITIVE_ID_SPOT_LIGHT        ( 1<<9 )
// other								  
#define LE_PRIMITIVE_ID_CAMERA            ( 1<<10 )
#endif // !LE_PRIMITIVE_ID

#if defined(LE_EDITOR)
struct LEBuildInPrimitiveInfo {
	std::string category; // "2D objects" or "3D object", etc
	std::string name;
	int         ID; // object will be created according to ID (see macro 'LE_PRIMITIVE_ID_XXX').
};
#endif // !LE_EDITOR

class LELevel:public LEObject {
	LE_INITIAL_DECLARE
private:
	std::vector<LEEntity*>  m_pEntities;
	std::vector<LELight*>   m_pLights;
	std::vector<LECamera*>  m_pCameras;
	std::vector<LEEntity*>  m_pPersistentEntities;
public:
	LEEntity*               createEntity(const std::string& name);
			               
	void                    destoryEntity(LEEntity* pEntity);

	void                    computeVisibleRenderer(LECamera* pCuller);

	std::vector<LECamera*>& getCameras();

	std::vector<LELight*>&  getLights();

	void                    addCamera(LECamera* pCamera);

	void                    removeCamera(LECamera* pCamera);

	void                    addLight(LELight* pLight);

	void                    removeLight(LELight* pLight);

#if defined(LE_EDITOR)
	// used for context menu of entity tree: "create object"
	static const std::vector<LEBuildInPrimitiveInfo>& getBuildInPrimitiveInfoList();

	LEEntity*                createSandBoxEntity();
#endif // !LE_EDITOR
	void createPrimitiveEntity(int ID);
};
#endif // !LE_LEVEL_H