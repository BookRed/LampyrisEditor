/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_colored_button.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_colored_button.h"
// QT Includes
#include <QPainter>
#include <QColorDialog>

void LEEditorColoredButton::paintEvent(QPaintEvent* e) {
	QPainter painter(this);
	QRect _rect = this->rect();
	painter.setPen(QColor(0, 0, 0));
	if (!m_isMouseEntered) {
		QPen pen(QColor(0, 0, 0), 2);
		painter.setPen(pen);
	}
	else {
		QPen pen(QColor(255, 170, 0), 2);
		painter.setPen(pen);
	}
	painter.setBrush(m_displayColor);
	painter.drawRect(QRect(_rect.x() + 3, _rect.y() + 3, _rect.width() - 6, _rect.height() - 6));
	painter.setPen(QColor(255, 255, 255));
	painter.setBrush(Qt::GlobalColor::white);
	painter.drawRect(QRect(_rect.x() + 16, _rect.y() + 65, _rect.width() - 21, 10));
	painter.setPen(Qt::black);

	auto f = painter.font();
	f.setPixelSize(10);
	painter.setFont(f);
	painter.drawText(17, 74, QStringLiteral("Select Color"));
}

void LEEditorColoredButton::mousePressEvent(QMouseEvent* e) {
	m_displayColor = QColorDialog::getColor(Qt::white, NULL, "pick Color", QColorDialog::ShowAlphaChannel);
	emit dispatchEvent();
}

void LEEditorColoredButton::focusInEvent(QFocusEvent* e) {
	QToolButton::focusInEvent(e);
}

void LEEditorColoredButton::focusOutEvent(QFocusEvent* e) {
	QToolButton::focusOutEvent(e);
}

void LEEditorColoredButton::enterEvent(QEvent* e) {
	m_isMouseEntered = true;
}

void LEEditorColoredButton::leaveEvent(QEvent* e) {
	m_isMouseEntered = false;
}

LEEditorColoredButton::LEEditorColoredButton(QWidget* parent) :
	QToolButton(parent) {
	this->setFixedSize(80, 80);
	QObject::connect(this, &QToolButton::clicked, [=]() {
		QColorDialog::getColor();
		});
	this->setMouseTracking(true);
}
