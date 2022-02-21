#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_wrapped_object_field.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_WRAPPED_OBJECT_FIELD_H
#define LE_EDITOR_WRAPPED_OBJECT_FIELD_H
// LE Include
#include "le_editor_property_field.h"
#include <QMessageBox>

class LEEditorWrappedObjectWidget :public LEEditorPropertyField {
	Q_OBJECT
private:
	QString m_objectTypeName = "TestComponent";
	QString m_objectName = "TestComponmentObject";
	// use QFontMetrics to contain the height and width of the text 
	int     objectTypeNameWidth;
	int     objectTypeNameHeight;

	bool    m_removeButtonHovered;
	bool    m_selectButtonHovered;
	bool    m_enter;

	QColor  m_color;
	QRect   m_removeButtonRect;
	QRect   m_selectButtonRect;

protected:
	void enterEvent(QEvent* e);

	void leaveEvent(QEvent* e);

	void mousePressEvent(QMouseEvent* e);

	void mouseMoveEvent(QMouseEvent* e);

	// the layout of this self-drawn control is:

	/*----------------------------------------*/
	/*| objectTypeName                        |/
	/*----------------------------------------*/
	/*| objectName  |removeButton|selectButton|
	/*----------------------------------------*/

	// remove button is visible when selected object is not null
	// click selectButton will open the object select window.
	void paintEvent(QPaintEvent* e);
public:
	LEEditorWrappedObjectWidget(QWidget* parent = nullptr) {
		this->setMouseTracking(true);
	}

Q_SIGNALS:
	void signalremoveButtonClicked();
	void signalselectButtonClicked();
};

class LEEditorWrappedObjectField :public LEEditorPropertyField {
	Q_OBJECT
private:
	LEEditorWrappedObjectWidget* m_widget;
public:
	LEEditorWrappedObjectField(const QString& title);
};
#endif // !LE_EDITOR_WRAPPED_OBJECT_FIELD_H
