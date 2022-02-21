/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Utility
 * \File:    le_file.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_file.h"

LEFile::LEFile(const std::string& path):
	m_fstream(path.c_str()),
    m_path(path.c_str()) {}

void LEFile::writeLine(const char* content) {
	this->write(content);
	this->write("\n");
}

void LEFile::write(const char* content) {
	if (!this->isOpened()) {
		return;
	}
	this->m_fstream.write(content, std::strlen(content));
}

std::string LEFile::read() {
	// a buffer to store data read from file.
	std::string content;
	content.resize(this->getFileSize() + 1);
	this->m_fstream.read(content.data(), this->getFileSize() + 1);
	return content;
}

bool LEFile::isExists(const std::string& path) {
	return std::filesystem::exists(path);
}

bool LEFile::create(const std::string& path) {
	return std::filesystem::create_directory(path);
}

bool LEFile::createFile(const std::string& path) {
	std::ofstream ouf(path);
	if (ouf) { 
		return true;
	}
	return false;
}
bool LEFile::isOpened() const {
	return this->m_fstream.is_open();
}

void LEFile::close() {
	this->m_fstream.close();
}

void LEFile::flush() {
	this->m_fstream.flush();
}

std::string LEFile::getFileName() const {
	return m_path.filename().string();
}

std::string LEFile::getFileDirectoryPath() const {
	return LEPathUtility::getFileDirectoryPath(this->m_path);
}

std::string LEFile::getFilePrefix() const {
	std::string fileName = this->getFileName();
	int index = fileName.find_first_of(".");
	return fileName.substr(0, index);
}

std::string LEFile::getFileSuffix() const {
	std::string fileName = this->getFileName();
	int index = fileName.find_first_of(".");
	return fileName.substr(index + 1, fileName.size() - index - 1);
}

unsigned int LEFile::getFileSize() const {
	return std::filesystem::file_size(this->m_path);
}

LEFile LEFile::copy() {
	Path newPath(LEPathUtility::getUnusedPath(this->m_path));
	std::filesystem::copy(this->m_path, newPath);
	LEFile file(newPath.string());
	return file;
}

void LEFile::remove() {
	std::filesystem::remove(this->m_path);
}

void LEFile::rename(const std::string& newFileName) {
	std::string folderPath = LEPathUtility::getFileDirectoryPath(newFileName.c_str());
	std::string oldPath = folderPath +
		"\\" +
		this->m_path.string();
	std::string newPath = folderPath +
		"\\" +
		newFileName;
	this->m_fstream.close();
	std::filesystem::rename(oldPath, newPath);
	this->m_fstream.open(newPath);
	this->m_path = newPath;
}
