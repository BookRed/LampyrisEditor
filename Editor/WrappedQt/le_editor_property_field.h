#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_property_field.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_PROPERTY_FIELD_H
#define LE_EDITOR_PROPERTY_FIELD_H
// QT Includes
#include <QLabel>

// Used for lineEdit,checkBox,comboBox,etc
class LEEditorPropertyField :public QWidget {
	Q_OBJECT
private:
	QLabel*  m_titleLabel;
	QString  m_title;
public:
	         LEEditorPropertyField();

	virtual ~LEEditorPropertyField();

	void     ensure(const QString& title,
		            QWidget*       control,
		            QWidget*       parent = nullptr);

	void     setTitle(const QString& title);

Q_SIGNALS:
	void     signalEventDispatch(QWidget* object);
};
#endif // !LE_EDITOR_PROPERTY_FIELD_H