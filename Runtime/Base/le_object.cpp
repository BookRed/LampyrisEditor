#pragma once
/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Base
 * \File:    le_object.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#include "le_object.h"
#include "le_class_info.h"
#include "le_reflection_system.h"

LE_INITIAL_IMPL_BEGIN(LEObject)
{

}
LE_INITIAL_IMPL_END(LEObject)

LEObject::LEObject():
	m_refCount(0),m_instanceID(0) {}

LEObject::~LEObject() {}

std::string LEObject::toString() const {
	unsigned long long address = reinterpret_cast<unsigned long long>(this);
	LEChar buffer[256] = { '\0' };
	::sprintf(buffer, "%llX", address);
	// for 64-bit system address space, we need 16-bit hexadecimal number
	int paddingSpaceCount = 16 - ::strlen(buffer);
	std::string returnValue;
	returnValue.reserve(64);
	returnValue = "object of <class 'LEObject'> at address: 0x";
	for (LEUInt i = 0; i < paddingSpaceCount; i++) {
		returnValue += "0";
	}
	returnValue = returnValue + buffer;
	return returnValue;
}

LEUInt LEObject::getHashCode() {
	return (reinterpret_cast<LEULongLong>(this));
}

LE_FORCEINLINE void LEObject::incRef() {
	this->m_refCount++;
}

LE_FORCEINLINE void LEObject::decRef() {
	if (--this->m_refCount == 0) {
		// push to GC Manager's queue
		// GCManager::getInstance()->appendDestroyingObject(this);
	}
}

LE_FORCEINLINE LEUInt LEObject::getRefCount() const {
	return this->m_refCount;
}
