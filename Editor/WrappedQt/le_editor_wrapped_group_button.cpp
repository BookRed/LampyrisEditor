/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_wrapped_group_button.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_wrapped_group_button.h"

void LEEditorWrappedGroupButtonInternal::paintEvent(QPaintEvent* e) {
	QPainter painter(this);

	int buttonCount = this->m_icons.size();

	painter.save();
	painter.setBrush(QColor(Qt::gray));

	if (this->m_mode == IconOnly) {
		// draw bounding rect
		painter.setPen(QPen(Qt::black, 2));
		painter.drawRoundedRect(rect(), sc_bounding_rect_radius, sc_bounding_rect_radius);

		// draw split line
		for (int i = 0; i < buttonCount - 1; i++) {
			painter.drawLine((i + 1) * sc_icon_width, 0, (i + 1) * sc_icon_width, sc_icon_height);
		}

		painter.setBrush(QColor(248, 128, 0));

		for (int i = 0; i < buttonCount; i++) {
			if ((this->m_hoveredMask & (1 << (i))) != 0) {
				painter.drawRect(QRect(i * sc_icon_width, 0, sc_icon_width, sc_icon_height));
				break;
			}
		}
		painter.restore();
		// draw icon
		for (int i = 0; i < buttonCount; i++) {
			painter.drawPixmap(QRect(i * sc_icon_width + 4, 4, sc_icon_width - 8, sc_icon_height - 8), this->m_icons[i]);
		}
	}
	else {
		// draw bounding rect
		painter.setPen(QPen(Qt::black, 2));
		painter.drawRoundedRect(rect(), sc_bounding_rect_radius, sc_bounding_rect_radius);

		// draw split line
		for (int i = 0; i < buttonCount - 1; i++) {
			painter.drawLine((i + 1) * (this->m_maxTextWidth + 30), 0, (i + 1) * (this->m_maxTextWidth + 30), this->m_fontHeight + 8);
		}

		// draw text
		QFontMetrics fm(this->font());

		painter.save();

		for (int i = 0; i < buttonCount; i++) {
			bool hovered = (this->m_hoveredMask & (1 << (i))) != 0;
			bool selected = (this->m_hoveredMask & (1 << (i + sc_max_button_count))) != 0;

			if (hovered) {
				painter.setBrush(QColor(244, 164, 96));
				painter.drawRect(QRect(i * (this->m_maxTextWidth + 30), 0, this->m_maxTextWidth + 30, this->m_fontHeight + 8));
			}
			if (selected) {
				painter.setBrush(QColor(248, 128, 0));
				painter.drawRect(QRect(i * (this->m_maxTextWidth + 30), 0, this->m_maxTextWidth + 30, this->m_fontHeight + 8));
			}
		}
		for (int i = 0; i < buttonCount; i++) {
			int width = fm.width(this->m_infos[i]);
			bool hoveredOrSelected = ((this->m_hoveredMask & (1 << (i))) != 0) || ((this->m_hoveredMask & (1 << (i + 16))) != 0);
			if (hoveredOrSelected) {
				painter.save();
				painter.setPen(Qt::white);
			}
			if (this->m_mode == DisplayMode::TextOnly) {
				painter.drawText(QRect(i * (this->m_maxTextWidth + 30) + 15 + (m_maxTextWidth - width) / 2, 4, this->m_maxTextWidth + 3, this->m_fontHeight), this->m_infos[i]);
			}
			else {
				painter.drawText(QRect(i * (this->m_maxTextWidth + 30) + 15 + (m_maxTextWidth - width) / 2 + 10, 4, this->m_maxTextWidth + 3, this->m_fontHeight), this->m_infos[i]);
			}
			if (hoveredOrSelected) {
				painter.restore();
			}
		}
		// draw small icon
		if (this->m_mode == DisplayMode::Both) {
			for (int i = 0; i < buttonCount; i++) {
				painter.drawPixmap(QRect(i * (this->m_maxTextWidth + 30) + 4, 2, 16, 16), this->m_icons[i]);
			}
		}
		painter.restore();
	}
	painter.restore();
}

void LEEditorWrappedGroupButtonInternal::mouseMoveEvent(QMouseEvent* e) {
	int buttonCount = this->m_icons.size();
	this->m_hoveredMask &= 0xffff0000;
	for (int i = 0; i < buttonCount; i++) {
		bool hovered = this->m_mode == DisplayMode::IconOnly ?
			               QRect(i * sc_icon_width, 0, sc_icon_width, sc_icon_height).contains(e->pos()) :
			               QRect(i * (this->m_maxTextWidth + 30), 0, this->m_maxTextWidth + 30, this->m_fontHeight + 8).contains(e->pos());
		if (hovered) {
			this->m_hoveredMask |= (1 << i);
			this->update();
			break;
		}
	}
	((QWidget*)this->parent())->setWindowTitle(QString("%1").arg(m_hoveredMask));
}

void LEEditorWrappedGroupButtonInternal::mousePressEvent(QMouseEvent* e) {
	int buttonCount = this->m_icons.size();

	for (int i = 0; i < buttonCount; i++) {
		if ((this->m_hoveredMask & (1 << i)) != 0) {
			this->m_hoveredMask &= 0x0000ffff;
			this->m_hoveredMask |= (1 << (sc_max_button_count + i));
			this->update();

			emit this->signalSelectedIndexChanged(i);
		}
	}
}

void LEEditorWrappedGroupButtonInternal::leaveEvent(QEvent* e) {
	this->m_hoveredMask &= 0xffff0000;
	this->update();
}

LEEditorWrappedGroupButtonInternal::LEEditorWrappedGroupButtonInternal(DisplayMode mode, const QVector<QPixmap>& icons,
	const QVector<QString>& infos):
	m_mode(mode),
	m_icons(icons),
	m_infos(infos), m_hoveredMask(0) {
	if (this->m_mode == IconOnly) {
		this->setFixedHeight(sc_icon_height);
		this->setFixedWidth(sc_icon_width * icons.size());
	}
	else {
		QFontMetrics fm(this->font());
		this->m_maxTextWidth = INT_MIN;
		for (int i = 0; i < this->m_infos.size(); i++) {
			int width = fm.width(this->m_infos[i]);
			this->m_maxTextWidth = std::max(this->m_maxTextWidth, width);
		}
		this->setFixedHeight(fm.height() + 8);
		this->setFixedWidth(this->m_infos.size() * (30 + this->m_maxTextWidth));
		this->m_fontHeight = fm.height();
	}
	this->setMouseTracking(true);
}

LEEditorWrappedGroupButton::LEEditorWrappedGroupButton(const QVector<QPixmap>& icons, const QVector<QString>& infos):
	m_button(new LEEditorWrappedGroupButtonInternal(LEEditorWrappedGroupButtonInternal::Both, icons, infos)),
	m_layout(new QHBoxLayout) {
	this->setLayout(this->m_layout);
	this->m_layout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));
	this->m_layout->addWidget(this->m_button);
	this->m_layout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));
	this->setMouseTracking(true);
	this->m_button->setParent(this);

	QObject::connect(this->m_button, &LEEditorWrappedGroupButtonInternal::signalSelectedIndexChanged, [=](int i) {
		emit this->signalSelectedIndexChanged(i);
	});
}
