/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_wrapped_color_field.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_wrapped_color_field.h"

void LEEditorWrappedColorField::dispatchEvent() {
	emit signalEventDispatch(this);
}

LEEditorWrappedColorField::LEEditorWrappedColorField(const QString& name) :
	m_button(new LEEditorColoredButton) {
	this->ensure(name, this->m_button);
	QObject::connect(this->m_button, &LEEditorColoredButton::dispatchEvent, this,&LEEditorWrappedColorField::dispatchEvent);
}