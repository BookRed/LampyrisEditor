#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_abstract_logger.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_FILE_LOGGER
#define LE_FILE_LOGGER

#include <QDate>
#include <QTime>
#include <memory>
#include "le_abstract_logger.h"

class FileLogger:public AbstractLogger {
	using FilePtr = std::unique_ptr<QFile>;
private:
	FilePtr mTargetFile;

	QString buildTimeStamp() const {
		QDateTime dateTime = QDateTime::currentDateTime();
		return dateTime.toString("yyyy-MM-dd hh:mm:ss.zzz");
	}

 	QString buildMessagePrefix(const LE_LOG_MESSAGE_LEVEL level) const {
		QString timeStamp = buildTimeStamp();
		if (level == LE_LOG_MESSAGE_LEVEL_INFO) {
			return QString::asprintf("[INFO %1]").arg(timeStamp);
		}
		else if (level == LE_LOG_MESSAGE_LEVEL_WARNING) {
			return QString::asprintf("[WARNING %1]").arg(timeStamp);
		}
		else if (level == LE_LOG_MESSAGE_LEVEL_ERROR) {
			return QString::asprintf("[ERROR %1]").arg(timeStamp);
		}
		else {
			return QString();
		}
	}
public:
	virtual void printInfo(const char* message) {
		if (!this->mTargetFile->isOpen()) return;
		this->mTargetFile->write((buildMessagePrefix(LE_LOG_MESSAGE_LEVEL_INFO) +
							      message +"\n").toUtf8());
	}

	virtual void printWarning(const char* message) {
		if (!this->mTargetFile->isOpen()) return;
		this->mTargetFile->write((buildMessagePrefix(LE_LOG_MESSAGE_LEVEL_WARNING) +
							      message +"\n").toUtf8());
	}

	virtual void printError(const char* message) {
		if (!this->mTargetFile->isOpen()) return;
		this->mTargetFile->write((buildMessagePrefix(LE_LOG_MESSAGE_LEVEL_ERROR) +
							      message +"\n").toUtf8());
	}

	FileLogger(const QString& filePath = QString()) {
		this->mTargetFile = std::make_unique<QFile>(filePath);
	}

	~FileLogger() {
		if (this->mTargetFile != nullptr && this->mTargetFile->isOpen()) {
			this->mTargetFile->flush();
			this->mTargetFile->close();
		}
	}
};
#endif //!LE_FILE_LOGGER
