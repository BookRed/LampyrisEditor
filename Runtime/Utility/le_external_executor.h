#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Utility
 * \File:    le_external_executor.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_EXTERNAL_EXECUTOR_H
#define LE_EXTERNAL_EXECUTOR_H

// LE Includes
#include "../Base/le_global.h"
#include "le_string_convert.h"

// References:
// [0]:https://blog.csdn.net/weixin_32558527/article/details/112705968

#if defined(LE_PLATFORM_WINDOWS)
class LEWindowsExternalExecutor {
private:
	std::string m_resultBuffer;
	std::string m_commandLine;
	int         m_exitCode;
public:
	LEWindowsExternalExecutor();

	LEWindowsExternalExecutor(const std::string& cmd);

	~LEWindowsExternalExecutor();

	bool execute();

	const std::string& getResult() const;

	int getExitCode() const;

	void setCommandLine(const std::string& cmd);
};
typedef LEWindowsExternalExecutor LEExternalExecutor;

#endif // !LE_PLATFORM_WINDOWS
#endif // !LE_EXTERNAL_EXECUTOR_H
