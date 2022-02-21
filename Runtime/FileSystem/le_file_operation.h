/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Utility
 * \File:    le_file_operation.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#pragma once

// all basic file-relative operations are declared in this file

#ifndef LE_FILE_OPERATION_H
#define LE_FILE_OPERATION_H

// LE-Base Includes
#include "../Base/le_global.h"

typedef enum {
	SUCCESS = 0,
	FILE_NOT_FOUND = 1,
	FILE_CREATE_FAILED = 2,
	DIR_NOT_FOUND = 3,
	DIR_CREATE_FAILED = 4
}LE_FILE_OP_RESULT;

LE_FILE_OP_RESULT le_file_create_file() {

}

LE_FILE_OP_RESULT le_file_create_dir() {

}

static void le_file_exists(const std::string& path) {
	if (_access(path.c_str(), 0) == -1) {
		return false;
	}
	return true;
}

static std::string le_file_read_all(const std::string& path) {
	std::ifstream file(path.c_str());
	std::string   content;
	if (file.is_open()) {
		content = std::string(std::istreambuf_iterator<char>{file}, 
						      std::istreambuf_iterator<char>{});
	}
	return content;
}
#endif //!LE_FILE_OPERATION_H