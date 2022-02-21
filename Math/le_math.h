#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Math
 * \File:    le_math.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_MATH_H
#define LE_MATH_H

// LE Includes
#include "../Common/le_global.h"

LE_NAMESPACE_BEGIN(Math)

static int getNextPow2(int value) {
	float f = (float)(value - 1);
	return 1 << ((*(unsigned int*)(&f) >> 23) - 126);
}

static LEBool isPow2(int value) {
	return (value & (value - 1)) == 0;
}

template<BasicType T1, BasicType T2>
static inline auto getMax(T1 a, T2 b)->decltype(a > b ? a : b){
	return a > b ? a : b;
}

template<BasicType T1, BasicType T2>
static inline auto getMin(T1 a, T2 b)->decltype(a < b ? a : b) {
	return a < b ? a : b;
}

template<BasicType T>
static inline T abs(T& v) {
	return v > 0 ? v : -v;
}

template<BasicType T>
static T clamp(T min, T max, T value) {
	if (value < min) {
		value = min;
	}
	else if (value > max) {
		value = max;
	}
	return value;
}

template<BasicType T>
static T clamp01(T value) {
	if (value < 0) {
		value = 0;
	}
	else if (value > 1) {
		value = 1;
	}
	return value;
}

static const int LE_PRIME_COUNT = 28;
static const uint32_t le_prime_list[LE_PRIME_COUNT] = {
 53ul, 97ul, 193ul, 389ul, 769ul,
 1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
 49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
 1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
 50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
 1610612741ul, 3221225473ul, 4294967291ul
};

static uint32_t getNextPrime(uint32_t capacity) {
	uint32_t i = 0;
	for (; i < LE_PRIME_COUNT; ++i) {
		if (le_prime_list[i] > capacity)
			return le_prime_list[i];
	}
	return le_prime_list[i];
}
LE_NAMESPACE_END
#endif // !LE_MATH_H
