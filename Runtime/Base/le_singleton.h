/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_singleton.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_SINGLETON_H
#define LE_SINGLETON_H
#include "le_global.h"

#include <type_traits>
template<typename T>
class Singleton {
public:
	static T* getInstance() {
		static T* instance = new T;
		return instance;
	}
};

#endif // !LE_SINGLETON_H