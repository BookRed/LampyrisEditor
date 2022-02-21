/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_gui_factory.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_gui_factory.h"
#include "le_editor_gui_data_keeper.h"
#include "le_editor_wrapped_line_edit.h"
#include "le_editor_gui_event_handler.h"
#include "le_editor_vector_field.h"
#include "le_editor_wrapped_button.h"
#include "le_editor_wrapped_check_box.h"
#include "le_editor_wrapped_color_field.h"
#include "le_editor_wrapped_object_field.h"
#include "le_editor_wrapped_progress_bar.h"
#include "le_editor_wrapped_slider.h"
#include "le_editor_wrapped_combo_box.h"
#include "le_editor_wrapped_component_section.h"
#include "Runtime/Base/le_assert.h"

void LEEditorGUIFactory::clearLayout(QLayout* layout) {
	QLayoutItem* child;
	while ((child = layout->takeAt(0)) != 0) {
		if (child->widget()) {
			child->widget()->hide();
		}
	}
}

void LEEditorGUIFactory::beginLayoutDetail(GUILayoutType type, int lineno) {
	QWidget* childWidget = LEEditorGUIDataKeeper::getControlByLineNo(m_parent, lineno);
	if (!childWidget) { // Create a new layout according to given type
		// add child layout to parent's
		QWidget* parentWidget = this->m_layoutStack.top();
		assert(parentWidget != NULL);
		QLayout* parentLayout = parentWidget->layout();
		childWidget = new QWidget(parentWidget);
		QLayout* pLayout = nullptr;
		if (type == GUILayoutType::VERTICAL) {
			pLayout = new QVBoxLayout(childWidget);
		}
		else if (type == GUILayoutType::HORIZONTAL) {
			pLayout = new QHBoxLayout(childWidget);
		}
		childWidget->setLayout(pLayout);
		pLayout->setContentsMargins(0, 0, 0, 0);
		pLayout->setSpacing(3);
		childWidget->setContentsMargins(0, 0, 0, 0);
		LEEditorGUIDataKeeper::appendChildControl(m_parent, childWidget, lineno);
		parentLayout->addWidget(childWidget);
	}
	else {
		// if layout exists before,clear all items of it
		this->clearLayout(childWidget->layout());
	}
	childWidget->show();
	this->m_layoutStack.push(childWidget);
}

LEEditorGUIFactory::LEEditorGUIFactory():m_handler(new LEEditorGUIEventHandler) {}

LEEditorGUIFactory::LEEditorGUIFactory(LEEditorWrapperWindow* parent) {}

void LEEditorGUIFactory::setTargetWindow(LEEditorWrapperWindow* parent) {
	this->m_parent = parent;
}

void LEEditorGUIFactory::begin() {
	this->m_layoutStack.clear();
	this->m_layoutStack.push_back(m_parent->widget());
}

void LEEditorGUIFactory::end() {
	this->m_layoutStack.top()->layout()->addItem(this->m_parent->getLastSpacer());
}

void LEEditorGUIFactory::beginHLayout(int lineno) {
	this->beginLayoutDetail(GUILayoutType::HORIZONTAL, lineno);
}

void LEEditorGUIFactory::beginVLayout(int lineno) {
	this->beginLayoutDetail(GUILayoutType::VERTICAL, lineno);
}

void LEEditorGUIFactory::endLayout() {
	this->m_layoutStack.pop();
	LE_ASSERT(this->m_layoutStack.size() >= 1);
}

int LEEditorGUIFactory::intField(int lineno, const char* title, int value) {
	LEEditorWrappedLineEdit* lineEdit = nullptr;
	QWidget* control = LEEditorGUIDataKeeper::getControlByLineNo(this->m_parent, lineno, title);
	int returnValue;
	if (control) {
		lineEdit = qobject_cast<LEEditorWrappedLineEdit*>(control);
		lineEdit->setTitle(title);
		lineEdit->show();
		returnValue = lineEdit->text().toInt();
	}
	else {
		lineEdit = new LEEditorWrappedLineEdit(title, 0);
		lineEdit->setText(QString::number(value));
		LEEditorGUIDataKeeper::appendChildControl(m_parent, lineEdit, lineno, title);
		QObject::connect(lineEdit, &LEEditorWrappedLineEdit::signalEventDispatch, m_handler, &LEEditorGUIEventHandler::slotHandle);
		returnValue = value;
	}
	this->m_layoutStack.top()->layout()->addWidget(lineEdit);
	return returnValue;
}

float LEEditorGUIFactory::floatField(int lineno, const char* title, float value) {
	LEEditorWrappedLineEdit* lineEdit = nullptr;
	QWidget* control = LEEditorGUIDataKeeper::getControlByLineNo(this->m_parent, lineno, title);
	float returnValue;
	if (control) {
		lineEdit = qobject_cast<LEEditorWrappedLineEdit*>(control);
		lineEdit->setTitle(title);
		lineEdit->show();
		if (m_handler->getLastSender() == control) {
			returnValue = lineEdit->text().toFloat();
		}
		else {
			lineEdit->setText(QString::number(value));
			returnValue = value;
		}
	}
	else {
		lineEdit = new LEEditorWrappedLineEdit(title, 1);
		lineEdit->setText(QString::number(value));
		LEEditorGUIDataKeeper::appendChildControl(m_parent, lineEdit, lineno, title);
		QObject::connect(lineEdit, &LEEditorWrappedLineEdit::signalEventDispatch, m_handler, &LEEditorGUIEventHandler::slotHandle);
		returnValue = value;
	}
	this->m_layoutStack.top()->layout()->addWidget(lineEdit);
	return returnValue;
}

QString LEEditorGUIFactory::stringField(int lineno, const char* title, const QString& value) {
	LEEditorWrappedLineEdit* lineEdit = nullptr;
	QWidget* control = LEEditorGUIDataKeeper::getControlByLineNo(this->m_parent, lineno, title);
	QString returnValue;
	if (control) {
		lineEdit = qobject_cast<LEEditorWrappedLineEdit*>(control);
		lineEdit->setTitle(title);
		lineEdit->show();
		if(this->m_handler->getLastSender() == lineEdit) {
			returnValue = lineEdit->text();
		} else {
			lineEdit->setText(value);
			returnValue = value;
		}
	}
	else {
		lineEdit = new LEEditorWrappedLineEdit(title, 2);
		lineEdit->setText(value);
		LEEditorGUIDataKeeper::appendChildControl(m_parent, lineEdit, lineno, title);
		QObject::connect(lineEdit, &LEEditorWrappedLineEdit::signalEventDispatch, m_handler, &LEEditorGUIEventHandler::slotHandle);
		returnValue = value;
	}
	this->m_layoutStack.top()->layout()->addWidget(lineEdit);
	return returnValue;
}

bool LEEditorGUIFactory::checkBox(int lineno, const char* title, bool value) {
	LEEditorWrappedCheckBox* checkBox = nullptr;
	QWidget* control = LEEditorGUIDataKeeper::getControlByLineNo(this->m_parent, lineno, title);
	bool returnValue;
	if (control) {
		checkBox = qobject_cast<LEEditorWrappedCheckBox*>(control);
		checkBox->setTitle(title);
		checkBox->show();
		returnValue = checkBox->isChecked();
	}
	else {
		checkBox = new LEEditorWrappedCheckBox(title, value);
		LEEditorGUIDataKeeper::appendChildControl(m_parent, checkBox, lineno, title);
		checkBox->setChecked(value);
		QObject::connect(checkBox, &LEEditorWrappedCheckBox::signalEventDispatch, m_handler, &LEEditorGUIEventHandler::slotHandle);
		returnValue = value;
	}
	this->m_layoutStack.top()->layout()->addWidget(checkBox);
	return returnValue;
}

int LEEditorGUIFactory::comboBox(int lineno, const char* title, const std::initializer_list<QString>& items, int index) {
	LEEditorWrappedComboBox* comboBox = nullptr;
	QWidget* control = LEEditorGUIDataKeeper::getControlByLineNo(this->m_parent, lineno, title);
	int returnValue;
	if (control) {
		comboBox = qobject_cast<LEEditorWrappedComboBox*>(control);
		comboBox->setTitle(title);
		comboBox->show();
		returnValue = comboBox->currentIndex();
	}
	else {
		comboBox = new LEEditorWrappedComboBox(title, items);
		LEEditorGUIDataKeeper::appendChildControl(m_parent, comboBox, lineno, title);
		comboBox->setCurrentIndex(index);
		QObject::connect(comboBox, &LEEditorWrappedComboBox::signalEventDispatch, m_handler, &LEEditorGUIEventHandler::slotHandle);
		returnValue = index;
	}
	this->m_layoutStack.top()->layout()->addWidget(comboBox);
	return returnValue;
}

bool LEEditorGUIFactory::button(int lineno, const char* text) {
	LEEditorWrappedButton* button = nullptr;
	QWidget* control = LEEditorGUIDataKeeper::getControlByLineNo(this->m_parent, lineno, text);
	bool returnValue;
	if (control) {
		button = qobject_cast<LEEditorWrappedButton*>(control);
		button->show();
		returnValue = button->isClickedLastTime();
	}
	else {
		button = new LEEditorWrappedButton(text, this->m_layoutStack.top());
		LEEditorGUIDataKeeper::appendChildControl(m_parent, button, lineno, text);
		QObject::connect(button, &LEEditorWrappedButton::signalEventDispatch, m_handler, &LEEditorGUIEventHandler::slotHandle);
		returnValue = false;
	}
	this->m_layoutStack.top()->layout()->addWidget(button);
	return returnValue;
}

float LEEditorGUIFactory::slider(int lineno, const char* title, const int minValue, const int maxValue, int step) {
	LEEditorWrappedSlider* slider = nullptr;
	QWidget* control = LEEditorGUIDataKeeper::getControlByLineNo(this->m_parent, lineno,title);
	float returnValue;
	if (control) {
		slider = qobject_cast<LEEditorWrappedSlider*>(control);
		slider->show();
		returnValue = slider->getValue();
	}
	else {
		slider = new LEEditorWrappedSlider(title, minValue, maxValue, step,this->m_parent);
		LEEditorGUIDataKeeper::appendChildControl(m_parent, slider, lineno,title);
		QObject::connect(slider, &LEEditorWrappedSlider::signalEventDispatch, m_handler, &LEEditorGUIEventHandler::slotHandle);
		returnValue = minValue;
	}
	this->m_layoutStack.top()->layout()->addWidget(slider);
	return returnValue;
}

void LEEditorGUIFactory::progressBar(int lineno, const char* title, int value) {
	LEEditorWrappedProgressBar* progressBar = nullptr;
	QWidget* control = LEEditorGUIDataKeeper::getControlByLineNo(this->m_parent, lineno, title);
	if (control) {
		progressBar = qobject_cast<LEEditorWrappedProgressBar*>(control);
		progressBar->setValue(value);
		progressBar->show();
	}
	else {
		progressBar = new LEEditorWrappedProgressBar(title, value);
		LEEditorGUIDataKeeper::appendChildControl(m_parent, progressBar, lineno, title);
	}
	this->m_layoutStack.top()->layout()->addWidget(progressBar);
}

void LEEditorGUIFactory::tipBox(int lineno, const char* text, const LEEditorWrappedTipBox::TipIcon icon) {
	LEEditorWrappedTipBox* tipBox = nullptr;
	QWidget* control = LEEditorGUIDataKeeper::getControlByLineNo(this->m_parent, lineno, text);
	if (control) {
		tipBox = qobject_cast<LEEditorWrappedTipBox*>(control);
		tipBox->setText(text);
		tipBox->show();
	}
	else {
		tipBox = new LEEditorWrappedTipBox(text, icon);
		LEEditorGUIDataKeeper::appendChildControl(m_parent, tipBox, lineno, text);
	}
	this->m_layoutStack.top()->layout()->addWidget(tipBox);
}

glm::vec2 LEEditorGUIFactory::vector2Field(int lineno, const char* text, glm::vec2 vec) {
	LEEditorVector2DField* vec2field = nullptr;
	QWidget* control = LEEditorGUIDataKeeper::getControlByLineNo(this->m_parent, lineno);
	if (control) {
		vec2field = qobject_cast<LEEditorVector2DField*>(control);
		vec2field->setValue(vec.x, vec.y);
		vec2field->show();
	}
	else {
		vec2field = new LEEditorVector2DField(text, vec.x, vec.y);
		LEEditorGUIDataKeeper::appendChildControl(m_parent, vec2field, lineno,text);
		QObject::connect(vec2field, &LEEditorVector2DField::signalEventDispatch, m_handler, &LEEditorGUIEventHandler::slotHandle);
	}
	this->m_layoutStack.top()->layout()->addWidget(vec2field);
	return vec2field->get();
}

glm::vec3 LEEditorGUIFactory::vector3Field(int lineno, const char* text, glm::vec3 vec) {
	LEEditorVector3DField* vec3field = nullptr;
	QWidget* control = LEEditorGUIDataKeeper::getControlByLineNo(this->m_parent, lineno, text);
	if (control) {
		vec3field = qobject_cast<LEEditorVector3DField*>(control);
		if(this->m_handler->getLastSender() != vec3field) {
			vec3field->setValue(vec.x, vec.y, vec.z);
		}
		vec3field->show();
	}
	else {
		vec3field = new LEEditorVector3DField(text, vec.x, vec.y, vec.z);
		LEEditorGUIDataKeeper::appendChildControl(m_parent, vec3field, lineno, text);
		QObject::connect(vec3field, &LEEditorVector3DField::signalEventDispatch, m_handler, &LEEditorGUIEventHandler::slotHandle);
	}
	this->m_layoutStack.top()->layout()->addWidget(vec3field);
	return vec3field->get();
}

glm::vec4 LEEditorGUIFactory::vector4Field(int lineno, const char* text, glm::vec4 vec) {
	LEEditorVector4DField* vec4field = nullptr;
	QWidget* control = LEEditorGUIDataKeeper::getControlByLineNo(this->m_parent, lineno, text);
	if (control) {
		vec4field = qobject_cast<LEEditorVector4DField*>(control);
		vec4field->setValue(vec.x, vec.y, vec.z, vec.w);
		vec4field->show();
	}
	else {
		vec4field = new LEEditorVector4DField(text, vec.x, vec.y, vec.z, vec.w);
		LEEditorGUIDataKeeper::appendChildControl(m_parent, vec4field, lineno, text);
		QObject::connect(vec4field, &LEEditorVector4DField::signalEventDispatch, m_handler, &LEEditorGUIEventHandler::slotHandle);
	}
	this->m_layoutStack.top()->layout()->addWidget(vec4field);
	return vec4field->get();
}

glm::vec4 LEEditorGUIFactory::colorField(int lineno, const char* text) {
	LEEditorWrappedColorField* colorField = nullptr;
	QWidget* control = LEEditorGUIDataKeeper::getControlByLineNo(this->m_parent, lineno, text);
	if (control) {
		colorField = qobject_cast<LEEditorWrappedColorField*>(control);
		//colorField->setValue(x, y, z, w);
		colorField->show();
	}
	else {
		colorField = new LEEditorWrappedColorField(text);
		LEEditorGUIDataKeeper::appendChildControl(m_parent, colorField, lineno, text);
		QObject::connect(colorField, &LEEditorWrappedColorField::signalEventDispatch, m_handler, &LEEditorGUIEventHandler::slotHandle);
	}
	this->m_layoutStack.top()->layout()->addWidget(colorField);
	return colorField->getColor();
}

bool LEEditorGUIFactory::componentSection(int lineno, const QString& title, bool canSetEnabled) {
	LEEditorWrappedComponentSection* section = nullptr;
	QWidget* control = LEEditorGUIDataKeeper::getControlByLineNo(this->m_parent, lineno);
	if (control) {
		section = qobject_cast<LEEditorWrappedComponentSection*>(control);
		section->show();
	}
	else {
		section = new LEEditorWrappedComponentSection(title);
		LEEditorGUIDataKeeper::appendChildControl(m_parent, section, lineno);
		QObject::connect(section, &LEEditorWrappedComponentSection::signalEventDispatch, m_handler, &LEEditorGUIEventHandler::slotHandle);
	}
	this->m_layoutStack.top()->layout()->addWidget(section);
	return section->isOpened();
}

void LEEditorGUIFactory::objectField(int lineno, const char* title) {
	LEEditorWrappedObjectField* objectField = nullptr;
	QWidget* control = LEEditorGUIDataKeeper::getControlByLineNo(this->m_parent, lineno, title);
	if (control) {
		objectField = qobject_cast<LEEditorWrappedObjectField*>(control);
		objectField->show();
	}
	else {
		objectField = new LEEditorWrappedObjectField(title);
		LEEditorGUIDataKeeper::appendChildControl(m_parent, objectField, lineno, title);
		QObject::connect(objectField, &LEEditorWrappedObjectField::signalEventDispatch, m_handler, &LEEditorGUIEventHandler::slotHandle);
	}
	this->m_layoutStack.top()->layout()->addWidget(objectField);
}
void LEEditorGUIFactory::splitter(int lineno) {
	QFrame* line = nullptr;
	QWidget* control = LEEditorGUIDataKeeper::getControlByLineNo(this->m_parent, lineno);
	if (control) {
		line = qobject_cast<QFrame*>(control);
		line->show();
	}
	else {
		line = new QFrame;
		line->setFrameShape(QFrame::HLine);
		LEEditorGUIDataKeeper::appendChildControl(m_parent, line, lineno);
	}
	this->m_layoutStack.top()->layout()->addWidget(line);
}
