#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_wrapped_button.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_WRAPPED_BUTTON_H
#define LE_EDITOR_WRAPPED_BUTTON_H
// QT Includes
#include <QPushButton>

class LEEditorWrappedButton :public QPushButton {
	Q_OBJECT
private:
	bool m_clicked = false;
	bool m_unused[3];
public:
	         LEEditorWrappedButton(const QString& text,
	                               QWidget*       parent = nullptr);

	bool     isClickedLastTime();
Q_SIGNALS:
	void     signalEventDispatch(QWidget* object);
};
#endif // !LE_EDITOR_WRAPPED_BUTTON_H

