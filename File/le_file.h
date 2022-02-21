#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  File
 * \File:    le_file.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_FILE_H
#define LE_FILE_H

// LE Includes
#include "le_path_utility.h"

class LEFile {
	typedef boost::filesystem::wfstream FileStream;
	typedef boost::filesystem::path     Path;
	typedef std::streampos				StreamPos;

private:
	FileStream m_fstream;
	Path       m_path;
public:
	LEFile(const LEString& path) :m_fstream(path.constStr()),
								  m_path(path.constStr()) {}

	LE_FORCEINLINE void writeLine(const LEString& content) {
		this->write(content);
		this->write(L"\n");
	}

	LE_FORCEINLINE void write(const LEString& content) {
		if (!this->isOpened()) {
			return;
		}
		this->m_fstream.write(content.constStr(), content.getLength());
	}

	LEString read() {
		// a buffer to store data read from file.
		LEWChar* buffer = new LEWChar[this->getFiuint32_t() + 1];
		::memset(buffer, 0, sizeof(LEWChar) * (this->getFiuint32_t() + 1));
		this->m_fstream.read(buffer, this->getFiuint32_t());

		LEString wstr(buffer);
		delete[] buffer;
		return wstr;
	}

	static LEBool isExists(const LEString& path) {
		return boost::filesystem::exists(path.constStr());
	}

	static LEBool create(const LEString& path) {
		return boost::filesystem::create_directory(path.constStr());
	}

	LEBool isOpened() const {
		return this->m_fstream.is_open();
	}

	void close() {
		this->m_fstream.close();
	}

	void flush() {
		this->m_fstream.flush();
	}

	LEString getFileName() const {
		return m_path.filename().c_str();
	}


	LEString getFilePrefix() const {
		LEString fileName = this->getFileName();
		int index = fileName.findFirst(L".");
		return fileName.substr(0, index);
	}

	LEString getFileSuffix() const {
		LEString fileName = this->getFileName();
		int index = fileName.findFirst(L".");
		return fileName.substr(index + 1, fileName.getLength() - index - 1);
	}

	uint32_t getFiuint32_t() const {
		return boost::filesystem::file_size(this->m_path);
	}

	LEFile* copy() {
		Path newPath(le_get_unused_path(this->m_path));
		boost::filesystem::copy(this->m_path, newPath);
		LEFile* file = new LEFile(LEString::fromStdWString(newPath.wstring()));
		return file;
	}

	void remove() {

	}

	void rename(const LEString& newFileName) {
		std::wstring folderPath = le_get_file_dir_Path(newFileName.constStr());
		std::wstring oldPath = folderPath + 
			                   L"\\" +
			                   this->m_path.wstring();
		std::wstring newPath = folderPath + 
			                   L"\\" +
			                   newFileName.constStr();
		this->m_fstream.close();
		boost::filesystem::rename(oldPath, newPath);
		this->m_fstream.open(newPath);
		this->m_path = newPath;
	}

};

#endif // !LE_FILE_H