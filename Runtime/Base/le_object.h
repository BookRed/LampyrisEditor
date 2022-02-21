#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_object.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_OBJECT_H
#define LE_OBJECT_H

// LE Includes
#include "le_global.h"
#include "le_initial.h"

// #include "../GC/le_gc_manager.h"

// STL Includes
#include <string>
#include <type_traits>

class LEClassInfo;

// The base class of classes in Lampyris Engine
class LEObject {
	LE_INITIAL_DECLARE
protected:
	unsigned int  m_refCount;
	unsigned int  m_instanceID;
public:
	LEObject();

	~LEObject();
	
	virtual std::string toString() const;

	virtual LEUInt getHashCode();

	LE_FORCEINLINE void incRef();

	LE_FORCEINLINE void decRef();

	LE_FORCEINLINE unsigned int getInstanceID() {
		if (this->m_instanceID == 0) {
			this->m_instanceID = std::hash<unsigned int>()(reinterpret_cast<long long>(this));
		}
		return this->m_instanceID;
	}

	LE_FORCEINLINE void setInstanceID(unsigned int instanceID) {
		 this->m_instanceID = instanceID;
	}

	LE_FORCEINLINE LEUInt getRefCount() const;
};
#endif // !LE_OBJECT_H

