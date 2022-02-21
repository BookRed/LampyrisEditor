/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_gui_data_keeper.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_gui_data_keeper.h"
// Qt Includes
#include <QMap>
#include <le_hash.h>

QMap<QWidget*, LEEditorWrapperWindow*> LEEditorGUIDataKeeper::ms_controlParentMap;
QMap<LEEditorWrapperWindow*, QMap<int, QWidget*>> LEEditorGUIDataKeeper::ms_windowControlMap;

LEEditorWrapperWindow* LEEditorGUIDataKeeper::getParentWindowByChildWidget(QWidget* widget) {
	if (LEEditorGUIDataKeeper::ms_controlParentMap.contains(widget)) {
		return LEEditorGUIDataKeeper::ms_controlParentMap[widget];
	}
	else {
		return nullptr;
	}
}

void LEEditorGUIDataKeeper::appendChildControl(LEEditorWrapperWindow* parentWindow, QWidget* widget, const int lineno,const char* name) {
	LEEditorGUIDataKeeper::ms_controlParentMap[widget] = parentWindow;
	LEEditorGUIDataKeeper::ms_windowControlMap[parentWindow][le_hash_value(lineno, std::hash<const char*>()(name))] = widget;
}
QWidget* LEEditorGUIDataKeeper::getControlByLineNo(LEEditorWrapperWindow* parentWindow, const int lineno, const char* name) {
	return LEEditorGUIDataKeeper::ms_windowControlMap[parentWindow][le_hash_value(lineno, std::hash<const char*>()(name))];
}
