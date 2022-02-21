/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Resource
 * \File:    le_character_info.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#pragma once

#ifndef LE_CHARACTER_INFO_H
#define LE_CHARACTER_INFO_H

struct LECharacterInfo {
	unsigned int    x0 = 0;      // left
	unsigned int    y0 = 0;      // top 
	unsigned int    x1 = 0;      // right
	unsigned int    y1 = 0;      // bottom

	// offset is relative to the previous character  
	unsigned int    offsetX = 0;
	unsigned int    offsetY = 0;
	/*
	bool            valid = false;
	*/
};
#endif // !LE_CHARACTER_INFO_H