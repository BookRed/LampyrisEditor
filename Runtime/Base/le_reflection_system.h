#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_reflection_system.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_enum_register.h"
#include "le_subsystem.h"
#include "le_singleton.h"

// STL Includes
#include <string>
#include <unordered_map>

class LEObject;
class LEProperty;
class LECallable;
class LEEnumInfo;
class LEClassInfo;

class LEReflectionSystem:public SubSystem,
	                     public Singleton<LEReflectionSystem> {
private:
	typedef bool(*LEClassMethod)();
	typedef LEObject*(*LEClassCreateMethod)();
	struct LEClassExecutionInfo {
		LEClassInfo*  classInfo;
		LEClassMethod initMethod = nullptr;
		LEClassMethod terminateMethod = nullptr;
		LEClassMethod propertyRegisterMethod = nullptr;
		LEClassMethod propertyDestoryMethod = nullptr;
		LEClassCreateMethod classCreateMethod = nullptr;
		int           priority;
	};
	std::unordered_map<std::string, LEClassInfo*> m_classMap;
	std::unordered_map<std::string, LEEnumInfo*>  m_enumMap;
	std::vector<LEClassExecutionInfo>             m_classExecutionInfo;

	int                  m_index = 0;
	bool                 m_method_added_flag = false;
	LEClassExecutionInfo m_current_info;
public:
	LEReflectionSystem() {}

	~LEReflectionSystem() {}

	void doStartUp() {
		for (auto& info : this->m_classExecutionInfo) {
			if (info.initMethod)  info.initMethod();
			if (info.propertyRegisterMethod) info.propertyRegisterMethod();
		}
	}

	void doShutDown() {
		for (auto& info : this->m_classExecutionInfo) {
			if (info.propertyDestoryMethod) info.propertyDestoryMethod();
			if (info.terminateMethod) info.terminateMethod();
		}
	}

	void doUpdate() {}

	LEClassInfo& defClass(const std::string& class_name);

	const LEClassInfo* getClassInfo(const std::string& name) ;

	void addClassExecutionInfo(LEClassInfo* classInfo, int priority = -1);

	void addInitialMethod(LEClassMethod method);

	void addTerminateMethod(LEClassMethod method);

	void addPropertyRegisterMethod(LEClassMethod method);

	void addPropertyDestoryMethod(LEClassMethod method);

	void addCreateInstanceMethod(LEClassCreateMethod method);

	void doPostRegister(LEClassInfo* classInfo) {
		if (this->m_method_added_flag) {
			this->m_current_info.classInfo = classInfo;
			this->m_classExecutionInfo.push_back(this->m_current_info);
			this->m_current_info = LEClassExecutionInfo();
			this->m_method_added_flag = false;
			this->m_index++;
		}
	}

	LEObject* createInstance(const std::string& className) const;
};