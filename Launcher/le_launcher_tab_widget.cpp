/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor/Launcher
 * \File:    le_launcher_tab_widget.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_launcher_tab_widget.h"
#include "le_launcher_tab_style.h"

LELauncherTabWidget::LELauncherTabWidget(QWidget* parent) :QTabWidget(parent) {
	this->tabBar()->setStyle(new LELauncherTabStyle);
	this->setTabPosition(QTabWidget::West);
}

LELauncherTabWidget::~LELauncherTabWidget() {

}

