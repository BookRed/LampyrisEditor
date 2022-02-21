#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_callable.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_CALLABLE_H
#define LE_CALLABLE_H

// LE Includes
#include "le_assert.h"

// STL Includes
#include <vector>
#include <string>

class LEObject;
class LEClassInfo;
class LEProperty;

class LECallable {
public:
	typedef void(*CallableFunctionPointer)(LEObject* object, 
		                                   LECallable* callable, 
		                                   void* parameters, 
		                                   void* returnValue);
private:
	LEClassInfo*             m_belonged_class;
	std::string              m_callable_name;
	LEProperty*              m_return_value;
	std::vector<LEProperty*> m_parameters;
	CallableFunctionPointer  m_function_ptr;

public:
	LECallable(LEClassInfo* belonged_class, 
		       const std::string& callable_name):
		m_belonged_class(belonged_class),
		m_callable_name(callable_name),
		m_function_ptr(nullptr),
		m_return_value(nullptr) {}

	std::string getCallableName() const {
		return this->m_callable_name;
	}

	unsigned int getParameterCount() const {
		return this->m_parameters.size();
	}

	LEProperty* getParameterAt(unsigned int index) const {
		LE_ASSERT(index<this->m_parameters.size() && index>=0);
		return this->m_parameters[index];
	}

	bool isNoReturnCallable() const {
		return this->m_return_value == nullptr;
	}
};

#endif // ! LE_CALLABLE_H