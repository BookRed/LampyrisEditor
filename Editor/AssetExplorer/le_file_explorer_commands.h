#pragma once
/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_file_explorer_command.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_FILE_EXPLORER_COMMANDS_H
#define LE_FILE_EXPLORER_COMMANDS_H
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QFileInfo>

#include <vector>
#include "../../Common/le_global.h"
#include "le_file_recycle_bin.h"
#include "le_abstract_edtior_command.h"

enum class PasteMode {
	Copy,
	Cut,
	Undefined
};

enum class DeleteMode {
	MoveToReCycle,
	Completely,
	Undefined
};
class PasteCommand final:public AbstractCommand {
private:
	QString              mTargetPathToPaste;
	PasteMode            mPasteMode;
	std::vector<QString> mFileOrDirToCopy;
	std::vector<QString> mFileOrDirPastedPaths;

	QString getUnusedFileOrDirPath(const QString& targetPath) {
		QFileInfo info(targetPath);
		QDir dir;
		if (!dir.exists(targetPath)) {
			return targetPath;
		}
		else {
			if (info.isDir()) {
				int index = 1;
				while (true) {
					QString mayUnnsedFilePath = QString("%1%2 (%3)").
						arg(info.absolutePath()).
						arg(info.baseName()).
						arg(index);

					if (!dir.exists(mayUnnsedFilePath)) {
						return mayUnnsedFilePath;
					}
					else {
						index++;
					}
				}
			}
			else if (info.isFile()) {
				int index = 1;
				while (true) {
					QString mayUnnsedFilePath = QString("%1/%2 (%3).%4").
						arg(info.absolutePath()).
						arg(info.baseName()).
						arg(index).
						arg(info.suffix());

					if (!dir.exists(mayUnnsedFilePath)) {
						return mayUnnsedFilePath;
					}
					else {
						index++;
					}
				}
			}
		}
	}

	bool doDirectoryCopy(const QDir& srcDir, const QDir& targetDir,const QString & newFileOrDirPath) {
		QFileInfo info(srcDir.path());
		if (!targetDir.exists(newFileOrDirPath)) {
			if (!targetDir.mkdir(newFileOrDirPath)){
				return false;
			}
		}

		QFileInfoList fileInfoList = srcDir.entryInfoList();
		foreach(QFileInfo fileInfo, fileInfoList) {
			if (fileInfo.fileName() == "." || 
				fileInfo.fileName() == "..")
				continue;
			if (fileInfo.isDir()) {
				if (!doDirectoryCopy(fileInfo.filePath(), 
					 targetDir.filePath(fileInfo.fileName()), newFileOrDirPath))
					return false;
			}
			else {
				QString targetPath = newFileOrDirPath + "/"+fileInfo.fileName();
				if (!QFile::copy(fileInfo.absoluteFilePath(),
					             newFileOrDirPath + fileInfo.fileName())) {
					return false;
				}
			}
		}
		return true;
	}
public:
	void execute() override{
		this->mFileOrDirPastedPaths.clear();
		this->mFileOrDirPastedPaths.reserve(this->mFileOrDirToCopy.size());
		QDir targetDir(this->mTargetPathToPaste);
		for (int i = 0; i < this->mFileOrDirToCopy.size(); i++) {
			QString newFileOrDirPath = this->getUnusedFileOrDirPath(this->mFileOrDirToCopy[i]);
			if (this->mPasteMode == PasteMode::Copy) {
				this->mFileOrDirPastedPaths.push_back(newFileOrDirPath);
				QDir srcDir(this->mFileOrDirToCopy[i]);
				if (srcDir.exists()) {
					QFileInfo info(this->mFileOrDirToCopy[i]);
					if (info.isRoot()) {
						continue;
					}
					else if (info.isFile()) {
						QFile file;
						if (!file.copy(info.absoluteFilePath(), newFileOrDirPath)) {
							QString errorStr = file.errorString();
						}
					}
					else if (info.isDir()) {
						this->doDirectoryCopy(srcDir, targetDir, newFileOrDirPath);
					}
				}
			}
			else {
				QDir srcDir(mFileOrDirToCopy[i]);
				if (srcDir.exists()) {
					QFileInfo info(mFileOrDirToCopy[i]);
					if (info.isRoot()) {
						continue;
					}
					else if (info.isFile()) {
						QFile file(mFileOrDirToCopy[i]);
						file.rename(info.absoluteFilePath(), newFileOrDirPath);
					}
					else if (info.isDir()) {
						QDir dir= info.dir();
						dir.rename(info.absoluteFilePath(), newFileOrDirPath);
					}
				}
			}
		}
	}

	void cancel() override {
		if (this->mPasteMode == PasteMode::Copy) {
			for (int i = 0; i < this->mFileOrDirPastedPaths.size(); i++) {
				QDir dir(this->mFileOrDirPastedPaths[i]);
				if (dir.exists()) {
					dir.remove(this->mFileOrDirPastedPaths[i]);
				}
			}
		}
		else {
			for (int i = 0; i < this->mFileOrDirPastedPaths.size(); i++) {
				QDir dir(this->mFileOrDirPastedPaths[i]);
				if (dir.exists()) {
					dir.rename(this->mFileOrDirPastedPaths[i], this->mFileOrDirToCopy[i]);
				}
			}
		}
	}

	QString toString() override {
		return QString::asprintf("Paste %1 file(s)", this->mFileOrDirToCopy.size());
	}

public:
	PasteCommand(const QString& targetPath,
		         std::vector<QString>& srcPaths,
		         PasteMode pasteMode = PasteMode::Copy) {
		this->mTargetPathToPaste = targetPath;
		this->mPasteMode = pasteMode;
		this->mFileOrDirToCopy.swap(srcPaths);
	}

	LE_NONCOPYABLE(PasteCommand);
};

class RenameCommand final:public AbstractCommand {
private:
	QString              mTargetName;
	std::vector<QString> mTargetPathsToRename;

	void execute() override {

	}

	void cancel() override {
	}

	QString toString() override {
		return QString::asprintf("Rename %1 file(s)", mTargetPathsToRename.size());
	}
public:
	RenameCommand() {

	}
};

class DeleteCommand final :public AbstractCommand {
private:
	std::vector<QString> mTargetPathsToDelete;
	DeleteMode           mDeleteMode;
	FileRecycleBin*      mpRecycleBin;
	int                  mRestoreCode;
	void execute() override {
		mRestoreCode = mpRecycleBin->moveFiles(mTargetPathsToDelete);
	}

	void cancel() override {
		assert(mRestoreCode != -1);
		mpRecycleBin->restoreFiles(mRestoreCode);
	}

	QString toString() override {
		return QString::asprintf("Delete %1 file(s)", mTargetPathsToDelete.size());
	}
public:
	DeleteCommand(FileRecycleBin * pRecycleBin,
		          const std::vector<QString> &filePaths,
		          DeleteMode deleteMode):
		          mpRecycleBin(pRecycleBin), 
		          mTargetPathsToDelete(filePaths),
		          mDeleteMode(deleteMode),
	              mRestoreCode(-1){
		assert(mpRecycleBin != nullptr);
	}
};

class MkFileCommand final :public AbstractCommand {
private:
	QString              mTargetName;
	QString				 mTargetPath;

	void execute() override {
		QString fullPath = this->mTargetPath + "/" +this->mTargetName;
		QFile file(fullPath);
		if (!file.exists(fullPath)) {
			file.open(QIODevice::ReadWrite);
		}
		else { /* file exists */ } 
		file.close();
	} 

	void cancel() override {
		QString fullPath = this->mTargetPath + "/" +this->mTargetName;
		if (QFile::exists(fullPath)) {
			QFile::remove(fullPath);
		}
		else { /* file had been deleted */ } 
	}

	QString toString() override {
		return QString::asprintf("MkFile '%1'", mTargetName);
	}
public:
	MkFileCommand(const QString &targetDirPath,const QString fileName):
		mTargetName(fileName),	
		mTargetPath(targetDirPath) {}
};

class MkDirCommand final :public AbstractCommand {
private:
	QString              mTargetName;
	std::vector<QString> mTargetPathsToRename;

	void execute() override {

	}

	void cancel() override {
	}

	QString toString() override {
		return QString::asprintf("Rename %1 file(s)", mTargetPathsToRename.size());
	}
public:
	MkDirCommand() {

	}
};

#endif //!LE_FILE_EXPLORER_COMMANDS_H
