/*!
 * \Lampyris GameEngine C++ Header
 * \Module:  Debug
 * \File:    le_assert_failure_report.h
 * \Author:  Timor
 * \Email:   390031942@qq,com
*/

#ifndef LE_ASSERT_FAILURE_REPORT_H
#define LE_ASSERT_FAILURE_REPORT_H

// STL Includes
#include <string>

// LE-base Includes
#include "../Common/le_global.h"
#include "../Collection/le_string_convert.h"

#include "../Common/le_global.h"

LE_NAMESPACE_BEGIN(Debug);

static void le_report_assert_failure(const char* expr,
								     const char* func,
	                                 const char* file,
	                                 const int   lineno) {
	std::string reason = std::string() + " Assertion\"" + expr + "\"Failed";
	reason.erase(std::remove_if(reason.begin(), reason.end(), isspace), reason.end());
	// We need to display crash reporter on some platforms 
#if defined(LE_PLATFORM_WINDOWS)
	// make shell cmd
	std::string cmd = "LampyrisCrashReporter.exe " + 
		               reason + " " + 
		               func   + " " + 
		               file   + " " + 
		               std::to_string(lineno);

	::WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
#elif defined(LE_PLATFORM_LINUX)
	::execv(argv[1], args_);
#endif
}

LE_NAMESPACE_END

#endif // !LE_ASSERT_FAILURE_REPORT_H