#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Common
 * \File:    le_type_traits.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_TYPE_TRAITS
#define LE_TYPE_TRAITS

// LE Includes
#include "le_object.h"

// STD Includes
#include <vector>
#include <string>
#include <type_traits>

template<typename T>
struct is_basic_type {
	using value_type = T;
	constexpr const static bool value = std::is_floating_point<T>::value ||
		std::is_integral<T>::value;
};

template<typename T>
struct is_range_type {
	using value_type = T;
	constexpr const static bool value = is_basic_type<T>::value &&
		!std::is_enum<T>::value &&
		!std::is_same<bool, T>::value;
};

template<typename T>
concept UnionType = std::is_union<T>::value;

template<typename T>
concept BasicType = is_basic_type<T>::value;

template<typename T>
concept ClassType = std::is_class<T>::value;

template<typename T>
concept RangeType = is_range_type<T>::value;

template<typename T>
concept EnumType = std::is_enum<T>::value;

template<typename T>
struct is_le_object :std::false_type {};

template<>
struct is_le_object<LEObject> :std::true_type {};

template<typename T>
struct is_le_object_pointer :std::false_type {};

template<>
struct is_le_object_pointer<LEObject*> :std::true_type {};

template<typename T>
struct is_string :std::false_type {};

template<>
struct is_string<std::string> :std::true_type {};

template<typename T>
struct is_vector :std::false_type {};

template<typename U>
struct is_vector<std::vector<U>> :std::true_type {};

#endif // !LE_TYPE_TRAITS