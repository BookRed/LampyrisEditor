#pragma once
/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Utility
 * \File:    le_xml.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#include "le_xml.h"

LEXml::LEXml() :m_currentNode(nullptr) {

}

LEXml::~LEXml() {

}

bool LEXml::isEmpty() const {
	return m_currentNode == nullptr;
}

bool LEXml::hasNext() const {
	return !this->m_currentNode->NoChildren();
}

void LEXml::moveToNext() {
	if (!this->isEmpty()) {
		this->m_currentNode = this->m_currentNode->NextSibling();
	}
}

bool LEXml::hasParent() const {
	return this->m_currentNode->Parent() != nullptr;
}

void LEXml::moveToParent() {
	this->m_currentNode = this->m_currentNode->Parent();
}

bool LEXml::moveToFirstChild(const char* nodeName) {
	auto node = this->m_currentNode->FirstChildElement(nodeName);
	if (!node) {
		return false;
	}
	else {
		this->m_currentNode = node;
		return true;
	}
}

std::tuple<LEXml*, const char*> LEXml::open(const char* fileName) {
	LEXml* xml = new LEXml;
	if (xml->m_xmlDoc.LoadFile(fileName)) {
		std::tuple<LEXml*, const char*> returnValue = { nullptr,xml->m_xmlDoc.ErrorStr()};
		delete xml;
		xml = nullptr;
		return returnValue;
	}
	xml->m_currentNode = xml->m_xmlDoc.RootElement();
	xml->m_path = fileName;
	return { xml,nullptr };
}

int LEXml::readInt() const {
	if (!this->m_currentNode) return 0;
	return ::atoi(this->m_currentNode->ToElement()->GetText());
}

float LEXml::readFloat() const {
	if (!this->m_currentNode) return 0.0f;
	return ::atof(this->m_currentNode->ToElement()->GetText());
}

const char* LEXml::readString() const {
	if (!this->m_currentNode) return nullptr;
	return this->m_currentNode->ToElement()->GetText();
}

void LEXml::removeCurrent() {
	if (!this->m_currentNode) return;
	auto node = this->m_currentNode->Parent();
	this->m_xmlDoc.DeleteNode(this->m_currentNode);
	this->m_currentNode = node;
}

void LEXml::insertFront(const char* nodeName, const char* value, bool moveToNewNode) {
	auto newNode = this->m_xmlDoc.NewElement(nodeName);
	newNode->SetText(value);
	this->m_currentNode->InsertFirstChild(newNode);
	if (moveToNewNode) {
		this->m_currentNode = newNode;
	}
}

void LEXml::insertBack(const char* nodeName, const char* value, bool moveToNewNode) {
	auto newNode = this->m_xmlDoc.NewElement(nodeName);
	newNode->SetText(value);
	this->m_currentNode->InsertEndChild(newNode);
	if (moveToNewNode) {
		this->m_currentNode = newNode;
	}
}

void LEXml::removeAttribute(const char* attributeName) {
	this->m_currentNode->ToElement()->DeleteAttribute(attributeName);
}

void LEXml::insertAfterCurrent(const char* nodeName, const char* value, bool moveToNewNode) {
	auto newNode = this->m_xmlDoc.NewElement(nodeName);
	if(value) newNode->SetText(value);
	this->m_currentNode->Parent()->InsertAfterChild(this->m_currentNode,newNode);
	if (moveToNewNode) {
		this->m_currentNode = newNode;
	}
}
