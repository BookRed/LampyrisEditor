/*!
 * \Lampyris GameEngine C++ Header
 * \Module:  Base
 * \File:    le_assert_failure_report.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_assert_failure_report.h"

void le_report_assert_failure(const char* expr, const char* func, const char* file, const int lineno, const int returnCode) {
	std::string reason = std::string() + " \"Assertion Failed: " + expr;
	// We need to display crash reporter on some platforms 
#if defined(LE_PLATFORM_WINDOWS)
	// make shell cmd
	std::string cmd = "LampyrisCrashReporter.exe " +
		reason + "\"  \"" +
		file + "\" " +
		std::to_string(lineno);
	::WinExec(cmd.c_str(), SW_SHOW);
	::exit(returnCode);
#elif defined(LE_PLATFORM_LINUX)
	::execv(argv[1], args_);
#endif
}
