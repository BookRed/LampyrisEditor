/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Collection
 * \File:    le_string_convert.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#pragma once

#ifndef LE_STRING_CONVERT_H
#define LE_STRING_CONVERT_H

// STL Includes
#include <string>

// LE-Base Includes
#include "../Common/le_global.h"

static std::wstring le_char_to_wchar(const std::string &str) {
	int nLen = MultiByteToWideChar(CP_ACP, 
		                           MB_PRECOMPOSED, 
		                           str.c_str(), 
		                           -1, 
		                           NULL, 
		                           0);
	if (nLen == 0) {
		return std::wstring();
	}
	wchar_t* wstr = new wchar_t[nLen];
	MultiByteToWideChar(CP_ACP, 
		                MB_PRECOMPOSED, 
		                str.c_str(), 
		                -1, 
		                wstr, 
		                nLen);
	std::wstring returnValue = wstr;
	LE_SAFE_RELEASE_ARRAY(wstr);
	return returnValue;
}

static std::string le_wchar_to_char(const std::wstring &wstr) {
	int nLen = WideCharToMultiByte(CP_ACP, 
		                           0, 
		                           wstr.c_str(), 
		                           -1, 
		                           NULL, 
		                           0, 
		                           NULL, 
		                           NULL);
	if (nLen == 0) {
		return std::string();
	}
	char* str = new char[nLen];
	WideCharToMultiByte(CP_ACP, 
		                0, 
		                wstr.c_str(), 
		                -1, 
		                str, 
		                nLen, 
		                NULL, 
		                NULL);
	std::string returnValue = str;
	LE_SAFE_RELEASE_ARRAY(str);
	return returnValue;
}

#endif //! LE_STRING_CONVERT_H