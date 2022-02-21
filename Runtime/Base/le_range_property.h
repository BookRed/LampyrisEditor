#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Reflection
 * \File:    le_range_property.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_RANGE_PROPERTY_H
#define LE_RANGE_PROPERTY_H

// LE Includes
#include "le_property.h"

// GLM Includes
#include <glm/glm.hpp>

// The variable types that can specify the range(we denoted them as 'range type') include:
// 1) integer type (unsigned, int, short, long,..)
// 2) floating point type (float, double)
// ATTENTION: It is not necessary to regard 'boolean' as range type.


#include "../Serialization/le_data_stream.h"
template<typename T>
class LERangeProperty :public LEProperty {
protected:
	bool   m_range_used;
	T      m_min_value;
	T      m_max_value;
	T      m_step;
public:
	LERangeProperty(LEClassInfo*          belongedClass,
				    const std::string&    propertyName,
				    unsigned              fieldOffset,
		            bool                  useRange   ,
		            T                     minValue   ,
		            T                     maxValue   ,
		            T                     step       ,
				    PropertyFlag          flag):
		            m_range_used(useRange),
					m_min_value(minValue),
		            m_max_value(maxValue),
		            m_step(step),
				    LEProperty(belongedClass, propertyName, fieldOffset,flag) {
		this->m_property_type = std::is_integral<T>::value ? PT_INT : PT_FLOAT;
	}

	void stepFoward(void* objectPtr) {
		T& value = this->getValue();
		value += this->m_step;
		value = glm::min(this->m_max_value, value);
	}

	void stepBackward(void* objectPtr) {
		T& value = this->getValue();
		value -= this->m_step;
		value = glm::max(this->m_min_value, value);
	}

	void setAsMaximum() {
		if (!this->m_range_used) {
			this->m_value = this->m_max_value;
		}
	}

	void setAsMinimum() {
		if (!this->m_range_used) {
			this->m_value = this->m_min_value;
		}
	}

	bool isRangeUsed() const {
		return this->m_range_used;
	}

	void set(void* objectPtr,void* value) {
		void* address = this->getPropertyPointer(objectPtr);
		(*(T*)(address)) = this->m_range_used?
			               glm::clamp(*(T*)(value),this->m_min_value,this->m_max_value):
						   *(T*)(value);
	}

	void archive(LEDataStream& stream, void* objectPtr) {
		void* address = this->getPropertyPointer(objectPtr);
		stream.archive(*(T*)address);
	}
};


#endif // !LE_RANGE_PROPERTY_H

