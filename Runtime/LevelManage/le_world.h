#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  LevelManage
 * \File:    le_world.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_WORLD_H
#define LE_WORLD_H

// LE Includes
#include "..\Base\le_object.h"
#include "..\Base\le_singleton.h"

// STL Includes
#include <vector>
#include <string>

class LEEntity;
class LELevel;

class LEWorld :public Singleton<LEWorld>,public LEObject {
	LE_INITIAL_DECLARE
private:
	LELevel*              m_pEntryLevel;
	LELevel*              m_pActiveLevel;
	std::vector<LELevel*> m_pLevels;
public:
	                      LEWorld();
			              
	virtual              ~LEWorld();
#ifdef LAMPYRIS_EDITOR
	void                  setEntryLevel(LEWorld* pWorld){
		LE_ASSERT_MSG(m_pEntryLevel != nullptr,"the level can not be nullptr")
	}
#endif
	LELevel*              getEntryLevel() const {
		return this->m_pActiveLevel;
	}

	void                  loadLevelByName(const std::string& levelName);

	void                  loadLevelAsyncByName(const std::string& levelName);

	void                  loadLevelByIndex(int index);
		                  
	void                  loadLevelAsyncIndex(int index);
		                  
	void                  moveEntityToLevel(LEEntity* pEntity, LELevel* pLevel);
			              
	LELevel*              createEmptyLevel();
			              
	void                  mergeLevel(LELevel* pSrcLevel, LELevel* pDstLevel);
		                  
	void                  setActiveLeveL(LELevel* pLevel);
			              
	LELevel*              getActiveLevel() const;

	std::vector<LELevel*> getAllLevels() const;
};
#endif // !LE_WORLD_H