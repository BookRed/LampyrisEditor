#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_console_message_item_widget.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_CONSOLE_MESSAGE_ITEM_WIDGET_H
#define LE_EDITOR_CONSOLE_MESSAGE_ITEM_WIDGET_H
// QT Includes
#include <QLabel>
#include <QListWidget>
#include <QWidget>
// LE Includes
#include "le_editor_console_definitions.h"

namespace Lampyris::Detail {
class LEEditorConsoleMessageItemWidget : public QWidget {
private:
	ConsoleMessage m_msg;
	QLabel*        m_iconLabel;
	QLabel*        m_calledInfoLabel;
	QLabel*        m_messasgeLabel;

	// Load the icon according to the message type 
	QPixmap&       getIconByMessageType(ConsoleMessageType type);

	// get the message text information prefix according to the message type 
	QString        getMessagePrefixString(ConsoleMessageType type);
public:
	               LEEditorConsoleMessageItemWidget(QListWidget* parent, ConsoleMessage message);

	               ConsoleMessageType getType();

	void           setMessage(ConsoleMessage message);
};
}

#endif // !LE_EDITOR_CONSOLE_MESSAGE_ITEM_WIDGET_H
