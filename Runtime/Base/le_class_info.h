/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_class_info.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_CLASS_INFO_H
#define LE_CLASS_INFO_H

// STL Includes
#include <vector>

// LE Includes
#include "le_callable.h"
#include "le_array_property.h"
#include "le_boolean_property.h"
#include "le_string_property.h"
#include "le_enum_property.h"
#include "le_map_property.h"
#include "le_object_property.h"
#include "le_pointer_property.h"
#include "le_range_property.cpp"
#include "le_type_traits.h"

class LEClassInfo {
private:
	std::string                    m_className;
	std::vector<LECallable*>       m_methods;
	std::vector<LEProperty*>       m_properties;
public:
	LEClassInfo(const std::string& className):
		m_className(className) {
		return;
	}
	// for types 'int','long','short'...
	template<class U, class T>
	requires RangeType<U>
	void addRangeProperty(U T::* val, const std::string& name,bool useRange = false,U min = U(),U max = U(),U step = U(), 
		LEProperty::PropertyFlag flag = LEProperty::PF_SAVE_LOAD) {
		int offset = reinterpret_cast<int>(*(void**)(&val));
		LEProperty* info = nullptr;

		if (std::_Is_nonbool_integral<U> || std::is_floating_point<U>::value) {
			info = new LERangeProperty<U>(this, name, offset,useRange,min,max,step,flag);
			this->m_properties.push_back(info);
		}
	}

	// for 'boolean'
	template<class T>
	void addBooleanProperty(bool T::* val, const std::string& name,LEProperty::PropertyFlag flag = LEProperty::PF_SAVE_LOAD) {
		int offset = reinterpret_cast<int>(*(void**)(&val));
		LEProperty* info = new LEBooleanProperty(this, name, offset,flag);
		this->m_properties.push_back(info);
	}

	template<class U, class T>
	void addObjectProperty(U T::* object, const std::string& name, LEProperty::PropertyFlag flag = LEProperty::PF_SAVE_LOAD) {
		int offset = reinterpret_cast<int>(*(void**)(&object));
		LEProperty* info = new LEObjectProperty<U>(this, name, offset, flag);
		this->m_properties.push_back(info);
	}

	template<class U, class T>
	void addEnumProperty(U T::* val, const std::string& name, const std::string& enum_name,
		LEProperty::PropertyFlag flag = LEProperty::PF_SAVE_LOAD) {
		int offset = reinterpret_cast<int>(*(void**)(&val));
		LEProperty* info = nullptr;

		if (std::is_enum<U>::value) {
			info = new LEEnumProperty<U>(this,
										 name,
										 offset, 
				                         enum_name,
								         flag);
			this->m_properties.push_back(info);
		}
	}

	template<class U,class T> 
	requires std::is_base_of<LEObject,U>::value
	void addPointerProperty(U* T::* val, const std::string& name, 
		LEProperty::PropertyFlag flag = LEProperty::PF_SAVE_LOAD) {
		int offset = reinterpret_cast<int>(*(void**)(&val));
		LEProperty* info = new LEPointerProperty(this,
											     name,
											     offset,
											     flag);
		this->m_properties.push_back(info);
	}

	template<class U,class T> 
	// requires std::is_base_of<LEObject, U>::value || BasicType<U> || std::is_same<std::string,U>::value || is_vector<U>::value
	void addArrayProperty(std::vector<U> T::* val, const std::string& name,
		LEProperty::PropertyFlag flag = LEProperty::PF_SAVE_LOAD) {
		int offset = reinterpret_cast<int>(*(void**)(&val));
		LEProperty* info = new LEArrayProperty<U>(this,
			                                      name,
			                                      offset,
											      flag);
		this->m_properties.push_back(info);
	}

	template<class Key,class Value,class T> 
	requires std::is_pointer<Key>::value || BasicType<Key>
	void addMapProperty(std::unordered_map<Key, Value> T::* val, const std::string& name,
		LEProperty::PropertyFlag flag = LEProperty::PF_SAVE_LOAD) {
		int offset = reinterpret_cast<int>(*(void**)(&val));
		LEProperty* info = new LEMapProperty<Key,Value>(this,
			                                            name,
			                                            offset,
										                flag);
		this->m_properties.push_back(info);
	}

	template<class T>
	void addStringProperty(std::string T::* val, const std::string& name, 
		LEProperty::PropertyFlag flag = LEProperty::PF_SAVE_LOAD) {
		int offset = reinterpret_cast<int>(*(void**)(&val));
		LEProperty* info = new LEStringProperty(this,
			                                    name,
			                                    offset,
				                                flag);
		this->m_properties.push_back(info);
	}
	template< typename R, typename T, typename... Args>
	LEClassInfo& defMethod(R T::* method, const char* name) {
		LECallable* callable =  new LECallable;
		return *this;
	}

	const std::vector<LEProperty*>& getAllProperties() const {
		return this->m_properties;
	}

	void clearProperties() {
		for (int i = 0; i < this->m_properties.size(); i++) {
			LE_SAFE_RELEASE(this->m_properties[i]);
		}
		this->m_properties.clear();
	}

	LEProperty* getProperty(const std::string& name) const {
		for (auto& property : this->m_properties) {
			if (property->getPropertyName() == name) {
				return property;
			}
		}
		return nullptr;
	}

	const std::string& getClassName() const {
		return this->m_className;
	}
};

#endif // !LE_CLASS_INFO_H