/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_wrapped_object_field.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_wrapped_object_field.h"
// QT Includes
#include <QPainter>
#include <QMouseEvent>

void LEEditorWrappedObjectWidget::enterEvent(QEvent* e) {
	this->m_enter = true;
}

void LEEditorWrappedObjectWidget::leaveEvent(QEvent* e) {
	this->m_enter = false;
}

void LEEditorWrappedObjectWidget::mousePressEvent(QMouseEvent* e) {
	if (e->button() == Qt::MouseButton::LeftButton) {
		if (this->m_removeButtonHovered) {
			emit this->signalremoveButtonClicked();
		}

		if (this->m_selectButtonHovered) {
			emit this->signalselectButtonClicked();
		}
	}
}

void LEEditorWrappedObjectWidget::mouseMoveEvent(QMouseEvent* e) {
	QPoint mousePos = e->pos();

	if (this->m_removeButtonRect.contains(mousePos)) {
		this->m_removeButtonHovered = true;
		this->m_selectButtonHovered = false;
		this->setCursor(Qt::CursorShape::PointingHandCursor);
	}
	else if (this->m_selectButtonRect.contains(mousePos)) {
		this->m_removeButtonHovered = false;
		this->m_selectButtonHovered = true;
		this->setCursor(Qt::CursorShape::PointingHandCursor);
	}
	else {
		this->m_removeButtonHovered = false;
		this->m_selectButtonHovered = false;
		this->setCursor(Qt::CursorShape::ArrowCursor);
	}
	this->update();
}

void LEEditorWrappedObjectWidget::paintEvent(QPaintEvent* e) {
	QPainter painter(this);
	// set the background color 
	painter.save();
	painter.setBrush(Qt::gray);

	// set the border color according to the hover status
	if (true) {//!this->m_enter) {
		QPen pen(QColor(0, 0, 0), 2);
		painter.setPen(pen);
	}
	else {
		QPen pen(QColor(255, 170, 0), 2);
		//painter.setPen(pen);
	}

	QFont thisFont = this->font();
	thisFont.setPixelSize(10);
	painter.setFont(thisFont);

	QFontMetrics fm(thisFont);
	this->objectTypeNameWidth = fm.width(this->m_objectTypeName);
	this->objectTypeNameHeight = fm.height();

	// draw bounding rect
	static const int sc_top_bounding_rect_left_right_margin = 12;
	static const int sc_top_bounding_rect_height = 40;
	static const int sc_bounding_rect_radius = 3;
	static const int sc_bottom_bounding_rect_offset_y = 5;

	QRect topBoundingRect = QRect(0,
		0,
		objectTypeNameWidth + 2 * sc_top_bounding_rect_left_right_margin,
		40);

	QRect bottomBoundingRect = QRect(0,
		objectTypeNameHeight + sc_bottom_bounding_rect_offset_y,
		this->width(),
		sc_top_bounding_rect_height - objectTypeNameHeight - sc_bottom_bounding_rect_offset_y);

	painter.drawRoundedRect(topBoundingRect,
		sc_bounding_rect_radius,
		sc_bounding_rect_radius);


	painter.drawRoundedRect(bottomBoundingRect,
		sc_bounding_rect_radius,
		sc_bounding_rect_radius);
	painter.restore();

	// draw object type name
	painter.setFont(thisFont);
	painter.drawText(12, 12, this->m_objectTypeName);

	// draw object name
	thisFont.setPixelSize(13);
	painter.setFont(thisFont);
	painter.save();

	painter.setPen(QColor(248, 128, 0));
	painter.setBrush(QColor(248, 128, 0));

	static const int sc_icon_size_normal = 16;
	static const int sc_icon_size_hovered = 20;
	static const int sc_icon_margin = 4;
	static const int sc_background_rect_left_margin = 1;
	static const int sc_background_rect_margin_y = 7;
	static const int sc_background_rect_offset_y = -4;

	// draw the background of object name
	QRect backgroundRect = QRect(sc_background_rect_left_margin,
		objectTypeNameHeight + sc_background_rect_margin_y,
		this->width() - 5 * sc_icon_margin - 2 * sc_icon_size_normal,
		sc_top_bounding_rect_height - objectTypeNameHeight - sc_bottom_bounding_rect_offset_y + sc_background_rect_offset_y);

	painter.drawRect(backgroundRect);
	painter.restore();
	// draw object name
	painter.drawText(12, objectTypeNameHeight + 22, this->m_objectName);

	// draw split line between background and remove button
	static const int sc_split_line_offset_y = 5;

	painter.setPen(QPen(Qt::black, 2));

	painter.drawLine(this->width() - 4 * sc_icon_margin - 2 * sc_icon_size_normal,
		objectTypeNameHeight + sc_split_line_offset_y,
		this->width() - 4 * sc_icon_margin - 2 * sc_icon_size_normal,
		sc_top_bounding_rect_height);

	static const int sc_button_margin = 2;
	if (this->m_removeButtonHovered) {
		this->m_removeButtonRect = QRect(this->width() - 3 * sc_icon_margin - 2 * sc_icon_size_normal - sc_button_margin,
			objectTypeNameHeight + 2 * sc_icon_margin,
			sc_icon_size_hovered,
			sc_icon_size_hovered);
	}
	else {
		this->m_removeButtonRect = QRect(this->width() - 3 * sc_icon_margin - 2 * sc_icon_size_normal,
			objectTypeNameHeight + 2 * sc_icon_margin + (sc_icon_size_hovered - sc_icon_size_normal) / 2,
			sc_icon_size_normal,
			sc_icon_size_normal);
	}
	if (this->m_selectButtonHovered) {
		this->m_selectButtonRect = QRect(this->width() - sc_icon_margin - sc_icon_size_normal - sc_button_margin,
			objectTypeNameHeight + 2 * sc_icon_margin,
			sc_icon_size_hovered,
			sc_icon_size_hovered);
	}
	else {
		this->m_selectButtonRect = QRect(this->width() - sc_icon_margin - sc_icon_size_normal,
			objectTypeNameHeight + 2 * sc_icon_margin + (sc_icon_size_hovered - sc_icon_size_normal) / 2,
			sc_icon_size_normal,
			sc_icon_size_normal);
	}
	painter.drawPixmap(this->m_removeButtonRect, QPixmap("D:\\搜狗高速下载\\ic24-remove.png"));

	// draw split line between remove and select button
	painter.drawLine(this->width() - 2 * sc_icon_margin - sc_icon_size_normal,
		objectTypeNameHeight + sc_split_line_offset_y,
		this->width() - 2 * sc_icon_margin - sc_icon_size_normal,
		sc_top_bounding_rect_height);

	painter.drawPixmap(this->m_selectButtonRect, QPixmap("D:\\搜狗高速下载\\select.png"));

}

LEEditorWrappedObjectField::LEEditorWrappedObjectField(const QString& title) :m_widget(new LEEditorWrappedObjectWidget) {
	this->setMinimumHeight(60);
	this->ensure(title, this->m_widget);

	QObject::connect(this->m_widget, &LEEditorWrappedObjectWidget::signalremoveButtonClicked, [=]() {
		QMessageBox::information(nullptr, "Message", "remove button clicked");
		});
	QObject::connect(this->m_widget, &LEEditorWrappedObjectWidget::signalselectButtonClicked, [=]() {
		QMessageBox::information(nullptr, "Message", "select button clicked");
		});
}
