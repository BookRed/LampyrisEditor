#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Utility
 * \File:    le_file.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_FILE_H
#define LE_FILE_H

// LE Includes
#include "../Base/le_global.h"
#include "le_path_utility.h"

// STL Includes
#include <fstream>
#include <filesystem>

class LEFile {
	typedef std::filesystem::path	   Path;
private:
	std::fstream m_fstream;
	Path         m_path;
public:
	LEFile(const std::string& path);

	void writeLine(const char* content);

	void write(const char* content);

	std::string read();

	static bool isExists(const std::string& path);

	static bool create(const std::string& path);

	static bool createFile(const std::string& path);

	LEBool isOpened() const;

	void close();

	void flush();

	std::string getFileFullPath() const {
		return this->m_path.string();
	}

	std::string getFileName() const;

	std::string getFileDirectoryPath() const;

	std::string getFilePrefix() const;

	std::string getFileSuffix() const;

	unsigned int getFileSize() const;

	LEFile copy();

	void remove();

	void rename(const std::string& newFileName);
};

#endif // !LE_FILE_H