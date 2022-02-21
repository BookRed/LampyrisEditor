#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_property.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_PROPERTY_H
#define LE_PROPERTY_H

// LE Includes
#include "le_type_traits.h"

// STL Includes
#include <vector>
#include <string>

class LEClassInfo;

class LEStream;
class LECallable;
class LEDataStream;

class LEProperty {
public:
	enum PropertyType {
		PT_None    = 0x00,
		PT_INT     = 0x01,
		PT_FLOAT   = 0x02,
		PT_Enum    = 0x04,
		PT_Object  = 0x08,
		PT_MAP     = 0x10,
		PT_ARRAY   = 0x20,
		PT_POINTER = 0x40,
		PT_STRING  = 0x80,
		PT_MAX     = 0x7fffffff
	};

	enum PropertyFlag {
		PF_None = 0x00,
		PF_CLONE = 0x01,
		PF_COPY = 0x02,
		PF_SAVE = 0x04,
		PF_LOAD = 0x08,
		PF_SAVE_LOAD = 0x0C,
		PF_MAX_ENUM = 0x7fffffff
	};

	enum PropertyOwnerType {
		POT_MEMBER = 0x01,     // a member of one class
		POT_PARAMTETER = 0x02, // a parameter of one callable object
		POT_MAX = 0x7fffffff
	};

protected:
	union {
		LEClassInfo* m_belonged_class;
		LECallable*  m_belonged_callable;
	};

	unsigned int      m_offset;
	std::string       m_property_name;
	PropertyType      m_property_type;
	PropertyFlag      m_property_flag;
	PropertyOwnerType m_property_owner_type;

public:

	LEProperty(LEClassInfo*       belongedClass,
		       const std::string& propertyName, 
		       unsigned int       offset,
			   PropertyFlag       flag): 
		m_belonged_class(belongedClass),
		m_property_name(propertyName),
		m_offset(offset),
		m_property_flag(flag) {}

	virtual ~LEProperty() {}

	LE_FORCEINLINE virtual PropertyType getPropertyType() const {
		return this->m_property_type;
	}

	LE_FORCEINLINE virtual PropertyFlag getPropertyFlag() const {
		return this->m_property_flag;
	}

	LE_FORCEINLINE virtual PropertyOwnerType getPropertyOwnerType() const {
		return this->m_property_owner_type;
	}

	LE_FORCEINLINE LEClassInfo* getClassInfo() const {
		return this->m_belonged_class;
	}

	LE_FORCEINLINE const std::string& getPropertyName() const{
		return this->m_property_name;
	}

	LE_FORCEINLINE virtual void* getPropertyPointer(void* objectPtr) const {
		return (void*)(((unsigned char*)objectPtr) + this->m_offset);
	}

	LE_FORCEINLINE virtual void set(void* objectPtr, void* value) = 0;

	LE_FORCEINLINE virtual void* get(void* objectPtr) {
		return this->getPropertyPointer(objectPtr);
	}

	virtual void archive(LEDataStream& stream, void* object) = 0;
};

#endif // !LE_PROPERTY_H