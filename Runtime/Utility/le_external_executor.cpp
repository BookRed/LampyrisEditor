/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Utility
 * \File:    le_external_executor.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_external_executor.h"

LEWindowsExternalExecutor::LEWindowsExternalExecutor():
	m_exitCode(-1) {}

LEWindowsExternalExecutor::LEWindowsExternalExecutor(const std::string& cmd):
	m_commandLine(cmd) {}

LEWindowsExternalExecutor::~LEWindowsExternalExecutor() {}

bool LEWindowsExternalExecutor::execute() {
	SECURITY_ATTRIBUTES sa;
	sa.nLength              = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle       = TRUE;

	HANDLE hRead, hWrite;
	if (!::CreatePipe(&hRead, &hWrite, &sa, 0)) {
		return false;
	}

	STARTUPINFO si         = { sizeof(si) };
	PROCESS_INFORMATION pi = {};
	si.hStdError           = hWrite;
	si.hStdOutput          = hWrite;
	si.wShowWindow         = SW_HIDE;
	si.dwFlags             = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	std::wstring cmdline = le_char_to_wchar(this->m_commandLine);
	if (!::CreateProcess(NULL, cmdline.data(),
	                     NULL, NULL, TRUE, 
	                     NULL, NULL, NULL, 
	                     &si, &pi)) {
		return false;
	}
	::CloseHandle(hWrite);

	char buffer[4096];
	::memset(buffer, '\0', 4096);
	DWORD byteRead    = 0;
	DWORD returnValue = 0;

	// get exit code
	::GetExitCodeProcess(pi.hProcess, &returnValue);
	this->m_exitCode = returnValue;

	this->m_resultBuffer.clear();
	// read all the output data
	while (true)		 {
		if (ReadFile(hRead,buffer,4095, &byteRead, NULL) == NULL) {
			break;
		}
		this->m_resultBuffer.append(buffer, byteRead);
	}    
	// solution for warning 'C6335'
	::CloseHandle(pi.hProcess);
	::CloseHandle(pi.hThread);
	return true;
}

const std::string& LEWindowsExternalExecutor::getResult() const {
	return this->m_resultBuffer;
}

int LEWindowsExternalExecutor::getExitCode() const {
	return this->m_exitCode;
}

void LEWindowsExternalExecutor::setCommandLine(const std::string& cmd) {
	this->m_commandLine = cmd;
}
