#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_lampyris_editor_main_window.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_LAMPYRIS_EDITOR_MAIN_WINDOW_H
#define LE_LAMPYRIS_EDITOR_MAIN_WINDOW_H

// QT Includes
#include <ui_le_editor_launcher.h>
#include "ui_le_editor_window.h"

/********************************************************************************************************************
*  __  __               _                                      _                _           _                       *
* |  \/  |             | |                                    | |              | |         | |                      *
* | \  / | _   _     __| | _ __  ___   __ _  _ __ ___     ___ | |_  __ _  _ __ | |_  ___   | |__    ___  _ __  ___  *
* | |\/| || | | |   / _` || '__|/ _ \ / _` || '_ ` _ \   / __|| __|/ _` || '__|| __|/ __|  | '_ \  / _ \| '__|/ _ \ *
* | |  | || |_| |  | (_| || |  |  __/| (_| || | | | | |  \__ \| |_| (_| || |   | |_ \__ \  | | | ||  __/| |  |  __/ *
* |_|  |_| \__, |   \__,_||_|   \___| \__,_||_| |_| |_|  |___/ \__|\__,_||_|    \__||___/  |_| |_| \___||_|   \___| *
* 		   __/ |                                                                                                    *
* 		  |___/                                                                                                     *
*                                                                                                                   *
********************************************************************************************************************/

// the main window of lampyris editor
// it consists of following parts:  
// 1) The menu bar at the top of the window, which is extensible with python scripts.
// 2) The tool bar under the menu bar, which provide some buttons of basic operation.
// 3) The object tree view of entites at the left
// 4) The asset explorer at the left bottom
// 5) The output console at the center bottom
// 6) The property editor at the right
// 7) The status bar at the most bottom, which outputs simple messages, and provides some buttons

class LELampyrisEditorMainWindow : public QMainWindow {
    Q_OBJECT
private:
    Ui::LampyrisEditorWindow window;
public:
    LELampyrisEditorMainWindow(QWidget *parent = Q_NULLPTR);

    // inject key event to sandbox window
    void keyPressEvent(QKeyEvent* event) override {
        this->window.widget_sandBox->keyPressEvent(event);
    }

    void keyReleaseEvent(QKeyEvent* event) override {
        this->window.widget_sandBox->keyReleaseEvent(event);
    }
    void closeEvent(QCloseEvent* event) override;
};

#endif // !LE_LAMPYRIS_EDITOR_MAIN_WINDOW_H