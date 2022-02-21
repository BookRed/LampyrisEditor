#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_gui_factory.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_GUI_FACTORY_H
#define LE_EDITOR_GUI_FACTORY_H
// LE Includes
#include "le_editor_wrapped_window.h"
#include "le_editor_gui_event_handler.h"
#include "le_editor_wrapped_tip_box.h"
// QT Includes
#include <QStack>
// GLM Includes
#include <glm/glm.hpp>
class LEEditorGUIFactory {
	enum GUILayoutType {
		VERTICAL,
		HORIZONTAL
	};

private:
	LEEditorWrapperWindow*   m_parent;
	LEEditorGUIEventHandler* m_handler;
	QStack<QWidget*>         m_layoutStack;

	void                     clearLayout(QLayout* layout);
		                     
	void                     beginLayoutDetail(GUILayoutType type, int lineno);
public:
	                         LEEditorGUIFactory();
	                         
	                         LEEditorGUIFactory(LEEditorWrapperWindow* parent);
		                     
	void                     setTargetWindow(LEEditorWrapperWindow* parent);
		                     
	void                     begin();
		                     
	void                     end();
		                     
	void                     beginHLayout(int lineno);
		                     
	void                     beginVLayout(int lineno);
		                     
	void                     endLayout();

	int                      intField(int lineno, const char* title, int value = 0);

	float                    floatField(int lineno,const char* title, float value = 0.0f);
		                     
	QString                  stringField(int lineno,const char* title,const QString& value = QString());
		                     
	bool                     checkBox(int lineno,const char* title, bool value = false);
		                     
	int                      comboBox(int lineno,const char* title,const std::initializer_list<QString>& items,const int index = 0);
		                     
	bool                     button(int lineno,const char* text);
		                     
	float                    slider(int lineno, const char* title, int minValue, int maxValue, int step);
		                     
	void                     progressBar(int lineno, const char* title, const int value);
		                     
	void                     tipBox(int lineno, const char* text, LEEditorWrappedTipBox::TipIcon icon);
		                     
	glm::vec2                vector2Field(int lineno, const char* text, glm::vec2 vec);
		                     
	glm::vec3                vector3Field(int lineno, const char* text, glm::vec3 vec);
		                     
	glm::vec4                vector4Field(int lineno, const char* text, glm::vec4 vec);
		                     
	glm::vec4                colorField(int lineno, const char* text);
		                     
	bool                     componentSection(int lineno, const QString& title, bool canSetEnabled);
		                     
	void                     objectField(int lineno, const char* title);

	void                     splitter(int lineno);
};
#endif // !
