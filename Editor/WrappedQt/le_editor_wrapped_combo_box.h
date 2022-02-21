#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_wrapped_combo_box.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_WRAPPED_COMBO_BOX_H
#define LE_EDITOR_WRAPPED_COMBO_BOX_H
// QT Includes
#include <QComboBox>
// LE Includes
#include "le_editor_property_field.h"

class LEEditorWrappedComboBox :public LEEditorPropertyField {
	Q_OBJECT
private:
	QComboBox* m_comboBox;
public:
	           LEEditorWrappedComboBox(const QString&                        title,
	                                   const std::initializer_list<QString>& items);
	int        currentIndex();

	void       setCurrentIndex(const int index);

	QString    itemText(const int index);

	void       clear();
};
#endif // !LE_EDITOR_WRAPPED_COMBO_BOX_H