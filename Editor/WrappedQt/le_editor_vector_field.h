#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_vector_field.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_VECTOR_FIELD_H
#define LE_EDITOR_VECTOR_FIELD_H

// LE Includes
#include "le_editor_property_field.h"
#include "le_editor_wrapped_line_edit.h"
// QT Includes
#include <QHBoxLayout>
#include <qvalidator.h>
// GLM Includes
#include <glm/glm.hpp>

class LEEditorVector2DField :public LEEditorPropertyField {
	Q_OBJECT
private:
	QLabel*                  m_labelX;
	QLabel*                  m_labelY;
	LEEditorLineEdit*        m_editX;
	LEEditorLineEdit*        m_editY;
	QWidget*                 m_container;
	QHBoxLayout*             m_layout;

	void ensureLayout();

	void setControlProperty();

	void connectSignal();
public:
	LEEditorVector2DField();

	virtual ~LEEditorVector2DField();

	void setValue(float x, float y);

	inline float getX();
	inline float getY();
	glm::vec2 get();

	LEEditorVector2DField(const QString& title, float x, float y);
};

class LEEditorVector3DField :public LEEditorPropertyField {
	Q_OBJECT
private:
	QLabel*                  m_labelX;
	QLabel*                  m_labelY;
	QLabel*                  m_labelZ;
	LEEditorLineEdit*        m_editX;
	LEEditorLineEdit*        m_editY;
	LEEditorLineEdit*        m_editZ;
	QWidget*                 m_container;
	QHBoxLayout*             m_layout;

	void ensureLayout();

	void setControlProperty();

	void connectSignal();
public:
	void setValue(float x, float y, float z);

	inline float getX();
	inline float getY();
	inline float getZ();

	glm::vec3 get();
	LEEditorVector3DField(const QString& title, float x, float y, float z);
};

class LEEditorVector4DField :public LEEditorPropertyField {
	Q_OBJECT
private:
	QLabel*                  m_labelX;
	QLabel*                  m_labelY;
	QLabel*                  m_labelZ;
	QLabel*                  m_labelW;
	LEEditorLineEdit*        m_editX;
	LEEditorLineEdit*        m_editY;
	LEEditorLineEdit*        m_editZ;
	LEEditorLineEdit*        m_editW;
	QVBoxLayout*             m_vlayout;
	QHBoxLayout*             m_hlayout_top;
	QHBoxLayout*             m_hlayout_bottom;
	QWidget*                 m_container_main;
	QWidget*                 m_container_top;
	QWidget*                 m_container_bottom;

	void ensureLayout();

	void setControlProperty();

	void connectSignal();
public:
	void setValue(float x, float y, float z, float w);

	float getX();
	float getY();
	float getZ();
	float getW();

	glm::vec4 get();

	LEEditorVector4DField(const QString& title, float x, float y, float z, float w);
};
#endif // !LE_EDITOR_VECTOR_FIELD_H
