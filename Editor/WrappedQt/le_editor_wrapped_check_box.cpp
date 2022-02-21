/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_wrapped_check_box.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_wrapped_check_box.h"

LEEditorWrappedCheckBox::LEEditorWrappedCheckBox(const QString& title, const bool value) :
	m_checkBox(new QCheckBox(this)) {
	QObject::connect(this->m_checkBox, &QCheckBox::toggled, [=]() {
		emit signalEventDispatch(this);
		});
	this->ensure(title, m_checkBox);
}

bool LEEditorWrappedCheckBox::isChecked() {
	return this->m_checkBox->isChecked();
}

void LEEditorWrappedCheckBox::setChecked(const bool checked) {
	this->m_checkBox->setChecked(checked);
}
