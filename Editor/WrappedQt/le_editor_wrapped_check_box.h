#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_wrapped_check_box.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_WRAPPED_CHECK_BOX_H
#define LE_EDITOR_WRAPPED_CHECK_BOX_H
// QT Includes
#include <QCheckBox>
// LE Includes
#include "le_editor_property_field.h"

class LEEditorWrappedCheckBox :public LEEditorPropertyField {
	Q_OBJECT
private:
	QCheckBox* m_checkBox;
public:
	           LEEditorWrappedCheckBox(const QString& title,
		                               const bool     value);
	bool       isChecked();

	void       setChecked(const bool checked);
};
#endif // !LE_EDITOR_WRAPPED_CHECK_BOX_H
