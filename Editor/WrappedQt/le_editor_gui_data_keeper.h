#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_gui_data_keeper.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_GUI_DATA_KEEPER
#define LE_EDITOR_GUI_DATA_KEEPER
// LE Includes
#include "le_editor_wrapped_window.h"

class LEEditorGUIDataKeeper {
	typedef QMap<int, QWidget*>                        ControlMap_t;
	typedef QMap<QWidget*, LEEditorWrapperWindow*>     ParentChildMap_t;
	typedef QMap<LEEditorWrapperWindow*, ControlMap_t> WindowControlMap_t;
private:
	static ParentChildMap_t   ms_controlParentMap;
	static WindowControlMap_t ms_windowControlMap;
public:
	static LEEditorWrapperWindow* getParentWindowByChildWidget(QWidget* widget);

	static void appendChildControl(LEEditorWrapperWindow* parentWindow,
	                               QWidget* widget,
	                               const int lineno, const char* name = "");
	static QWidget* getControlByLineNo(LEEditorWrapperWindow* parentWindow,
	                                   const int lineno,const char* name = "");
};
#endif // !LE_EDITOR_GUI_DATA_KEEPER