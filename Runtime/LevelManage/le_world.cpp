/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  LevelManage
 * \File:    le_world.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_level.h"
#include "le_world.h"
#include "..\Base\le_entity.h"
#include "..\Base\le_class_info.h"

LE_INITIAL_IMPL_BEGIN(LEWorld)
{
	LE_PROPERTY_REGISTER_METHOD(&LEWorld::initProperty);
	LE_PROPERTY_DESTORY_METHOD(&LEWorld::terminateProperty);
	LE_CREATE_INSTANCE_METHOD(&LEWorld::createInstance);
}
LE_INITIAL_IMPL_END(LEWorld)

LE_ADD_PROPERTY_BEGIN(LEWorld)
{
	LE_ADD_PROPERTY_POINTER(LEWorld, m_pEntryLevel);
	LE_ADD_PROPERTY_POINTER(LEWorld, m_pActiveLevel);
	LE_ADD_PROPERTY_ARRAY(LEWorld, m_pLevels);
}
LE_ADD_PROPERTY_END

LEWorld::LEWorld() {

}

LEWorld::~LEWorld() {

}

void LEWorld::loadLevelByName(const std::string& levelName) {

}

void LEWorld::loadLevelAsyncByName(const std::string& levelName) {

}

void LEWorld::loadLevelByIndex(int index) {
}

void LEWorld::loadLevelAsyncIndex(int index) {

}

void LEWorld::moveEntityToLevel(LEEntity* pEntity, LELevel* pLevel) {

}

LELevel* LEWorld::createEmptyLevel() {
	this->m_pActiveLevel = new LELevel;
	return this->m_pActiveLevel;
}

void LEWorld::mergeLevel(LELevel* pSrcLevel, LELevel* pDstLevel) {

}

void LEWorld::setActiveLeveL(LELevel* pLevel) {

}

LELevel* LEWorld::getActiveLevel() const {
	return m_pActiveLevel;
}

std::vector<LELevel*> LEWorld::getAllLevels() const {
	return this->m_pLevels;
}
