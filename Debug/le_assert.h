/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Debug
 * \File:    le_assert.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_ASSERT_H
#define LE_ASSERT_H

// LE-base Includes
#include "../Common/le_global.h"
#include "le_assert_failure_report.h"

LE_USING_NAMESPACE(Debug);
#ifndef LE_ASSERT 

#ifdef LE_DEBUG
	#define LE_ASSERT(expr) if(!(expr)) le_report_assert_failure(#expr,__FUNCTION__,__FILE__,__LINE__)
	#define LE_ASSERT_WITH_MSG(expr,msg) if(!(expr)) le_report_assert_failure(#expr,__FUNCTION__,__FILE__,__LINE__)
#else
	#define LE_ASSERT(expr)
	#define LE_ASSERT_WITH_MSG(expr,msg)
#endif // !LE_DEBUG 

#endif // !LE_ASSERT
#endif // !LE_ASSERT_H