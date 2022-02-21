#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor/Launcher
 * \File:    le_launcher_tab_style.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_LAUNCHER_TAB_STYLE_H
#define LE_LAUNCHER_TAB_STYLE_H

// QT Includes
#include <QPainter>
#include <QProxyStyle>
#include <QStyleOptionTab>

class LELauncherTabStyle : public QProxyStyle {
public:
	QSize sizeFromContents(ContentsType type, 
		                   const QStyleOption* option,
						   const QSize& size, 
		                   const QWidget* widget) const;

	void drawControl(ControlElement element, 
		             const QStyleOption* option, 
		             QPainter* painter, 
		             const QWidget* widget) const;
};

#endif // !LE_LAUNCHER_TAB_STYLE_H