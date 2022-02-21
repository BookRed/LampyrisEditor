/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Serialization
 * \File:    le_serialization_manager.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_SERIALIZATION_MANAGER_H
#define LE_SERIALIZATION_MANAGER_H

// LE Includes
#include "le_serialization.h"
#include "../Collection/le_map.h"
#include "../Collection/le_string.h"
#include "../Common/le_subsystem.h"

LE_USING_NAMESPACE(Collection);

class SerializationManager:extend Singleton<SerializationManager> {
private:
	std::istringstream                m_iss;
	std::ostringstream                 m_oss;
	LEMap<LEString, std::string>       m_map;
public:
	SerializationManager() {

	}

	~SerializationManager() {

	}

	template <class T>
	void serialize(T &object, const LEString& name) {
		boost::archive::binary_oarchive oa(this->m_oss);
		oa << object;
		m_map[name] = this->m_oss.str();
		this->m_oss.clear();
	}
	
	template <class T>
	T deserialize(const LEString& name) {
		this->m_iss = std::istringstream(this->m_map[name]);
		boost::archive::binary_iarchive ia(this->m_iss);
		T t;
		ia >> t;
		return t;
	}
	void doStartUp() {

	}

	void doShutDown() {

	}
};
#endif // !LE_SERIALIZATION_MANAGER_H