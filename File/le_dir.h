#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  File
 * \File:    le_dir.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_DIR_H
#define LE_DIR_H

class LEDir {
private:
	static int getFileNames(const LEString& dir, std::vector<LEFile*>& filenames)
	{
		Path path(dir);
		if (!boost::filesystem::exists(path)) {
			return;
		}
		boost::filesystem::directory_iterator end_iter;
		for (boost::filesystem::directory_iterator iter(path); iter != end_iter; ++iter) {
			if (boost::filesystem::is_regular_file(iter->status())) {
				filenames.push_back(new LEFile(iter->path().wstring()));
			}
			if (boost::filesystem::is_directory(iter->status())) {
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
		if (!boost::filesystem::exists(path)) {
			return;
		}
		boost::filesystem::directory_iterator end_iter;
		for (boost::filesystem::directory_iterator iter(path); iter != end_iter; ++iter) {
			if (boost::filesystem::is_regular_file(iter->status())) {
				filenames.push_back(new LEFile(iter->path().wstring()));
			}
			if (boost::filesystem::is_directory(iter->status())) {
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

