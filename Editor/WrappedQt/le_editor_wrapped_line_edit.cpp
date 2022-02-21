/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_wrapped_line_edit.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_wrapped_line_edit.h"
// QT Includes
#include <qvalidator.h>

LEEditorWrappedLineEdit::LEEditorWrappedLineEdit(const QString& title, int type) :
	m_lineEdit(new LEEditorLineEdit(title, this)) {
	if (type == 0) {
		this->m_lineEdit->setValidator(new QIntValidator);
	}
	else if (type == 1) {
		this->m_lineEdit->setValidator(new QDoubleValidator);
	}
	QObject::connect(this->m_lineEdit, &LEEditorLineEdit::editingFinished, [=]() {
		emit signalEventDispatch(this);
		});
	this->ensure(title, m_lineEdit);
}

void LEEditorWrappedLineEdit::setTitle(const QString& title) {

}

void LEEditorWrappedLineEdit::setText(const QString& text) {
	this->m_lineEdit->setText(text);
}

QString LEEditorWrappedLineEdit::text() {
	return this->m_lineEdit->text();
}