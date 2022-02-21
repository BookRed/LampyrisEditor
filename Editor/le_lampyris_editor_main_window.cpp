/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_lampyris_editor_main_window.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_manager.h"
#include "le_lampyris_editor_main_window.h"
#include "le_editor_property_editor_widget.h"

LELampyrisEditorMainWindow::LELampyrisEditorMainWindow(QWidget *parent)
    : QMainWindow(parent) {
    // start up lampyris application, to execute all init methods of each sub-modules
    LEApplication::getInstance()->doStartUp();
    // init widget's UI
    this->window.setupUi(this);
    this->setDockNestingEnabled(true);
    LEEditorPropertyEditorWidget* pPropertyEditorWidget = new LEEditorPropertyEditorWidget;
    this->window.DockWidget_Inspector->setWidget(pPropertyEditorWidget);
    // inject windows of manager
    LampyrisEditorManager::getInstance()->m_pEntityTreeWidget = this->window.EntityTreeWidget;
    LampyrisEditorManager::getInstance()->m_pPropertyEditorWidget = pPropertyEditorWidget;
}
void LELampyrisEditorMainWindow::closeEvent(QCloseEvent* event) {
	LEApplication::getInstance()->doShutDown();
	QApplication::quit();
}
