#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_wrapped_group_button.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_WRAPPED_GROUP_BUTTON_H
#define LE_EDITOR_WRAPPED_GROUP_BUTTON_H
// QT Includes
#include <QHBoxLayout>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

class LEEditorWrappedGroupButtonInternal :public QWidget {
	Q_OBJECT
public:
	enum DisplayMode {
		IconOnly,
		TextOnly,
		Both,
		MaxEnum = 0x7fffffff
	};
private:
	QVector<QPixmap>  m_icons;
	QVector<QString>  m_infos;
	DisplayMode       m_mode;

	static const int  sc_icon_width = 32;
	static const int  sc_icon_height = 32;

	static const int  sc_bounding_rect_radius = 3;
	static const int  sc_max_button_count = 16;

	int               m_hoveredMask;
	int               m_maxTextWidth;
	int               m_fontHeight;
protected:
	// the layout of this self-drawn control is:

	/*----------------------------------------*/
	/*| Btn 1 | Btn 2 | Btn 3 |  ...           |
	/*----------------------------------------*/

	void paintEvent(QPaintEvent* e);

	void mouseMoveEvent(QMouseEvent* e);

	void mousePressEvent(QMouseEvent* e);

	void leaveEvent(QEvent* e);
public:
	LEEditorWrappedGroupButtonInternal(DisplayMode               mode,
		                const QVector<QPixmap>& icons,
		                const QVector<QString>& infos);
Q_SIGNALS:
	void signalSelectedIndexChanged(int index);
};

class LEEditorWrappedGroupButton :public QWidget {
	Q_OBJECT
private:
	QHBoxLayout*                        m_layout;
	LEEditorWrappedGroupButtonInternal* m_button;
public:
	LEEditorWrappedGroupButton(const QVector<QPixmap>& icons,
		const QVector<QString>& infos);
Q_SIGNALS:
	void signalSelectedIndexChanged(int index);
};
#endif // !LE_EDITOR_WRAPPED_GROUP_BUTTON_H