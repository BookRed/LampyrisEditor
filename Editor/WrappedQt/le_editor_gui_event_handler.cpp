/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_gui_event_handler.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_wrapped_window.h"
#include "le_editor_gui_data_keeper.h"
#include "le_editor_gui_event_handler.h"

void LEEditorGUIEventHandler::slotHandle(QWidget* object) {
	LEEditorWrapperWindow* window = LEEditorGUIDataKeeper::getParentWindowByChildWidget(object);
	this->m_sender = object;
	window->beginDraw();
	window->update();
	window->onGUI();
	window->endDraw();
	this->m_sender = nullptr;
}

QWidget* LEEditorGUIEventHandler::getLastSender() const {
	return this->m_sender;
}
