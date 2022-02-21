#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Utility
 * \File:    le_dir.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_DIR_H
#define LE_DIR_H

// STL Includes
#include <vector>
#include <filesystem>

class LEDir {
private:
	static int getFileNames(const std::string& dir, std::vector<LEFile*>& filenames)
	{
		std::filesystem::path path(dir);
		if (!std::filesystem::exists(path)) {
			return;
		}
		std::filesystem::directory_iterator end_iter;
		for (std::filesystem::directory_iterator iter(path); iter != end_iter; ++iter) {
			if (std::filesystem::is_regular_file(iter->status())) {
				filenames.push_back(new LEFile(iter->path().wstring()));
			}
			if (std::filesystem::is_directory(iter->status())) {
				LEDir::getFileNames(iter->path().wstring(), filenames);
			}
		}
		return filenames.size();
	}

public:
	static std::vector<LEFile*> getFileNames(const LEString& dir) {
		std::vector<LEFile*> filenames;
		LEDir::getFileNames(dir, filenames);
		return filenames;
	}

	void remove() {

	}

	void rename() {

	}

	void copy() {

	}
};

class LEDir {
private:
	static int getFileNames(const LEString& dir, std::vector<LEFile*>& filenames)
	{
		Path path(dir);
		if (!std::filesystem::exists(path)) {
			return;
		}
		std::filesystem::directory_iterator end_iter;
		for (std::filesystem::directory_iterator iter(path); iter != end_iter; ++iter) {
			if (std::filesystem::is_regular_file(iter->status())) {
				filenames.push_back(new LEFile(iter->path().wstring()));
			}
			if (std::filesystem::is_directory(iter->status())) {
				LEDir::getFileNames(iter->path().wstring(), filenames);
			}
		}
		return filenames.size();
	}

public:
	static std::vector<LEFile*> getFileNames(const LEString& dir) {
		std::vector<LEFile*> filenames;
		LEDir::getFileNames(dir, filenames);
		return filenames;
	}

	void remove() {

	}

	void rename() {

	}

	void copy() {

	}
};

#endif // !LE_DIR_H

