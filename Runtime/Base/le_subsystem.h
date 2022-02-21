/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_subsystem.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_SUBSYSTEM_H
#define LE_SUBSYSTEM_H

#include "le_object.h"

interface SubSystem { // extend LEObject {
public:
	virtual void doStartUp() = 0;

	virtual void doShutDown() = 0;

	virtual void doUpdate() = 0;
};
#endif // ! LE_SUBSYSTEM_H