#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_hash.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_HASH
#define LE_HASH
#include <vector>

template <typename T>
inline void le_hash_combine(std::size_t& seed, const T& val) {
	seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename T>
inline void le_hash_value(std::size_t& seed, const T& val) {
	le_hash_combine(seed, val);
}
template <typename T, typename... Types>
inline void le_hash_value(std::size_t& seed, const T& val, const Types &... args) {
	le_hash_combine(seed, val);
	le_hash_value(seed, args...);
}

template <typename... Types>
inline std::size_t le_hash_value(const Types &... args) {
	std::size_t seed = 0;
	le_hash_value(seed, args...);
	return seed;
}

struct le_pair_hash {
	template <class T1, class T2>
	std::size_t operator()(const std::pair<T1, T2>& p) const {
		return le_hash_value(p.first, p.second);
	}
};

#endif //!LE_HASH