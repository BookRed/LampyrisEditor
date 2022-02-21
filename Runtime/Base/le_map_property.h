#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_map_property.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_MAP_PROPERTY_H
#define LE_MAP_PROPERTY_H

// LE Includes
#include "le_property.h"
#include "../Serialization/le_data_stream.h"

// STL Includes
#include <unordered_map>

template<typename Key,typename Value>
class LEMapProperty :public LEProperty {
public:
	LEMapProperty(LEClassInfo *      belongedClass,
		          const std::string& propertyName,
		          unsigned int       fieldOffset,
                  PropertyFlag       flag):
		          LEProperty(belongedClass, propertyName, fieldOffset,flag) {}

	std::unordered_map<Key,Value>& getMapObject(void* objectPtr) const {
		void* address = this->getPropertyPointer(objectPtr);
		auto &map = (*(std::unordered_map<Key, Value>*)(address));
		return map;
	}

	unsigned int getMapSize(void* objectPtr) const {
		return this->getMapObject(objectPtr).size();
	}

	Value& getValue(void* objectPtr, const Key& key) const {
		void* address = this->getPropertyPointer(objectPtr);
		auto& map = (*(std::unordered_map<Key, Value>*)(address));
		LE_ASSERT(this->containsKey(key));
		return map[key];
	}

	void setValue(void* objectPtr, const Key& key, const Value& value) const {
		LE_ASSERT(this->containsKey(key));
		this->getMapObject(objectPtr)[key] = value;
	}

	void set(void* objectPtr, void* value) {
		void* address = this->getPropertyPointer(objectPtr);
		(*(std::unordered_map<Key, Value>*)(address)) = (*(std::unordered_map<Key, Value>*)(objectPtr));
	}

	void clear(void* objectPtr) {
		this->getMapObject(objectPtr).clear();
	}

	bool containsKey(void* objectPtr,const Key& key) const {
		auto& map = this->getMapObject(objectPtr);
		return map.find(key) != map.end();
	}

	void insert(void* objectPtr, const Key& key, const Value& value) {
		auto& map = this->getMapObject(objectPtr);
		map.insert(key, value);
	}

	void removeKey(void* objectPtr, const Key& key) {
		if (this->containsKey(objectPtr, key)) {
			auto& map = this->getMapObject(objectPtr);
			map.erase(map.find(key));
		}
	}

	void archive(LEDataStream& stream, void* objectPtr) {
		auto& map = this->getMapObject(objectPtr);
		stream.archive(map);
	}
};

#endif // !LE_MAP_PROPERTY_H
