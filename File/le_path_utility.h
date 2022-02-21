#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  File
 * \File:    le_path_utility.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "../Common/le_global.h"
#include "../Collection/le_string.h"
#include "../Debug/le_assert.h"

// Boost Includes
#include <boost/filesystem.hpp>

#ifdef LE_PLATFORM_WINDOWS
#include <shlobj.h>
#endif 
LE_USING_NAMESPACE(Collection);

typedef boost::filesystem::wfstream  FileStream;
typedef boost::filesystem::path	     Path;
typedef std::streampos				 StreamPos;

// returns the path of directory where the file exists
static std::wstring le_get_file_dir_Path(const Path& path) {
	std::wstring filePath(boost::filesystem::absolute(path).wstring());
	int index = filePath.find_last_of(L"/");
	std::wstring fileDirPath = filePath.substr(0, index);
	return fileDirPath;
}

// file: 'C:/XXX/123.txt'
// returns: '123' 
static std::wstring le_get_prefix_of_file_name(const Path& filePath) {
	std::wstring pathStr = filePath.wstring();
	int index = pathStr.find_first_of(L"."); 
	return pathStr.substr(0, index);
}


// file: 'C:/XXX/123.txt'
// returns: 'txt' 
static std::wstring le_get_suffix_of_file_name(const Path& filePath) {
	std::wstring pathStr = filePath.wstring();
	int index = pathStr.find_first_of(L".");
	return pathStr.substr(index + 1, pathStr.size() - index - 1);
}

// file: 'C:/XXX/123.txt'
// returns: '123.txt' 
static std::wstring le_get_file_name(const Path& filePath) {
	std::wstring pathStr = filePath.wstring();
	int index = pathStr.find_first_of(L"/");
	return pathStr.substr(index + 1, pathStr.size() - index - 1);
}

// target path: 'C:/XX/XXX'
// if target path exists, it returns 'C:/XX/XXX (1)'
// if 'C:/XX/XXX (1)' exists, it returns 'C:/XX/XXX (2)'
static std::wstring le_get_unused_path(const Path& originPath) {
	if (!boost::filesystem::exists(originPath)) {
		return originPath.wstring();
	}
	else {
		std::wstring absPath = boost::filesystem::absolute(originPath).wstring();
		if (boost::filesystem::is_directory(originPath)) {	// is folder?
			int index = 1;
			while (true) {
				std::wstring mayUnnsedFilePath = boost::filesystem::absolute(originPath).wstring() +
					L" (" + std::to_wstring(index) + L")";
				if (!boost::filesystem::exists(mayUnnsedFilePath)) {
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
				std::wstring absPath = le_get_file_dir_Path(originPath);
				std::wstring prefix = le_get_prefix_of_file_name(originPath);
				std::wstring suffix = le_get_suffix_of_file_name(originPath);
				std::wstring mayUnnsedFilePath = absPath +
					L"/" +
					prefix +
					L" (" + std::to_wstring(index) + L")" +
					suffix;
				if (!boost::filesystem::exists(mayUnnsedFilePath)) {
					return mayUnnsedFilePath;
				}
				else {
					index++;
				}
			}
		}
	}
}

static LEString le_get_app_data_save_path() {
	LEWChar m_lpszDefaultDir[MAX_PATH] = { 0 };
	LEWChar szDocument[MAX_PATH] = { 0 };

	LPITEMIDLIST pidl = NULL;
	::SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
	if (pidl && SHGetPathFromIDList(pidl, szDocument))
	{
		GetShortPathName(szDocument, m_lpszDefaultDir, _MAX_PATH);
	}
	return LEString(m_lpszDefaultDir) + L"\\Lampyris";
}