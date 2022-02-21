/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_line_edit.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_line_edit.h"
// QT Includes
#include <QKeyEvent>

void LEEditorLineEdit::keyPressEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_Return) {
		this->clearFocus();
		emit editingFinished();
	}
	else {
		QLineEdit::keyPressEvent(event);
	}
}

void LEEditorLineEdit::focusInEvent(QFocusEvent* event) {
	QLineEdit::focusInEvent(event);
	return;
	this->setReadOnly(false);
	QLineEdit::focusInEvent(event);
}

void LEEditorLineEdit::focusOutEvent(QFocusEvent* event) {
	QLineEdit::focusOutEvent(event);
	return;
	this->setCursorPosition(this->text().length());
	this->setReadOnly(true);
	emit editingFinished();
}

LEEditorLineEdit::LEEditorLineEdit(const QString& str,
                                   QWidget* parent) :QLineEdit(str, parent) {}

