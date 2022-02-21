#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_wrapped_color_field.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_WRAPPED_COLOR_FIELD_H
#define LE_EDITOR_WRAPPED_COLOR_FIELD_H
// LE Includes
#include "le_editor_colored_button.h"
#include "le_editor_property_field.h"

class LEEditorWrappedColorField :public LEEditorPropertyField {
	Q_OBJECT
private:
	LEEditorColoredButton* m_button;

	void      dispatchEvent();
public:
        	  LEEditorWrappedColorField(const QString& name);

	void      setColor(glm::vec4 color) { this->m_button->setColor(color); }

	glm::vec4 getColor() { return this->m_button->getColor(); }
};
#endif // !LE_EDITOR_WRAPPED_COLOR_FIELD_H
