/*!
 * \Lampyris GameEngine C++ Header
 * \Module:  Debug
 * \File:    le_crash_reporter_exec.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_CRASH_REPORTER_EXEC_H
#define LE_CRASH_REPORTER_EXEC_H

#include "../Common/le_global.h"

LE_NAMESPACE_BEGIN(Debug)

static void le_exec_crash_reporter() {
#if defined(LE_PLATFORM_WINDOWS)
	// make execute cmd
	std::string cmd = "LampyrisCrashReporter.exe " +
		reason + " " +
		func + " " +
		file + " " +
		std::to_string(lineno);

	::WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
#elif defined(LE_PLATFORM_LINUX)
	::execv(argv[1], args_);
#endif
}

LE_NAMESPACE_END

#endif // !LE_CRASH_REPORTER_EXEC_H

