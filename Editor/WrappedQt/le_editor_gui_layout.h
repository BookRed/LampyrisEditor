#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_gui_layout.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_GUI_LAYOUT_H
#define LE_EDITOR_GUI_LAYOUT_H
// LE Includes
#include "le_editor_gui_factory.h"
#include "le_editor_wrapped_window.h"
// GLM Includes
#include <glm/glm.hpp>

class LEEditorGUILayout {
private:
	static LEEditorGUIFactory* ms_factory;
	static LEEditorWrapperWindow* ms_window;
public:
	static void begin(LEEditorWrapperWindow* target);

	static void end();

	static void beginHLayout(const int lineno);

	static void beginVLayout(const int lineno);

	static void endLayout();

	static int intField(const int lineno,
	                    const char* title,
	                    const int value = 0);

	static float floatField(const int lineno,
	                        const char* title,
	                        const float value = 0.0f);

	static QString stringField(const int lineno,
	                           const char* title,
	                           const char* value);

	static bool checkBox(const int lineno,
	                     const char* title,
	                     const bool value = false);

	static int comboBox(const int lineno,
	                    const char* title,
	                    const std::initializer_list<QString>& items,
	                    const int index = 0);

	static bool button(const int lineno,
	                   const char* text);


	static float slider(const int lineno,
	                    const char* title);

	static float slider(const int lineno,
	                    const char* title,
	                    const int step);

	static float slider(const int lineno,
	                    const char* title,
	                    const int minValue,
	                    const int maxValue,
	                    const int step);

	static void progressBar(const int lineno,
	                        const char* title,
	                        const int value);

	static void tipBox(const int lineno,
	                   const char* text,
	                   const LEEditorWrappedTipBox::TipIcon icon);

	static glm::vec2 vector2Field(const int lineno,
	                              const char* text,
	                              glm::vec2 vec2);

	static glm::vec3 vector3Field(const int lineno,
	                              const char* text,
	                              glm::vec3 vec3);

	static glm::vec4 vector4Field(const int lineno,
	                              const char* text,
		                          glm::vec4 vec4);

	static glm::vec4 colorField(const int lineno, const char* text);

	static void objectField(const int lineno, const char* title);

	static bool componentSection(const int lineno,
		                         const QString& title, 
		                         bool canSetEnabled);

	static void splitter(const int lineno);
};

#define LE_EDITOR_LAYOUT_BUTTON(btnName)  \
	LEEditorGUILayout::button(__LINE__,btnName)

#define LE_EDITOR_LAYOUT_VECTOR3(vec3Name)  \
	LEEditorGUILayout::vector3Field(__LINE__,vec3Name,0,0,0)


#endif // !LE_EDITOR_GUI_LAYOUT_H
