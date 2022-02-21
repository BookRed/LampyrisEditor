#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_application.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_APPLICATION_H
#define LE_APPLICATION_H

// LE Includes
#include "le_global.h"
#include "le_subsystem.h"
#include "le_singleton.h"
#include "../Utility/le_string_convert.h"

class LEApplication:public SubSystem,public Singleton<LEApplication> {
private:
	/* Command Line Parameters*/
	int     m_argc;
	char**  m_argv;
	std::string m_projectPath;
public:
	void doStartUp();

	void doShutDown();

	void doUpdate();

	void setCommandLineParameter(int argc,char** argv);

	std::string getCurrentWorkDirectory() const;

	std::string getApplicationDirectory() const;
#if defined(LE_EDITOR)
	std::string getProjectDirectory() const;
#endif // !LE_EDITOR
};
#endif // !LE_APPLICATION_H