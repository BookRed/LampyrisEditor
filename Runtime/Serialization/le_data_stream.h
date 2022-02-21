/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Serialization
 * \File:    le_data_stream.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_DATA_STREAM_H
#define LE_DATA_STREAM_H

// STL Includes
#include <vector>
#include <unordered_map>
#include <unordered_set>

// LE Includes
#include "../Utility/le_json.h"
#include "../Utility/le_base64.h"
#include "../Base/le_reflection_system.h"

class LEObject;

class LEDataStream {
	// the header comments of saved file.
	static const std::string ms_header[2];
private:
	LEJsonReader* m_reader;
	LEJsonWriter* m_writer;

	struct UnassignedObjectInfo {
		LEObject**     ppObject;
		unsigned int   uuid;
		// it will be greater than zero only when LEObject is archived, not LEObject *
		int            objectByteCount;
	};
public:
	enum ArchivePhase {
		AP_COLLECT,  // collect LEObjects
		AP_WRITE,    // serialize all LEObjects and write
		AP_READ,     //
		AP_POST_READ,// recover all the pointer
		AP_MAX_ENUM = 0x7ffffff
	};
private:
	LEObject*                                   m_currentArchiveObject;
	ArchivePhase                                m_archivePhase;
	std::string                                 m_archivePath;
	std::unordered_map<LEObject*, int>          m_collectedObjects;
	std::unordered_map<unsigned int, LEObject*> m_objectMap;
	std::stack<std::string>                     m_currentPropertyNameStack;
	std::stack<int>                             m_objectIndexStack;
	int                                         m_vecDepth;
	std::vector<UnassignedObjectInfo>           m_unassignedObjectInfo;
	std::vector<LEObject*>                      m_unassignedObjectPointers;
	int                                         m_unassignedObjectPointersUsedCount;

	void assignLEObject(LEObject** ppObject, unsigned int uuid,unsigned int byteCount = 0) {
		LEObject* pObject = this->m_objectMap[uuid];
		if (!byteCount) {
			// If it is LEObject*, assign the address directly 
			(*ppObject) = pObject;
		}
		else {
			// If it is LEObject, we conduct memory copy 
			// and then, we should release the memory of the copied LEObject
			::memcpy(*ppObject, pObject, byteCount);
			LE_SAFE_RELEASE(pObject);
		}
	}
public:
	LEDataStream():m_vecDepth(0),
		m_unassignedObjectPointersUsedCount(0),
		m_currentArchiveObject(nullptr),
		m_reader(nullptr),m_writer(nullptr),
		m_archivePhase(AP_MAX_ENUM){}

	virtual ~LEDataStream() {}

	template<BasicType T>
	void archive(T& t,int vectorElementIndex = -1,bool fromMap = false) {
		if (this->m_archivePhase == AP_WRITE) {
			if (vectorElementIndex == -1) {
				this->m_writer->writeAt(t,
					                   "objects",
					                   this->m_objectIndexStack.top(),
					                   this->m_currentPropertyNameStack.top());
			}
			else {
				this->m_writer->writeAtCurrent(t, vectorElementIndex);
			}
		}
		else {
			if (vectorElementIndex == -1) {
				if (std::is_integral<T>::value) {
					t = this->m_reader->readInt("objects", 
						                        this->m_objectIndexStack.top(), 
						                        this->m_currentPropertyNameStack.top());
				}
				else {
					t = this->m_reader->readFloat("objects",
						                          this->m_objectIndexStack.top(), 
						                          this->m_currentPropertyNameStack.top());
				}
			}
			else {
				if (std::is_integral<T>::value) {
					t = this->m_reader->readIntAtCurrent(vectorElementIndex);
				}
				else {
					t = this->m_reader->readFloatAtCurrent(vectorElementIndex);
				}
			}
		}
	}

	template<class T>
	void archive(T& object, int vectorElementIndex = -1) {
		unsigned int byteCount = sizeof(T);
		auto str = typeid(T).name();
		if (std::is_base_of<LEObject, T>::value) {
			if (this->m_archivePhase == AP_READ) {
				this->m_unassignedObjectPointers.push_back((LEObject*)&object);
				this->archive(this->m_unassignedObjectPointers.back(), vectorElementIndex, byteCount);
			}
			else {
				LEObject* pObject = (LEObject*)&object;
				this->archive(pObject, vectorElementIndex);
			}
		}
		else {
			char* objectAddr = (char*)&object;
			if (this->m_archivePhase == AP_WRITE) {
				std::string dataString = LEBase64::encode(objectAddr, byteCount);
				if (vectorElementIndex == -1) {
					this->m_writer->writeAt(dataString,
						"objects",
						this->m_objectIndexStack.top(),
						this->m_currentPropertyNameStack.top());
				}
				else {
					this->m_writer->writeAtCurrent(dataString, vectorElementIndex);
				}
			}
			else {
				std::string encodedObject;
				if (vectorElementIndex == -1) {
					encodedObject = this->m_reader->readString("objects",
						this->m_objectIndexStack.top(),
						this->m_currentPropertyNameStack.top());
				}
				else {
					encodedObject = this->m_reader->readStringAtCurrent(vectorElementIndex);
				}
				const char* dataStr = LEBase64::decode(encodedObject.c_str(), encodedObject.size());
				::memcpy(objectAddr, dataStr, sizeof(char) * byteCount);
			}
		}
	}

	void archiveLEObject(LEObject** &pObject, int vectorElementIndex, int byteCount) {
		this->m_unassignedObjectPointers.push_back(*pObject);
		this->archive(this->m_unassignedObjectPointers.back(),vectorElementIndex, byteCount);
	}

	void archive(std::string& str, int vectorElementIndex = -1) {
		if (this->m_archivePhase == AP_WRITE) {
			if (vectorElementIndex == -1) {
				this->m_writer->writeAt(str,
					"objects",
					this->m_objectIndexStack.top(),
					this->m_currentPropertyNameStack.top());
			}
			else {
				this->m_writer->writeAtCurrent(str, vectorElementIndex);
			}
		}
		else {
			if (vectorElementIndex == -1) {
				str.assign(this->m_reader->readString("objects", 
					                                  this->m_objectIndexStack.top(), 
					                                  this->m_currentPropertyNameStack.top()));
			}
			else {
				str.assign(this->m_reader->readStringAtCurrent(vectorElementIndex));
			}
		}
	}

	template<class Key, class Value>
	void archive(std::unordered_map<Key, Value>& map,int vectorElementIndex = -1) {
		if (this->m_archivePhase == AP_WRITE) {
			this->m_writer->pushValueLocation("objects",
				                              this->m_objectIndexStack.top(),
				                              this->m_currentPropertyNameStack.top());
			this->m_writer->writeAtCurrent(map.size(), "size");
			int index = 0;
			std::vector<Key>   keyVector(map.size());
			std::vector<Value> valueVector(map.size());
			for (auto [key, value] : map) {
				keyVector[index] = key;
				valueVector[index] = value;
				index++;
			}
			this->archive(keyVector, true /*from map*/, true /*is key*/);
			this->archive(valueVector, true /*from map*/, false /*is key*/);
			this->m_writer->popValueLocation();
		}
		else {
			this->m_reader->pushValueLocation("objects",
				                              this->m_objectIndexStack.top(),
				                              this->m_currentPropertyNameStack.top());
			int size = this->m_reader->readIntAtCurrent("size");
			std::vector<Key>   keyVector(size);
			std::vector<Value> valueVector(size);
			this->archive(keyVector, true /*from map*/, true /*is key*/);
			this->archive(valueVector, true /*from map*/, false /*is key*/);
			for (int i = 0; i < size;i++) {
				map[keyVector[i]] = valueVector[i];
			}
			this->m_reader->popValueLocation();
		}
	};

	template<class T>
	void archive(T* &pObject, int vectorElementIndex = -1) {
		if (std::is_base_of<LEObject,T>::value) {
			LEObject** ppObject = (LEObject**)&pObject;
			this->archive(*ppObject, vectorElementIndex, 0);
		}
		else {
			// [Warning]: non-LEObject-derived object pointer can not be archived. 
		}
	}

	template<class T>
	void archive(std::vector<T>& vec,int vectorElementIndex = -1,bool fromMap = false,bool isKey = true) {
		if (this->m_archivePhase == AP_WRITE) {
			if (fromMap) {
				this->m_writer->pushValueLocation("objects",
					this->m_objectIndexStack.top(),
					this->m_currentPropertyNameStack.top(), isKey ? "keys" : "values");
				for (int i = 0; i < vec.size(); i++) {
					this->archive(vec[i], i);
				}
				this->m_writer->popValueLocation();
			}
			else {
				if (m_vecDepth == 0) {
					this->m_writer->pushValueLocation("objects",
						                              this->m_objectIndexStack.top(),
						                              this->m_currentPropertyNameStack.top());
				}
				else {
					this->m_writer->pushValueLocationAfterCurrent(vectorElementIndex);
				}
				this->m_writer->writeAtCurrent(vec.size(), "size");
				this->m_writer->pushValueLocationAfterCurrent("values");
				m_vecDepth++;
				for (int i = 0; i < vec.size(); i++) {
					this->archive(vec[i], i);
				}
				m_vecDepth--;
				this->m_writer->popValueLocation();
				this->m_writer->popValueLocation();
			}
		}
		else {
			if (fromMap) {
				this->m_reader->pushValueLocation("objects",
					                              this->m_objectIndexStack.top(),
					                              this->m_currentPropertyNameStack.top(), isKey ? "keys" : "values");
				for (int i = 0; i < vec.size(); i++) {
					this->archive(vec[i], i);
				}
				this->m_reader->popValueLocation();
			}
			else {
				if (m_vecDepth == 0) {
					this->m_reader->pushValueLocation("objects",
						                              this->m_objectIndexStack.top(),
						                              this->m_currentPropertyNameStack.top());
				}
				else {
					this->m_reader->pushValueLocationAfterCurrent(vectorElementIndex);
				}
				vec.resize(this->m_reader->readIntAtCurrent("size"));
				this->m_reader->pushValueLocationAfterCurrent("values");
				m_vecDepth++;
				for (int i = 0; i < vec.size(); i++) {
					this->archive(vec[i], i);
				}
				m_vecDepth--;
				this->m_reader->popValueLocation();
				this->m_reader->popValueLocation();
			}
		}
	}

	// the parameter 'byteCount' is used for archive LEObject
	void archive(LEObject*& rootObject, int vectorElementIndex = -1, int byteCount = 0);

	void setArchiveFilePath(const std::string& path);

	void setArchivePhase(ArchivePhase phase);

	void pushCurrentPropertyName(const std::string& name);

	void popCurrentPropertyName() {
		this->m_currentPropertyNameStack.pop();
	}

	void saveObject(LEObject* object);

	LEObject* loadObject();
};
#endif // !LE_DATA_STREAM_H