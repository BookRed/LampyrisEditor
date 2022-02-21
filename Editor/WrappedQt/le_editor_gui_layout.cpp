/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_gui_layout.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_gui_layout.h"

LEEditorGUIFactory* LEEditorGUILayout::ms_factory = nullptr;
LEEditorWrapperWindow* LEEditorGUILayout::ms_window = nullptr;

void LEEditorGUILayout::begin(LEEditorWrapperWindow* target) {
	if (ms_window != nullptr) {
		// LEEditorGUILayout::end() hadn't been called last time.
	}
	if (ms_factory == nullptr) {
		ms_factory = new LEEditorGUIFactory;
	}
	ms_window = target;
	ms_factory->setTargetWindow(target);
	ms_factory->begin();
}

void LEEditorGUILayout::end() {
	ms_window = nullptr;
	ms_factory->end();
}

void LEEditorGUILayout::beginHLayout(const int lineno) {
	ms_factory->beginHLayout(lineno);
}

void LEEditorGUILayout::beginVLayout(const int lineno) {
	ms_factory->beginVLayout(lineno);
}

void LEEditorGUILayout::endLayout() {
	ms_factory->endLayout();
}

int LEEditorGUILayout::intField(int lineno, const char* title, const int value) {
	return ms_factory->intField(lineno, title, value);
}

float LEEditorGUILayout::floatField(int lineno, const char* title, const float value) {
	return ms_factory->floatField(lineno, title, value);
}

QString LEEditorGUILayout::stringField(int lineno, const char* title, const char* value) {
	return ms_factory->stringField(lineno, title, value);
}

bool LEEditorGUILayout::checkBox(int lineno, const char* title, const bool value) {
	return ms_factory->checkBox(lineno, title, value);
}

int LEEditorGUILayout::comboBox(int lineno, const char* title, const std::initializer_list<QString>& items, const int index) {
	return ms_factory->comboBox(lineno, title, items, index);
}

bool LEEditorGUILayout::button(int lineno, const char* text) {
	return ms_factory->button(lineno, text);
}

float LEEditorGUILayout::slider(const int lineno, const char* title) {
	return LEEditorGUILayout::slider(lineno, title, 0, 1, 1);
}

float LEEditorGUILayout::slider(const int lineno, const char* title, const int step) {
	return LEEditorGUILayout::slider(lineno, title, 0, 1, step);
}

float LEEditorGUILayout::slider(const int lineno, const char* title, const int minValue, const int maxValue, const int step) {
	return ms_factory->slider(lineno, title, minValue, maxValue, step);
}

void LEEditorGUILayout::progressBar(const int lineno, const char* title, const int value) {
	ms_factory->progressBar(lineno, title, value);
}

void LEEditorGUILayout::tipBox(const int lineno, const char* text, const LEEditorWrappedTipBox::TipIcon icon) {
	return ms_factory->tipBox(lineno, text, icon);
}

glm::vec2 LEEditorGUILayout::vector2Field(const int lineno, const char* text, glm::vec2 vec) {
	return ms_factory->vector2Field(lineno, text, vec);
}

glm::vec3 LEEditorGUILayout::vector3Field(const int lineno, const char* text, glm::vec3 vec) {
	return ms_factory->vector3Field(lineno, text, vec);
}

glm::vec4 LEEditorGUILayout::vector4Field(const int lineno, const char* text, glm::vec4 vec) {
	return ms_factory->vector4Field(lineno, text, vec);
}

glm::vec4 LEEditorGUILayout::colorField(const int lineno, const char* text) {
	return ms_factory->colorField(lineno, text);
}

void LEEditorGUILayout::objectField(const int lineno, const char* title) {
	ms_factory->objectField(lineno, title);
}

bool LEEditorGUILayout::componentSection(const int lineno, const QString& title, bool canSetEnabled) {
	return ms_factory->componentSection(lineno, title, canSetEnabled);
}

void LEEditorGUILayout::splitter(const int lineno) {
	ms_factory->splitter(lineno);
}
