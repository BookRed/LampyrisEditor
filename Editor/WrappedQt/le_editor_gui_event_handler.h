#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_gui_event_handler.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_GUI_EVENT_HANDLER_H
#define LE_EDITOR_GUI_EVENT_HANDLER_H
// LE Includes
#include <QWidget>

class LEEditorGUIEventHandler :public QObject {
	Q_OBJECT
private:
	QWidget* m_sender;
public slots:
	void     slotHandle(QWidget* object);

	QWidget* getLastSender() const;
};
#endif // !LE_EDITOR_GUI_EVENT_HANDLER_H