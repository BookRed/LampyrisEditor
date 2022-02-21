/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    main.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// QT Includes
#include <QtWidgets/QApplication>
// LE Includes
#include "Runtime/Base/le_application.h"
#include "Launcher/le_lampyris_editor_launcher_window.h"

// the entry point of Lampyris Editor

// 感谢过来点开我的的
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

int main(int argc, char *argv[]) {
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication a(argc, argv);
    QFont font;
    font.setFamily(QString::fromUtf8("Microsoft YaHei"));
    font.setPointSize(9);
    a.setFont(font);
    LEApplication::getInstance()->setCommandLineParameter(argc, argv);
    LELampyrisEditorLauncherWindow w;
    w.show();
    return a.exec();
}
