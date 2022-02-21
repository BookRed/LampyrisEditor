/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Base
 * \File:    le_enum_property.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_enum_property.h"
#include "../Serialization/le_data_stream.h"

template<EnumType T>
void LEEnumProperty<T>::archive(LEDataStream& stream, void* objectPtr) {
	void* address = this->getPropertyPointer(objectPtr);
	std::vector<T>& vec = (*(std::vector<T>*)(address));
	stream.archive(vec);
}
