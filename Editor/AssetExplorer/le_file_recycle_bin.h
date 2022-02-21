#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_file_recycle_bin.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_FILE_RECYCLE_BIN_H
#define LE_FILE_RECYCLE_BIN_H

#include <QDir>
#include <QString>
#include <vector>
#include <unordered_map>
#if defined(_WIN32)
#include <Windows.h>
#endif

class FileRecycleBin
{
	typedef struct {
		QString srcPath;
		QString removedPath;
	} RestoreInfo;

	using RestoreCode      = int;
	using RestoreInfoList  = std::vector<RestoreInfo>;

private:
	std::unordered_map<RestoreCode, RestoreInfoList> mRestoreInfoMap;
	static constexpr const char* sTargetDirPath = "D:/$LE_RECYCLE_BIN/";
	void createRecycleBinRootDir() {
		QString tragetDirPath = sTargetDirPath;
		QDir dir;
		if (!dir.exists(tragetDirPath)) {
			dir.mkdir(tragetDirPath);
#if defined(_WIN32)
			SetFileAttributes((LPCWSTR)tragetDirPath.unicode(), FILE_ATTRIBUTE_HIDDEN);
#endif
		}
	}

	void deleteAllFiles() {
		QDir dir;
		if (dir.exists(sTargetDirPath)) {
			dir.remove(sTargetDirPath);
		}
	}
public:
	FileRecycleBin() {
		createRecycleBinRootDir();
		srand(unsigned int(time(NULL)));
	}

	~FileRecycleBin() {
		deleteAllFiles();
	}

	RestoreCode moveFiles(const std::vector<QString>& filePaths) {
		RestoreInfoList restoreInfoList;
		int restoreCode = rand();
		QString foldPath = QString::asprintf("%s%d", sTargetDirPath, restoreCode);
		QDir foldDir;
		if (!foldDir.exists(foldPath)) {
			if (!foldDir.mkdir(foldPath)) {
				return -1;
			}
		}
		for (const auto& filePath : filePaths) {				
			QFile f(filePath);
			RestoreInfo info;
			info.srcPath = filePath;
			info.removedPath = sTargetDirPath + 
				               QString::number(restoreCode) + 
				               "/" + QFileInfo(f).fileName();
			restoreInfoList.push_back(info);
			f.rename(info.srcPath,info.removedPath);
		}
		mRestoreInfoMap[restoreCode] = restoreInfoList;
		return restoreCode;
	}
	void restoreFiles(const RestoreCode code) {
		if (mRestoreInfoMap.find(code) != mRestoreInfoMap.end()) {
			RestoreInfoList restoreInfoList = mRestoreInfoMap[code];
			for (const auto& restoreInfo : restoreInfoList) {
				QFile f(restoreInfo.removedPath);
				f.rename(restoreInfo.removedPath,restoreInfo.srcPath);
			}																																																																												
		}
	}
};

#endif //!LE_FILE_RECYCLE_BIN_H