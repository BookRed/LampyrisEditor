#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Utility
 * \File:    le_random.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_RANDOM_H
#define LE_RANDOM_H

// STD Includes
#include <random>
#include <numeric>

namespace LEDetail {
	template<typename T>
	static int le_random_internal(T distribution) {
		std::random_device randomDev;
		std::mt19937 generator(randomDev());
		return distribution(generator);
	}
}

static int le_random_range_int(int lower,int upper) {
	return LEDetail::le_random_internal(std::uniform_int_distribution(lower, upper));
}

static float le_random_range_float(float lower, float upper) {
	return LEDetail::le_random_internal(std::uniform_real_distribution(lower, upper));
}

static int le_random_int() {
	return LEDetail::le_random_internal(std::uniform_int_distribution(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()));
}

static float le_random_float(float lower, float upper) {
	return LEDetail::le_random_internal(std::uniform_real_distribution(std::numeric_limits<float>::min(), std::numeric_limits<float>::max()));
}
#endif // !LE_RANDOM_H