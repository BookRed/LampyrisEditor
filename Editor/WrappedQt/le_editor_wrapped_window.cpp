/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_wrapped_window.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_wrapped_window.h"

LEEditorWrapperWindow::LEEditorWrapperWindow():
	m_scrollAreaWidgetContents(new QWidget),
	m_layout(new QGridLayout),
    m_spacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding)),
	m_drawing(false),
	QScrollArea(nullptr) {
	this->setWidget(m_scrollAreaWidgetContents);
	this->setWidgetResizable(true);
	this->m_layout->setContentsMargins(9, 9, 9, 9);
	this->m_layout->setSpacing(0);
	// this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	this->m_scrollAreaWidgetContents->setLayout(m_layout);
	this->installEventFilter(this->m_scrollAreaWidgetContents);
}
