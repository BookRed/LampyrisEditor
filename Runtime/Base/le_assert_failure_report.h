/*!
 * \Lampyris GameEngine C++ Header
 * \Module:  Base
 * \File:    le_assert_failure_report.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_ASSERT_FAILURE_REPORT_H
#define LE_ASSERT_FAILURE_REPORT_H

// STL Includes
#include <string>

// LE-base Includes
#include "le_global.h"
#include "../Utility/le_string_convert.h"

void le_report_assert_failure(const char* expr,
							  const char* func,
	                          const char* file,
	                          const int   lineno,
							  const int   returnCode = 0);
#endif // !LE_ASSERT_FAILURE_REPORT_H