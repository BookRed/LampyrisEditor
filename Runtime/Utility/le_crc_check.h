#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Utility
 * \File:    le_crc_check.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// Reference:
// [1]:https://www.cnblogs.com/LyShark/p/13656473.html

// STD Includes
#include <cstdint>
#include <string>

class LECRCCheck {
public:
	static uint32_t calculate(const std::string& fname);
};
