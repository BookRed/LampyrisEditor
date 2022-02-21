#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor/Launcher
 * \File:    le_launcher_tab_widget.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_LAUNCHER_TAB_WIDGET_H
#define LE_LAUNCHER_TAB_WIDGET_H

// QT Includes
#include <QTabWidget>

class LELauncherTabWidget:public QTabWidget {
	Q_OBJECT
public:
	explicit LELauncherTabWidget(QWidget* parent = nullptr);

	~LELauncherTabWidget();
};

#endif // !LE_LAUNCHER_TAB_WIDGET_H