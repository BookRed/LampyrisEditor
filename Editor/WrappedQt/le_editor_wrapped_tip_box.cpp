#pragma once
/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_wrapped_tip_box.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// QT Includes
#include <QLabel>
#include <QStyle>
#include <qmessagebox.h>
// LE Includes
#include "le_editor_wrapped_tip_box.h"

void LEEditorWrappedTipBox::setText(const QString& tipText) {
	QFontMetrics fm(m_tipLabel->font());
	int height = fm.height();
	int space = fm.lineSpacing();
	int lineCount = 1;
	for (int i = 0; i < tipText.size(); i++) {
		if (tipText[i] == '\n')
			lineCount++;
	}
	int totalHeight = (lineCount)*height + (lineCount - 1) * space + 15;
	this->m_tipLabel->setText(tipText);
}

void LEEditorWrappedTipBox::ensureIcon(const TipIcon iconType) {
	QPixmap pixmap;
	switch (iconType) {
	case TipIcon::Information:
		pixmap = style()->standardPixmap(QStyle::StandardPixmap::SP_MessageBoxInformation); break;
	case TipIcon::Warning:
		pixmap = style()->standardPixmap(QStyle::StandardPixmap::SP_MessageBoxWarning); break;
	case TipIcon::Error:
		pixmap = style()->standardPixmap(QStyle::StandardPixmap::SP_MessageBoxCritical); break;
	default:
		break;
	}
	this->m_iconLabel->setPixmap(pixmap);
}

LEEditorWrappedTipBox::LEEditorWrappedTipBox(const QString& tipText,
	const TipIcon icon) :m_layout(new QGridLayout),
	m_tipLabel(new QLabel),
	m_iconLabel(new QLabel) {
	this->ensureIcon(icon);
	this->setText(tipText);
	this->m_layout->addWidget(this->m_iconLabel);
	this->m_layout->addWidget(this->m_tipLabel);
	this->setLayout(this->m_layout);
}
