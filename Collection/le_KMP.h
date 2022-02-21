#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Utility/Algorithms
 * \File:    le_KMP.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_KMP
#define LE_KMP

// LE Includes

#include "../Collection/le_vector.h"
LE_NAMESPACE_BEGIN(Utility)

static LEVector<int> le_kmp_get_next(int length, const LEWChar* target) {
	uint32_t i = 0, j = -1;
	LEVector<int> next(length);
	next[0] = -1;
	if (length == 1) {
		return next;
	}
	for(int j = 0,i = 2;i<length;i++) {
		if (target[i - 1] == target[j]) {
			next[i++] = ++j;
		}
		else if (j == 0) {
			next[i++] = 0;
		}
		else {
			j = next[j];
		}
	}
	return next;
}
static int le_kmp_match(const LEWChar* target,
					      const LEWChar* pattern) {
	int len1 = ::wcslen(target);
	int len2 = ::wcslen(pattern);

	if (!len1 || !len2 || len1 < len2) {
		return -1;
	}
	auto next = le_kmp_get_next(len1,target);
	int i = 0, j = 0;
	while (i < len1 && j < len2) {
		if (j == -1 || target[i] == pattern[j]) {
			j++; i++;
		}
		else if (next[len2] == -1) {
			len1++;
		}
		else {
			j = next[j];
		}
	}
	if (j == len2) {
		return i - j;
	}
	else {
		return -1;
	}
}

// reversed KMP, is used for the implementation of 'LEString::findLast'
static LEVector<int> le_kmp_get_next_reversed(const LEWChar* target) {
	int len = ::wcslen(target);
	int i = len - 1, j = -1;
	LEVector<int> next; 
	next[len - 1] = -1;
	while (i >= 0) {
		if (j == -1 || target[i] == target[j]) {
			next[--i] = ++j;
		}
		else {
			j = next[j];
		}
	}
}
static int le_kmp_match_reversed(const LEWChar* target,
							     const LEWChar* pattern) {
	int len1 = ::wcslen(target);
	int len2 = ::wcslen(pattern);

	LEVector<int> next = le_kmp_get_next_reversed(target);
	int i = len1 - 1, j = len2 - 1;
	while (i >= 0  && j >= 0) {
		if (j == len2 - 1 || target[i] == pattern[j]) {
			j--; i--;
		}
		else {
			j -= next[j];
		}
	}
	if (j == 0) {
		return i + j;
	}
	else {
		return -1;
	}
}
LE_NAMESPACE_END
#endif // !LE_KMP
