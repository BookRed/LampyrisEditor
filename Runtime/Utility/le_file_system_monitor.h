#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Utility
 * \File:    le_file_system_monitor.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_FILE_SYSTEM_MONITOR
#define LE_FILE_SYSTEM_MONITOR

// LE Includes
#include "../Base/le_assert.h"
#include "../Base/le_platform.h"
#include "../Base/le_singleton.h"
#include "le_path_utility.h"

// STL Includes
#include <iostream>
#include <thread>
#include <unordered_map>

#ifdef LE_PLATFORM_WINDOWS

struct LEPathNode{
	bool                     m_isFile;
	std::string              m_path;
	std::vector<LEPathNode*> m_children;
	LEPathNode*              m_parent;
	LEPathNode() :m_isFile(false), m_parent(nullptr) {}
	LEPathNode(const std::string& path, bool isFile,LEPathNode* parent = nullptr) :
		m_path(path), m_isFile(isFile), m_parent(nullptr) {}
};

class LEFileSystemMonitor:public Singleton<LEFileSystemMonitor> {
private:
	constexpr static int                         ms_bufferSize  = 4096;
	constexpr static int                         ms_sleepTimems = 500;
	LEPathNode*                                  m_root;
	bool                                         m_running;
	std::thread                                  m_monitorExecThread;
	std::unordered_map<std::string, LEPathNode*> m_pathMap;
public:			         
	LEFileSystemMonitor():
		m_root(new LEPathNode),m_running(false) {}

	~LEFileSystemMonitor() {}

	void scanAllFile(LEPathNode* root) {
		std::filesystem::path pathObject(root->m_path);
		if (!std::filesystem::exists(pathObject) ||
			!std::filesystem::is_directory(pathObject)) {
			return ;
		}
		std::filesystem::directory_entry direntry(pathObject);
		if (direntry.status().type() == std::filesystem::file_type::directory) {
			//TODO something....
		}
		std::filesystem::directory_iterator dirIter(pathObject);
		for (auto& iter : dirIter) {
			LEPathNode* node = nullptr;
			bool isDir = false;
			// folder
			if (iter.status().type() == std::filesystem::file_type::directory) {
				node = new LEPathNode(iter.path().string(), false, root);
			}
			// file
			else if (iter.status().type() == std::filesystem::file_type::regular) {
				node = new LEPathNode(iter.path().string(), true, root);
			}
			node = new LEPathNode(iter.path().string(), false, root);
			root->m_children.push_back(node);
			this->m_pathMap[node->m_path] = node;
			scanAllFile(node);
		}
	}

	void traverse(LEPathNode* root, std::vector<std::string>& paths) {
		if (root) {
			for (int i = 0; i < root->m_children.size(); i++) {
				if (root->m_children[i]->m_isFile) {
					paths.push_back(root->m_children[i]->m_path);
				}
				else {
					traverse(root->m_children[i], paths);
				}
			}
		}
	}
	void run() {
		this->scanAllFile(this->m_root);
		std::string monitorPath  = this->m_root->m_path;
		const std::wstring path  = le_char_to_wchar(monitorPath);
		const int fileSharedFlag = FILE_SHARE_READ  | 
			                       FILE_SHARE_WRITE | 
			                       FILE_SHARE_DELETE;
		const int fileFlag = FILE_FLAG_BACKUP_SEMANTICS | 
			                 FILE_FLAG_OVERLAPPED;
		const int fileNotifyFlag = FILE_NOTIFY_CHANGE_FILE_NAME | 
			                       FILE_NOTIFY_CHANGE_DIR_NAME | 
			                       FILE_NOTIFY_CHANGE_LAST_WRITE;
		HANDLE hDirectory = CreateFile(path.c_str(),
			                           FILE_LIST_DIRECTORY,
			                           fileSharedFlag,
			                           NULL,
			                           OPEN_EXISTING,
			                           fileFlag,
			                           NULL);
		LE_ASSERT(hDirectory != INVALID_HANDLE_VALUE);
		BYTE   buffer[ms_bufferSize];
		DWORD  dwBufferSize;
		WCHAR  szFileName[MAX_PATH];
		char*  szFilePath;
		PFILE_NOTIFY_INFORMATION pNotify = (PFILE_NOTIFY_INFORMATION)(&buffer);

		while (true) {
			if (ReadDirectoryChangesW(hDirectory,
				                      &buffer,
				                      ms_bufferSize,
				                      TRUE,
				                      fileNotifyFlag,
				                      &dwBufferSize,
				                      NULL,
				                      NULL)) {
				pNotify->FileName[pNotify->FileNameLength/2] = L'\0';
				std::string notifiedPathStr = le_wchar_to_char(path + pNotify->FileName);
				bool isDir = LEPathUtility::isDirectory(notifiedPathStr);
				switch (pNotify->Action) {
				case FILE_ACTION_ADDED: {
					if (isDir) {
						// we don't have to care about new directory.
						std::cout << "[FILE] folder added: " << notifiedPathStr << std::endl;
					}
					else {
						// if added file is supported resource, notify resource manager to import it.
						std::cout << "[FILE] file added: " << notifiedPathStr << std::endl;
					}
					break;
				}
				case FILE_ACTION_REMOVED: {
					isDir = !this->m_pathMap[notifiedPathStr]->m_isFile;
					if (isDir) {
						std::cout << "[FILE] folder removed: " << notifiedPathStr << std::endl;
						std::vector<std::string> paths;
						this->traverse(this->m_pathMap[notifiedPathStr], paths);
						for (int i = 0; i < paths.size(); i++) {
							std::cout << "[FILE] affected sub-path:" << paths[i] << std::endl;
						}
					}
					else {
						std::cout << "[FILE] file removed: " << notifiedPathStr << std::endl;
					}
					break;
				}
				case FILE_ACTION_MODIFIED: {
					if (!isDir) {
						std::cout << "[FILE] file modified: " << notifiedPathStr << std::endl;
					}
					break;
				}
				case FILE_ACTION_RENAMED_OLD_NAME: {
					if (pNotify && pNotify->NextEntryOffset != 0UL) {
						PFILE_NOTIFY_INFORMATION pNext = (PFILE_NOTIFY_INFORMATION)((LPBYTE)pNotify + pNotify->NextEntryOffset);
						LE_ASSERT((DWORD)((BYTE*)pNext - buffer) < dwBufferSize);
						pNext->FileName[pNext->FileNameLength/2] = L'\0';
						std::string nextNotifiedPathStr = le_wchar_to_char(path + pNext->FileName);
						if (isDir) {
							std::cout << "[FILE] folder renamed from " << notifiedPathStr << " to " << nextNotifiedPathStr << std::endl;
						}
						else {
							std::cout << "[FILE] file renamed from " << notifiedPathStr << " to " << nextNotifiedPathStr << std::endl;
						}
					}
					break;
				}
				case FILE_ACTION_RENAMED_NEW_NAME: {
					printf("FILE_ACTION_RENAMED_NEW_NAME.\n");
					break;
				}
				default:
					break;
				}
			}
		}
	}

	void setMonitorPath(const std::string& path) {
		this->m_root->m_path = path;
	}


};

#endif // !LE_PLATFORM_WINDOWS
#endif // !LE_FILE_SYSTEM_MONITOR