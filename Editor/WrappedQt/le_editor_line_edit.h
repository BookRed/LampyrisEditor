#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_line_edit.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_WRAPPED_LINE_EDITOR_H
#define LE_EDITOR_WRAPPED_LINE_EDITOR_H
// QT Includes
#include <QLineEdit>

// allow user to finish editing after 'enter' is pressed.
class LEEditorLineEdit :public QLineEdit {
	Q_OBJECT
protected:
	virtual void keyPressEvent(QKeyEvent* event);

	void         focusInEvent(QFocusEvent* event);

	void         focusOutEvent(QFocusEvent* event);
public: 
	explicit     LEEditorLineEdit(const QString& str,
	                              QWidget*       parent = nullptr);
Q_SIGNALS:
	void         signalFocusOut();
};
#endif // !LE_EDITOR_WRAPPED_LINE_EDITOR_H