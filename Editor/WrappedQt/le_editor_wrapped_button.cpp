/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_wrapped_button.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_wrapped_button.h"

LEEditorWrappedButton::LEEditorWrappedButton(const QString& text, QWidget* parent) :
	QPushButton(text, parent) {
	QObject::connect(this, &QPushButton::clicked, [=]() {
		this->m_clicked = true;
		emit signalEventDispatch(this);
	});
}

bool LEEditorWrappedButton::isClickedLastTime() {
	bool value = this->m_clicked;
	this->m_clicked = false;
	return value;
}
