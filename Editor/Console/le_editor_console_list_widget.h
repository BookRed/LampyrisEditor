#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_console_list_widget.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_CONSOLE_WIDGET_H
#define LE_EDITOR_CONSOLE_WIDGET_H
// QT Includes
#include <QScrollBar>
#include <QWidget>
#include <QListWidget>
// LE Includes
#include "le_editor_console_message_item_widget.h"

namespace Lampyris::Detail {
class LEEditorConsoleListWidget : public QWidget {
	using ItemVector = QVector<Lampyris::Detail::ConsoleMessage>;
	using ItemWidgetMap = std::unordered_map<int, LEEditorConsoleMessageItemWidget*>;
	Q_OBJECT
private:
	int           m_shouldDisplayCount;
	QScrollBar*   m_customScrollBar;
	QListWidget*  m_listWidget;
	ItemVector*   m_pMessageVector;
	QVector<int>  m_messageIndices;
	ItemWidgetMap m_indexItemWidgetMap;
	bool          m_isAutoRoll;

	//添加一个Item
	void          addItemInternal(int relativePos, int msgIndex);
	//头插入一个Item
	void          insertItemInternal(int msgIndex);
protected:
	// qt-native events
	virtual void  wheelEvent(QWheelEvent* event);

	virtual void  resizeEvent(QResizeEvent* event);
public:
	explicit      LEEditorConsoleListWidget(QWidget* parent = 0);

	             ~LEEditorConsoleListWidget();

	void          setAutoRoll(bool value);
		          
	void          setMessagePtr(ItemVector* _msgPtr);
		          
	void          setValidIndexes(const QVector<int>& _indexes);
		          
	void          clear();

	void          refreshAfterResize();

	void          refreshAfterSliderValueChanged();

	void          refreshAfterIndexesChanged();

	void          refreshAfterPrint(int index);
};
#endif // !LE_EDITOR_CONSOLE_WIDGET_H
}