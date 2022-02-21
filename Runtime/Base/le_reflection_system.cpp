#pragma once
/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Base
 * \File:    le_reflection_system.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_class_info.h"
#include "le_reflection_system.h"

LEClassInfo& LEReflectionSystem::defClass(const std::string& class_name) {
	LEClassInfo* classInfo = new LEClassInfo(class_name);
	this->m_classMap[class_name] = classInfo;
	return *classInfo;
}

const LEClassInfo* LEReflectionSystem::getClassInfo(const std::string& name) {
	if (this->m_classMap.contains(name)) {
		return this->m_classMap[name];
	}
	return nullptr;
}

void LEReflectionSystem::addClassExecutionInfo(LEClassInfo* classInfo, int priority) {
	LEClassExecutionInfo classExecutionInfo;
	classExecutionInfo.classInfo = classInfo;
	this->m_classExecutionInfo.push_back(classExecutionInfo);
}

void LEReflectionSystem::addInitialMethod(LEClassMethod method) {
	this->m_method_added_flag = true;
	this->m_current_info.initMethod = method;
}

void LEReflectionSystem::addTerminateMethod(LEClassMethod method) {
	this->m_method_added_flag = true;
	this->m_current_info.terminateMethod = method;
}

void LEReflectionSystem::addPropertyRegisterMethod(LEClassMethod method) {
	this->m_method_added_flag = true;
	this->m_current_info.propertyRegisterMethod = method;
}

void LEReflectionSystem::addPropertyDestoryMethod(LEClassMethod method) {
	this->m_method_added_flag = true;
	this->m_current_info.propertyDestoryMethod = method;
}

void LEReflectionSystem::addCreateInstanceMethod(LEClassCreateMethod method) {
	this->m_method_added_flag = true;
	this->m_current_info.classCreateMethod = method;
}

LEObject* LEReflectionSystem::createInstance(const std::string& className) const {
	for (int i = 0; i < this->m_classExecutionInfo.size(); i++) {
		if (this->m_classExecutionInfo[i].classInfo->getClassName() == className) {
			auto callable = this->m_classExecutionInfo[i].classCreateMethod;
			if (callable) {
				return callable();
			}
			else {
				// [Fatal Error]:
			}
		}
	}
	return nullptr;
}
