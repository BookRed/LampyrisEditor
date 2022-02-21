#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Utility
 * \File:    le_json.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_JSON_H
#define LE_JSON_H

// JSON Includes
#include <json/json.h>

// STL Includes
#include <fstream>

// LE Includes
#include "../Base/le_type_traits.h"

#if defined(LE_DEBUG)
  #pragma comment(lib,"jsoncpp_d.lib")
#else
#  pragma comment(lib,"jsoncpp.lib")
#endif // !LE_DEBUG

#if defined(__GNUC__)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
  #pragma warning(disable:4996)
#endif

#define LE_JSON_TYPE_CONSTRAINT_EXPR(Type)  \
	(std::is_integral<Type>::value       || \
     std::is_floating_point<Type>::value || \
	 is_string<Type>::value)

template<typename First>
static Json::Value& locateValueRecursive(Json::Value& value, First arg) {
	return value[arg];
}

template<typename First, typename ...Args>
static Json::Value& locateValueRecursive(Json::Value& value, First arg, Args... args) {
	return locateValueRecursive(value[arg], args...);
}

template<typename First>
static Json::Value& locateValue(Json::Value& value, First arg) {
	return value[arg];
}

template<typename First, typename ...Args>
static Json::Value& locateValue(Json::Value& value, First arg, Args... args) {
	return locateValueRecursive(value[arg], args...);
}

class LEJsonBase {
protected:
	static std::string         ms_last_error;
	std::stack<Json::Value*>   m_valueStack;
	Json::Value                m_root;
	Json::Value*               m_valueCurrent;
public:
	LEJsonBase(const Json::Value& value):
		m_root(value), m_valueCurrent(nullptr) {}

	template<typename ...Args>
	void setValueLocation(Args... args) {
		Json::Value& value = locateValue(this->m_root, args...);
		this->m_valueCurrent = &value;
	}

	template<typename ...Args>
	void pushValueLocation(Args... args) {
		Json::Value& value = locateValue(this->m_root, args...);
		this->m_valueCurrent = &value;
		this->m_valueStack.push(&value);
	}

	template<typename ...Args>
	void pushValueLocationAfterCurrent(Args... args) {
		Json::Value& value = locateValue(*this->m_valueCurrent, args...);
		this->m_valueCurrent = &value;
		this->m_valueStack.push(&value);
	}

	void popValueLocation() {
		if (!this->m_valueStack.empty()) {
			this->m_valueStack.pop();
			if (!this->m_valueStack.empty()) {
				this->m_valueCurrent = this->m_valueStack.top();
			}
			else {
				// [Warning]: stack is empty
				this->m_valueCurrent = &this->m_root;
			}
		}
	}
};
class LEJsonReader:public LEJsonBase {
private:
	Json::Reader       m_reader;
	LEJsonReader(const Json::Reader &reader, const Json::Value &root) :
		m_reader(reader), LEJsonBase(root) {}
public:
	virtual ~LEJsonReader() {}

	static LEJsonReader* open(const std::string& filePath){
		std::ifstream ifs;
		ifs.open(filePath);
		if (!ifs.is_open()) {
			return nullptr;
		}
		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(ifs, root, false)) {
			ms_last_error = reader.getFormatedErrorMessages();
			ifs.close();
			return nullptr;
		}
		else {
			ms_last_error.clear();
			LEJsonReader* json = new LEJsonReader(reader, root);
			ifs.close();
			return json;
		}
	}

	static LEJsonReader* openFromString(const std::string& jsonString) {
		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(jsonString, root, false)) {
			ms_last_error = reader.getFormatedErrorMessages();
			return nullptr;
		}
		else {
			ms_last_error.clear();
			LEJsonReader* json = new LEJsonReader(reader, root);
			return json;
		}
	}
	static std::string& getLastErrorString() {
		return ms_last_error;
	}

	template<typename ...Args>
	bool contains(Args... args) {
		return !locateValue(this->m_root, args...).isNull();
	}

	template<typename ...Args>
	std::string readString(Args... args) {
		return locateValue(this->m_root,args...).asString();
	}

	template<typename ...Args>
	int readInt(Args ...args) {
		return locateValue(this->m_root,args...).asInt();
	}

	template<typename ...Args>
	int readUInt(Args ...args) {
		return locateValue(this->m_root, args...).asUInt();
	}

	template<typename ...Args>
	float readFloat(Args... args) {
		return locateValue(this->m_root,args...).asFloat();
	}

	template<typename ...Args>
	double readDouble(Args... args) {
		return locateValue(this->m_root, args...).asDouble();
	}

	template<typename ...Args>
	bool readBool(Args... args) {
		return locateValue(this->m_root, args...).asBool();
	}

	template<typename ...Args>
	int getArraySize(Args... args) {
		return locateValue(this->m_root, args...).size();
	}

	template<typename ...Args>
	bool containsAtCurrent(Args... args) {
		return !locateValue(*this->m_valueCurrent, args...).isNull();
	}

	template<typename ...Args>
	std::string readStringAtCurrent(Args... args) {
		return locateValue(*this->m_valueCurrent, args...).asString();
	}

	template<typename ...Args>
	int readIntAtCurrent(Args ...args) {
		return locateValue(*this->m_valueCurrent, args...).asInt();
	}

	template<typename ...Args>
	int readUIntAtCurrent(Args ...args) {
		return locateValue(*this->m_valueCurrent, args...).asUInt();
	}

	template<typename ...Args>
	float readFloatAtCurrent(Args... args) {
		return locateValue(*this->m_valueCurrent, args...).asFloat();
	}

	template<typename ...Args>
	double readDoubleAtCurrent(Args... args) {
		return locateValue(*this->m_valueCurrent, args...).asDouble();
	}

	template<typename ...Args>
	bool readBoolAtCurrent(Args... args) {
		return locateValue(*this->m_valueCurrent, args...).asBool();
	}

	template<typename ...Args>
	int getArraySizeAtCurrent(Args... args) {
		return locateValue(*this->m_valueCurrent, args...).size();
	}
};

class LEJsonWriter:public LEJsonBase {
private:
	static std::string           ms_last_error;
	Json::FastWriter             m_writer;
	Json::CommentPlacement       m_commentPlacement;
	std::ofstream                m_ofs;

	LEJsonWriter(Json::FastWriter writer,Json::Value root):
		m_writer(writer),m_commentPlacement(Json::commentBefore),
		LEJsonBase(root){}

public:
	virtual ~LEJsonWriter() {
		if (this->m_ofs.is_open()) {
			this->m_ofs.close();
		}
	}
	static LEJsonWriter* open(const std::string& filePath) {
		std::ofstream ofs;
		ofs.open(filePath);
		if (!ofs.is_open()) {
			return nullptr;
		}
		LEJsonWriter* writer = new LEJsonWriter(Json::FastWriter(),Json::Value());
		writer->m_ofs.swap(ofs);
		return writer;
	}

	template<class T, typename ...Args>
	void writeAt(const T& value, Args... args) {
		locateValue(this->m_root, args...) = Json::Value(value);
	}

	template<class T,typename ...Args>
	void writeAtCurrent(const T& value, Args... args) {
		locateValue(*this->m_valueCurrent, args...) = Json::Value(value);
	}

	void writeComment(const std::string& value) {
		this->m_root.setComment("// " + value, this->m_commentPlacement);
	}

	void setCommentPlacement(Json::CommentPlacement placement) {
		this->m_commentPlacement = placement;
	}

	void saveToFile() {
		auto jsonStr = this->m_writer.write(this->m_root);
		this->m_ofs << this->m_root << std::endl;
		this->m_ofs.flush();
	}
};
#endif // !LE_JSON_H