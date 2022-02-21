/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Base
 * \File:    le_application.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_application.h"
#include "le_reflection_system.h"

void LEApplication::doStartUp() {
	LEReflectionSystem::getInstance()->doStartUp();
}

void LEApplication::doShutDown() {
	LEReflectionSystem::getInstance()->doShutDown();
}

void LEApplication::doUpdate() {

}

void LEApplication::setCommandLineParameter(int argc, char** argv) {
	this->m_argc = argc;
	this->m_argv = argv;
}

std::string LEApplication::getCurrentWorkDirectory() const {
	static std::string dir;
	if (dir.empty()) {
#if defined(LE_PLATFORM_WINDOWS)
		wchar_t path[MAX_PATH]; // Full path   
		::GetCurrentDirectory(MAX_PATH, path);
		dir = le_wchar_to_char(std::wstring(path));
#endif
	}
	return dir;
}

std::string LEApplication::getApplicationDirectory() const {
	static std::string dir;
	if (dir.empty()) {
		dir = this->m_argv[0];
	}
	return dir;
}

std::string LEApplication::getProjectDirectory() const {
	return this->m_projectPath;
}
