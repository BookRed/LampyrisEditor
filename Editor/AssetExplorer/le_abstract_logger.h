#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_abstract_logger.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_ABSTRACT_LOGGER
#define LE_ABSTRACT_LOGGER

#define LE_LOG_MESSAGE_LEVEL_INFO    0x001
#define LE_LOG_MESSAGE_LEVEL_WARNING 0x002
#define LE_LOG_MESSAGE_LEVEL_ERROR   0x003

#define LE_LOG_MESSAGE_LEVEL         int

class AbstractLogger {
public:
	virtual void printInfo() = 0;
	virtual void printWarning() = 0;
	virtual void printError() = 0;
};
#endif //!LE_ABSTRACT_LOGGER