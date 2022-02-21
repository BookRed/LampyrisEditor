#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_wrapped_slider.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_WRAPPED_SLIDER_H
#define LE_EDITOR_WRAPPED_SLIDER_H
// QT Includes
#include <QSlider>
#include <QHBoxLayout>
// LE Includes
#include "le_editor_property_field.h"
#include "le_editor_wrapped_line_edit.h"

class LEEditorWrappedSlider :public LEEditorPropertyField {
	Q_OBJECT
private:
	QHBoxLayout*             m_layout;
	QWidget*                 m_widget;
	QSlider*                 m_slider;
	LEEditorWrappedLineEdit* m_edit;
public:
	                         LEEditorWrappedSlider(const QString& title, const int min, const int max, const int step, QWidget* parent = nullptr);
	LEEditorWrappedSlider(const QString& title, int min, int max, int step);

	inline void              setMax(const int max) { this->m_slider->setMaximum(max); }
				             
	inline void              setMin(const int min) { this->m_slider->setMinimum(min); }
				             
	inline void              setStep(const int step) { this->m_slider->setSingleStep(step); }
				             
	inline void              setValue(const int value) { this->m_slider->setValue(value); }
				             
	inline int               getValue() const { return this->m_slider->value(); }
};
#endif // !LE_EDITOR_WRAPPED_SLIDER_H
