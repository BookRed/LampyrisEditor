/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Resource
 * \File:    le_resource.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_RESOURCE_H
#define LE_RESOURCE_H

// LE Includes
#include "../Base/le_object.h"
#include "../Utility/le_file.h"

#ifdef RT_FONT
#undef RT_FONT
#endif // !RT_FONT
class LEResource {
public:
	enum ResourceType {
		RT_MESH,
		RT_SHADER,
		RT_TEXTURE,
		RT_MATERIAL,
		RT_FONT,
		RT_PREFAB,
		RT_AUDIO,
		RT_MAX_ENUM
	};
protected:
	uint32_t      m_resourceID{ 0 };
	std::string   m_resourcePath;
public:
	LEResource() {}

	LEResource(const std::string& filePath):m_resourcePath(filePath){}

	std::string getResourcePath();

	std::string getSerializedResourcePath();

	virtual ~LEResource() {}

	virtual ResourceType getType() const = 0;

	virtual void onLoaded() = 0;

	virtual void onUnloaded() = 0;
};

class LEResourceProxyBase:public LEObject {
public:
	enum LoadMode {
		Synchronous,
		Asynchronous
	};

private:
	bool m_isGCEnabled;
	bool m_isLoaded;

public:
	bool isLoaded() const {
		return this->m_isLoaded;
	}
};

template<class T>
class LEResourceProxy:public LEResourceProxyBase {
private:
	T* m_defaultResource;
	T* m_loadedResource;

public:
	LEResourceProxy() {

	}

	virtual ~LEResourceProxy() {

	}

	static LEResourceProxy<T>* createFromResource() {

	}
};
#endif // !LE_RESOURCE_H
