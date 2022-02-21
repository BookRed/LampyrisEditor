#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_property_editor_widget.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_PROPERTY_EDITOR_WIDGET_H
#define LE_EDITOR_PROPERTY_EDITOR_WIDGET_H
// LE Includes
#include "WrappedQt/le_editor_wrapped_window.h"
#include "le_interface_property_editor_widget.h"

class LEEditorPropertyEditorWidget:public LEEditorWrapperWindow,public IPropertyEditorWidget {	
private:
	LEEntity* m_pEntity;
public:
	LEEditorPropertyEditorWidget();

	// interface methods
	void onGUI() override;

	void setEntity(LEEntity* pEntity) override;

	void setIdle() override;

	void refresh() override;
};
#endif // !LE_EDITOR_PROPERTY_EDITOR_WIDGET_H
