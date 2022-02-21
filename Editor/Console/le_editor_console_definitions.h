#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_console_definitions.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_CONSOLE_DEFINITIONS_H
#define LE_EDITOR_CONSOLE_DEFINITIONS_H
// QT Includes
#include <QString>

namespace Lampyris::Detail {
	enum ConsoleMessageType {
		Info = 0,
		Warning = 1,
		Error = 2
	};

	//ConsoleMessage of Console
	typedef struct ConsoleMessage {
		//called Info can be presented as : function_something(...) in Line xx
		QString            calledInfo;
		//main content of message
		QString            message;
		//type of message (Info,Warning,Error)
		ConsoleMessageType type;

		ConsoleMessage() {}

		ConsoleMessage(QString calledInfo,
		               QString message,
		               ConsoleMessageType type) :
			calledInfo(calledInfo),
			message(message),
			type(type) {}

		bool serach(const QString& content) {
			int calledInfoPos = this->calledInfo.indexOf(content);
			int messagePos = this->message.indexOf(content);

			bool calledInfoMatched = false;
			bool messageMatched = false;

			int contentSize = content.size();
			if (calledInfoPos != -1) {
				calledInfoMatched = true;
			}
			if (messagePos != -1) {
				messageMatched = true;
			}
			return calledInfoMatched || messageMatched;
		}

	} ConsoleMessage;
}

#endif
