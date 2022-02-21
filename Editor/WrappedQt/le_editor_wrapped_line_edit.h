#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_wrapped_line_edit.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_WRAPPED_LINE_EDIT_H
#define LE_EDITOR_WRAPPED_LINE_EDIT_H
// LE Includes
#include "le_editor_line_edit.h"
#include "le_editor_property_field.h"

class LEEditorWrappedLineEdit :public LEEditorPropertyField {
	Q_OBJECT
private:
	LEEditorLineEdit* m_lineEdit;
public:
	LEEditorWrappedLineEdit(const QString& title,
		                    const int      type = 0);

	void                    setTitle(const QString& title);

	void                    setText(const QString& text);

	void                    setAlignment(Qt::Alignment align) {
		this->m_lineEdit->setAlignment(align);
	}

	void                    setPasswordMode(bool value) {
		if (!value) {
			this->m_lineEdit->setEchoMode(QLineEdit::Normal);
		}
		else {
			this->m_lineEdit->setEchoMode(QLineEdit::Password);
		}
	}
	QString                 text();
};
#endif // !LE_EDITOR_WRAPPED_LINE_EDIT_H