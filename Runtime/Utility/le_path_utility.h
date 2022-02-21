#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Utility
 * \File:    le_path_utility.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "../Base/le_global.h"
#include "../Base/le_assert.h"
#include "le_string_convert.h"

// STL Includes
#include <filesystem>

#ifdef LE_PLATFORM_WINDOWS
#include <shlobj.h>
#endif // ! LE_PLATFORM_WINDOWS

typedef std::filesystem::path	   Path;
typedef std::streampos			   StreamPos;

class LEPathUtility {
public:
	// returns the path of directory where the file exists
	static std::string getFileDirectoryPath(const Path& path) {
		std::string filePath(std::filesystem::absolute(path).string());
		int i1 = filePath.find_last_of("\\");
		int i2 = filePath.find_last_of("/");
		std::string fileDirPath = filePath.substr(0, i1>i2?i1:i2);
		return fileDirPath;
	}

	// file: 'C:/XXX/123.txt'
	// returns: '123' 
	static std::string getPrefix(const Path& filePath) {
		std::string pathStr = filePath.string();
		int index = pathStr.find_first_of(".");
		return pathStr.substr(0, index);
	}


	// file: 'C:/XXX/123.txt'
	// returns: 'txt' 
	static std::string getSuffix(const Path& filePath) {
		std::string pathStr = filePath.string();
		int index = pathStr.find_first_of(".");
		return pathStr.substr(index + 1, pathStr.size() - index - 1);
	}

	// file: 'C:/XXX/123.txt'
	// returns: '123.txt' 
	static std::string getFileName(const Path& filePath) {
		std::string pathStr = filePath.string();
		int index = pathStr.find_first_of("/");
		return pathStr.substr(index + 1, pathStr.size() - index - 1);
	}

	// target path: 'C:/XX/XXX'
	// if target path exists, it returns 'C:/XX/XXX (1)'
	// if 'C:/XX/XXX (1)' exists, it returns 'C:/XX/XXX (2)'
	static std::string getUnusedPath(const Path& originPath) {
		if (!std::filesystem::exists(originPath)) {
			return originPath.string();
		}
		else {
			std::string absPath = std::filesystem::absolute(originPath).string();
			if (std::filesystem::is_directory(originPath)) {	// is folder?
				int index = 1;
				while (true) {
					std::string mayUnnsedFilePath = std::filesystem::absolute(originPath).string() +
						" (" + std::to_string(index) + ")";
					if (!std::filesystem::exists(mayUnnsedFilePath)) {
						return mayUnnsedFilePath;
					}
					else {
						index++;
					}
				}
			}
			else { // is file
				int index = 1;
				while (true) {
					std::string absPath = getFileDirectoryPath(originPath);
					std::string prefix = getPrefix(originPath);
					std::string suffix = getSuffix(originPath);
					std::string mayUnnsedFilePath = absPath +
						"/" +
						prefix +
						" (" + std::to_string(index) + ")" +
						suffix;
					if (!std::filesystem::exists(mayUnnsedFilePath)) {
						return mayUnnsedFilePath;
					}
					else {
						index++;
					}
				}
			}
		}
	}

	static std::string getAppDataSavePath() {
	#ifdef LE_PLATFORM_WINDOWS
		wchar_t m_lpszDefaultDir[MAX_PATH] = { 0 };
		wchar_t szDocument[MAX_PATH] = { 0 };

		LPITEMIDLIST pidl = NULL;
		::SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
		if (pidl && SHGetPathFromIDList(pidl, szDocument)) {
			GetShortPathName(szDocument, m_lpszDefaultDir, _MAX_PATH);
		}
		return le_wchar_to_char(std::wstring(m_lpszDefaultDir) + L"\\Lampyris\\");
	#endif // ! LE_PLATFORM_WINDOWS
	}

	static bool isDirectory(const std::string& path) {
		return std::filesystem::is_directory(path);
	}
};
