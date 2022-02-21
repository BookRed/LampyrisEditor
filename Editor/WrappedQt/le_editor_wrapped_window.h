#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_wrapped_window.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_WRAPPED_WINDOW_H
#define LE_EDITOR_WRAPPED_WINDOW_H
// QT Includes
#include <qcoreevent.h>
#include <QGridLayout>
#include <QScrollArea>
#include <QPainter>
#include <QPaintEvent>

class LEEditorWrapperWindow :public QScrollArea {
	Q_OBJECT
protected:
	QWidget*             m_scrollAreaWidgetContents;
	QGridLayout*         m_layout;
	QSpacerItem*         m_spacerItem;
	QPixmap              m_snapshot;
	bool                 m_drawing;
	                     LEEditorWrapperWindow();

	bool eventFilter(QObject* watched, QEvent* event) override {
		if(watched == this->m_scrollAreaWidgetContents && event->type() == QEvent::Paint) {
			QPainter painter(m_scrollAreaWidgetContents);
			if (m_drawing) {
				// if paint event is triggered during executing 'onGUI', draw snapshot instead.
				painter.drawPixmap(0, 0, this->m_snapshot);
			}
			else {
				QWidget::paintEvent((QPaintEvent*)event);
			}
			return true;
		}
		return QWidget::eventFilter(watched, event);
	}
public:
	// the methods should be called before 'onGUI' 
	void                 beginDraw() {
		this->m_snapshot = this->m_scrollAreaWidgetContents->grab();
		this->m_drawing = true;
	}

	void                 endDraw() {
		this->m_drawing = false;
	}

	virtual void         onGUI() = 0;

	// the methods should be called at the end of 'onGUI' 
	QSpacerItem* getLastSpacer() const {
		return this->m_spacerItem;
	}
};
#endif // !LE_EDITOR_WRAPPED_WINDOW_H