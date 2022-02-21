#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_initial.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_INITIAL_H
#define LE_INITIAL_H

class LEObject;

#define LE_INITIAL_DECLARE_NO_CREATE                   \
public:                                                \
	static bool        ms_is_class_registered;         \
	static int         ms_priority;                    \
	static bool        doClassRegister();              \
	virtual const LEClassInfo* getClassInfo() const;   \
	static bool initProperty();                        \
	static bool terminateProperty();                   \
private:                                               \
	static LEClassInfo ms_class_info;       

#define LE_INITIAL_DECLARE                             \
	LE_INITIAL_DECLARE_NO_CREATE                       \
public:                                                \
	static LEObject*   createInstance();               

#define LE_PRIORITY_DONT_CARE

#define LE_INITIAL_METHOD(method)           \
	LEReflectionSystem::getInstance()->addInitialMethod(method);

#define LE_TERMINATE_METHOD(method)         \
	LEReflectionSystem::getInstance()->addTerminateMethod(method);

#define LE_PROPERTY_REGISTER_METHOD(method) \
	LEReflectionSystem::getInstance()->addPropertyRegisterMethod(method);

#define LE_PROPERTY_DESTORY_METHOD(method)  \
	LEReflectionSystem::getInstance()->addPropertyDestoryMethod(method);

#define LE_CREATE_INSTANCE_METHOD(method)  \
	LEReflectionSystem::getInstance()->addCreateInstanceMethod(method);
		
#define LE_INITIAL_IMPL_BEGIN(class_name)                  \
	LEClassInfo class_name::ms_class_info                  \
		= LEClassInfo(#class_name);                        \
	const LEClassInfo* class_name::getClassInfo() const {  \
		return &this->ms_class_info; }                     \
	int         class_name::ms_priority;                   \
	bool class_name::doClassRegister() {                   \
		if(class_name::ms_is_class_registered) return true;\

#define LE_INITIAL_IMPL_NO_CREATE_BEGIN(class_name)   \
	LE_INITIAL_IMPL_BEGIN(class_name)

#define LE_INITIAL_IMPL_NO_CREATE_END(class_name) \
	LEReflectionSystem::getInstance()->doPostRegister(&class_name::ms_class_info);\
		return true; \
	}\
	bool class_name::terminateProperty()  \
	{                  \
		class_name::ms_class_info.clearProperties();  \
		return true;                                   \
	}                                 \
	bool class_name::ms_is_class_registered = class_name::doClassRegister();

#define LE_INITIAL_IMPL_END(class_name) \
	LEReflectionSystem::getInstance()->doPostRegister(&class_name::ms_class_info);\
		return true; \
	}\
	bool class_name::terminateProperty()  \
	{                  \
		class_name::ms_class_info.clearProperties();  \
		return true;                                   \
	}                                 \
	LEObject* class_name::createInstance() {return new class_name;} \
	bool class_name::ms_is_class_registered = class_name::doClassRegister();

#define LE_ADD_PROPERTY_BEGIN(class_name) \
	bool class_name::initProperty() {

#define LE_ADD_PROPERTY_END return true;}

#define LE_ADD_PROPERTY_BOOL(class_name,property) \
	class_name::ms_class_info.addBooleanProperty(&class_name::property,#property);

#define LE_ADD_PROPERTY_RANGE_USE(class_name,property,min,max,step) \
	class_name::ms_class_info.addRangeProperty(&class_name::property,#property,min,max,step);

#define LE_ADD_PROPERTY_RANGE_NOUSE(class_name,property) \
	class_name::ms_class_info.addRangeProperty(&class_name::property,#property);

#define LE_ADD_PROPERTY_ARRAY(class_name,property) \
	class_name::ms_class_info.addArrayProperty(&class_name::property,#property);

#define LE_ADD_PROPERTY_STRING(class_name,property) \
	class_name::ms_class_info.addStringProperty(&class_name::property,#property);

#define LE_ADD_PROPERTY_OBJECT(class_name,property) \
	class_name::ms_class_info.addObjectProperty(&class_name::property,#property);

#define LE_ADD_PROPERTY_MAP(class_name,property) \
	class_name::ms_class_info.addMapProperty(&class_name::property,#property);

#define LE_ADD_PROPERTY_POINTER(class_name,property) \
	class_name::ms_class_info.addPointerProperty(&class_name::property,#property);

#endif // !LE_INITIAL_H 