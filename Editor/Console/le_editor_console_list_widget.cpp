/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_console_list_widget.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_console_list_widget.h"
// QT Includes
#include <QHBoxLayout>
#include <QWheelEvent>

namespace Lampyris::Detail {
LEEditorConsoleListWidget::LEEditorConsoleListWidget(QWidget* parent):
	m_shouldDisplayCount(4),
	m_listWidget(new QListWidget(this)),
	m_customScrollBar(new QScrollBar(this)) {
	// the main layout of console Widget
	QHBoxLayout* layout = new QHBoxLayout;
	this->setLayout(layout);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(this->m_listWidget);
	layout->addWidget(this->m_customScrollBar);

	// [ListWidget]
	// disable default m_customScrollBar of m_listWidget
	this->m_listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->m_listWidget->setSelectionMode(QListWidget::SelectionMode::ExtendedSelection);
	this->m_listWidget->setStyleSheet("QListWidget::item:selected"
		                               "{background: qlineargradient(x1 : 0, y1 : 0,"
		                               "x2 : 0, y2 : 1,"
		                               "stop : 0 #6ea1f1, stop: 1 #567dbc);} ");

	// [ScrollBar]
	this->m_customScrollBar->setSingleStep(1);
	this->m_customScrollBar->setValue(0);
	QObject::connect(this->m_customScrollBar, &QScrollBar::valueChanged, this,
	                 &LEEditorConsoleListWidget::refreshAfterSliderValueChanged);
}

LEEditorConsoleListWidget::~LEEditorConsoleListWidget() {
}

void LEEditorConsoleListWidget::addItemInternal(int relativePos, int msgIndex) {
	LEEditorConsoleMessageItemWidget* widget = NULL;
	// 找到对应显示位置的ItemWidget然后更新Message
	if (this->m_indexItemWidgetMap.find(relativePos) != this->m_indexItemWidgetMap.end()) {
		this->m_listWidget->item(relativePos)->setHidden(false);
		widget = this->m_indexItemWidgetMap[relativePos];
		widget->setMessage(this->m_pMessageVector->at(msgIndex));
	}
	// 如果没找到对应位置的ItemWidget,那么就new一个
	else {
		LEEditorConsoleMessageItemWidget* widget = new LEEditorConsoleMessageItemWidget(this->m_listWidget, this->m_pMessageVector->at(msgIndex));
		QListWidgetItem* item = new QListWidgetItem;
		item->setSizeHint(QSize(item->sizeHint().width(), 60));
		widget->setAttribute(Qt::WA_DeleteOnClose);
		this->m_listWidget->addItem(item);
		this->m_listWidget->setItemWidget(item, widget);
		this->m_indexItemWidgetMap[relativePos] = widget;
	}
}

//头插入一个Item
void LEEditorConsoleListWidget::insertItemInternal(int msgIndex) {
	LEEditorConsoleMessageItemWidget* widget = NULL;
	if (this->m_indexItemWidgetMap.find(0) == this->m_indexItemWidgetMap.end()) {
		this->m_listWidget->item(0)->setHidden(false);
		widget = this->m_indexItemWidgetMap[0];
		widget->setMessage(this->m_pMessageVector->at(msgIndex));
	}
}

void LEEditorConsoleListWidget::wheelEvent(QWheelEvent* event) {
	int numDegrees = event->angleDelta().y() / 8;
	int numSteps = -numDegrees / 15;
	if (event->orientation() == Qt::Vertical) {
		//index为增量后的listWidget应该显示的第一个元素的索引值
		//重新显示条目
		//用于记录Item的数量
		this->m_customScrollBar->setValue(numSteps + this->m_customScrollBar->value());
		this->refreshAfterSliderValueChanged();
	}
	event->accept();
}

void LEEditorConsoleListWidget::resizeEvent(QResizeEvent* event) {
	// re-calculate item count to display
	int height                 = event->size().height();
	int value                  = m_customScrollBar->value();
	const int itemWidgetHeight = 60;
	this->m_shouldDisplayCount         = std::max(4,height / itemWidgetHeight)+1;
	if (this->m_pMessageVector) {
		for (int i = m_shouldDisplayCount; i < m_indexItemWidgetMap.size(); i++) {
			m_listWidget->item(i)->setHidden(true);
		}
		this->refreshAfterResize();
	}
	QWidget::resizeEvent(event);
}

void LEEditorConsoleListWidget::refreshAfterResize() {
	int itemCount = 0;
	const int startIndex = this->m_customScrollBar->value();
	for (int i = startIndex; itemCount < this->m_shouldDisplayCount && i < this->m_messageIndices.size(); i++) {
		addItemInternal(i - startIndex, this->m_messageIndices[i]);
		itemCount++;
	}
	// If 'itemCount' is smaller than 'm_shouldDisplayCount', add item widgets from back to front from 'startIndex'
	if (itemCount < this->m_shouldDisplayCount) {
		int i = startIndex - 1;
		// Insert Item to row 0 until the 'itemCount' is equal to 'm_shouldDisplayCount' 
		for (; i >= 0 && itemCount < this->m_shouldDisplayCount; i++) {
			this->insertItemInternal(this->m_messageIndices[i]);
			itemCount++;
		}
		this->m_customScrollBar->setValue(i);
	}
	this->m_customScrollBar->setPageStep(this->m_shouldDisplayCount);
	this->m_customScrollBar->setRange(0, this->m_messageIndices.size() - this->m_shouldDisplayCount);
	if (this->m_messageIndices.size() <= this->m_shouldDisplayCount) {
		this->layout()->removeWidget(this->m_customScrollBar);
		this->m_customScrollBar->hide();
	}
	else if (this->m_customScrollBar->isHidden()) {
		this->layout()->addWidget(this->m_customScrollBar);
		this->m_customScrollBar->show();
	}

}

void LEEditorConsoleListWidget::refreshAfterSliderValueChanged() {
	int itemCount = 0;
	int startIndex = this->m_customScrollBar->value();

	for (int i = startIndex; itemCount < this->m_shouldDisplayCount && i < this->m_messageIndices.size(); i++) {
		addItemInternal(i - startIndex, this->m_messageIndices[i]);
		itemCount++;
	}
}

void LEEditorConsoleListWidget::refreshAfterIndexesChanged() {
	this->m_listWidget->clear();
	this->m_indexItemWidgetMap.clear();

	int itemCount = 0;
	this->m_customScrollBar->setValue(0);
	this->m_customScrollBar->setRange(0, this->m_messageIndices.size() - this->m_shouldDisplayCount);
	for (int i = 0; itemCount < this->m_shouldDisplayCount && i < this->m_messageIndices.size(); i++) {
		this->addItemInternal(i, this->m_messageIndices[i]);
		itemCount++;
	}
	if (this->m_messageIndices.size() <= this->m_shouldDisplayCount) {
		this->layout()->removeWidget(this->m_customScrollBar);
		this->m_customScrollBar->hide();
	}
	else if (m_customScrollBar->isHidden()) {
		this->layout()->addWidget(this->m_customScrollBar);
		this->m_customScrollBar->show();
	}
}

void LEEditorConsoleListWidget::refreshAfterPrint(int index) {\
	const int itemCount = this->m_messageIndices.size();
	// show the m_customScrollBar if 'itemCount' is more than 
	this->m_messageIndices.push_back(index);
	this->m_customScrollBar->setRange(0, itemCount - this->m_shouldDisplayCount);
	if (itemCount > this->m_shouldDisplayCount) {
		this->layout()->addWidget(this->m_customScrollBar);
		this->m_customScrollBar->show();
	}
	if (this->m_isAutoRoll) {
		// If the number of output is smaller than m_shouldDisplayCount, then append it directly to the end 
		if (this->m_messageIndices.size() < this->m_shouldDisplayCount) {
			this->addItemInternal(this->m_messageIndices.size() - 1, this->m_messageIndices.back());
		}
		else {
			// startIndex + 1 
			int startIndex = this->m_customScrollBar->value() + 1;
			int itemCount = 0;
			for (int i = startIndex; itemCount < this->m_shouldDisplayCount && i < this->m_messageIndices.size(); i++) {
				addItemInternal(i - startIndex, this->m_messageIndices[i]);
				itemCount++;
			}
			this->m_customScrollBar->setValue(startIndex);
		}
	}
}

void LEEditorConsoleListWidget::setAutoRoll(bool value) {
	this->m_isAutoRoll = value;
}

void LEEditorConsoleListWidget::setMessagePtr(ItemVector* _msgPtr) {
	this->m_pMessageVector = _msgPtr;
}

void LEEditorConsoleListWidget::clear() {
	this->m_pMessageVector = NULL;
	this->m_listWidget->clear();
}

void LEEditorConsoleListWidget::setValidIndexes(const QVector<int>& _indexes) {
	this->m_messageIndices = _indexes;
}
}