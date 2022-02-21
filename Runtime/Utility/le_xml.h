#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Utility
 * \File:    le_xml.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_XML_H
#define LE_XML_H

// STL Includes
#include <tuple>
#include <string>
// 3rd-party Includes
#include <tinyxml2/tinyxml2.h>

class LEXml {
private:
	tinyxml2::XMLDocument m_xmlDoc;
	tinyxml2::XMLNode*    m_currentNode;
	std::string           m_path;

	LEXml();
public:
	// open an xml file
	static std::tuple<LEXml*,const char*> open(const char* fileName);

	~LEXml();

	bool isEmpty() const;

	bool hasNext() const;

	void moveToNext();

	bool hasParent() const;

	void moveToParent();

	bool moveToFirstChild(const char* nodeName);

	int readInt() const;

	float readFloat() const;

	const char* readString() const;

	void removeCurrent();

	void insertAfterCurrent(const char* nodeName, const char* value, bool moveToNewNode = true);

	void insertFront(const char* nodeName, const char* value, bool moveToNewNode = true);

	void insertBack(const char* nodeName, const char* value, bool moveToNewNode = true);

	template<typename T>
	void addAttribute(const char* attributeName, T value) {
		this->m_currentNode->ToElement()->SetAttribute(attributeName, value);
	}

	void setCurrentElement(const char* value) {
		this->m_currentNode->ToElement()->SetText(value);
	}
	const char* readStringElement(const char* attributeName) {
		return this->m_currentNode->FirstChildElement(attributeName)->GetText();
	}

	int readIntElement(const char* attributeName) {
		this->m_currentNode->ToElement()->IntAttribute(attributeName);
	}

	float readFloatElement(const char* attributeName) {
		this->m_currentNode->ToElement()->FloatAttribute(attributeName);
	}
	void removeAttribute(const char* attributeName);

	void moveToRoot() {
		this->m_currentNode = this->m_xmlDoc.RootElement();
	}

	void save() {
		this->m_xmlDoc.SaveFile(this->m_path.c_str());
	}
};
#endif //! LE_XML_H		