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
#include "le_global.h"
#include "le_assert_failure_report.h"

#ifndef LE_ASSERT 

#ifdef LE_DEBUG
	#define LE_ASSERT(expr) if(!(expr)) le_report_assert_failure(#expr,__FUNCTION__,__FILE__,__LINE__)
	#define LE_ASSERT_RETURN(expr,value) { int ret = expr; if(ret!=value) le_report_assert_failure(#expr,__FUNCTION__,__FILE__,__LINE__,ret); }
	#define LE_ASSERT_MSG(expr,value) if(!(expr)) le_report_assert_failure(#expr,__FUNCTION__,__FILE__,__LINE__)
#else
	#define LE_ASSERT(expr) if(!(expr)) {}
	#define LE_ASSERT_RETURN(expr,value) {expr;}
	#define LE_ASSERT_MSG(expr) if(!(expr)) {}
#endif // !LE_DEBUG 

#define LE_STATIC_ASSERT(expr,messgae) \
	static_assert(expr,message);
#endif // !LE_ASSERT
#endif // !LE_ASSERT_H