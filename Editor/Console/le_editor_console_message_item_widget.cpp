/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_console_message_item_widget.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_console_message_item_widget.h"
// QT Includes
#include <QDateTime>
#include <QHBoxLayout>

namespace Lampyris::Detail {
QPixmap& LEEditorConsoleMessageItemWidget::getIconByMessageType(ConsoleMessageType type) {
	static QPixmap consoleIconInfo("res/console_info.png");
	static QPixmap consoleIconWarn("res/console_warning.png");
	static QPixmap consoleIconError("res/console_error.png");
	if (type == ConsoleMessageType::Info) {
		return consoleIconInfo;
	}
	if (type == ConsoleMessageType::Warning) {
		return consoleIconWarn;
	}
	return consoleIconError;
}

QString LEEditorConsoleMessageItemWidget::getMessagePrefixString(ConsoleMessageType type) {
	QString result;
	if (type == ConsoleMessageType::Info) {
		result = "[Info] ";
	}
	else if (type == ConsoleMessageType::Warning) {
		result = "[Warning] ";
	}
	else {
		result = "[Error] ";
	}
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss");
	return result;
}

ConsoleMessageType LEEditorConsoleMessageItemWidget::getType() {
	return m_msg.type;
}

void LEEditorConsoleMessageItemWidget::setMessage(ConsoleMessage message) {
	this->m_msg = message;
	m_iconLabel->setPixmap(getIconByMessageType(message.type).scaled(QSize(30,30)));
	m_calledInfoLabel->setText(getMessagePrefixString(message.type) + " " + message.calledInfo);
	m_messasgeLabel->setText(message.message);
}

LEEditorConsoleMessageItemWidget::LEEditorConsoleMessageItemWidget(QListWidget* parent, ConsoleMessage message) {
	// The layout of LEEditorConsoleMessageItemWidget consists of two parts:
	// Part1: The icon for the message is displayed on the left,
	// and the text is displayed on the right
	// Part2 :The upper right is used to display the call information,
	// and the lower right is used to display the main content of the message
	this->m_msg = message;
	QLayout* layout = new QHBoxLayout;
	this->setLayout(layout);
	layout->setContentsMargins(10, 0, 10, 0);
	layout->setSpacing(1);
	// Part 1
	this->m_iconLabel = new QLabel;
	this->m_iconLabel->setFixedWidth(30);
	this->m_iconLabel->setPixmap(getIconByMessageType(message.type).scaled(QSize(30, 30)));
	layout->addWidget(this->m_iconLabel);

	// Part 2
	QWidget* widgetRight = new QWidget;
	layout = new QVBoxLayout;
	widgetRight->setLayout(layout);
	widgetRight->layout()->setSpacing(1);

	// build message text
	this->m_msg.calledInfo = (this->getMessagePrefixString(this->m_msg.type) + " " + this->m_msg.calledInfo);
	this->m_calledInfoLabel = new QLabel(this->m_msg.calledInfo);
	this->m_messasgeLabel = new QLabel(m_msg.message);

	// ensure controls 
	layout->addWidget(this->m_calledInfoLabel);
	layout->addWidget(this->m_messasgeLabel);
	this->layout()->addWidget(widgetRight);
}
}
