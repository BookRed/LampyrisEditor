/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_launcher_project_template_button.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_launcher_project_template_button.h"

void LELauncherProjectTemplateButton::onButtonClicked(bool isChecked) {
	// reverse the checked state
	this->m_checkBox->setChecked(!isChecked);
	emit buttonClickSignal(this->m_type, !isChecked);
	this->update();
}

void LELauncherProjectTemplateButton::onCheckBoxClicked(bool isChecked) {
	emit buttonClickSignal(this->m_type, !isChecked);
	this->update();
}

void LELauncherProjectTemplateButton::mousePressEvent(QMouseEvent* event) {
	this->onButtonClicked(this->m_checkBox->isChecked());
	QWidget::mousePressEvent(event);
}

void LELauncherProjectTemplateButton::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	QRect _rect = this->rect();
	painter.setPen(QColor(0, 0, 0));
	QPen pen;
	if (this->m_checkBox->isChecked()) {
		pen = QPen(QColor(255, 170, 0), 2);
	} else {
		if (!m_isMouseEntered) {
			pen = QPen((QColor(255, 255, 255), 2));
		}
		else {
			pen = QPen(QColor(255, 170, 0,127), 2);
		}
	}
	painter.setPen(pen);
	painter.drawPixmap(2, 2, *this->m_pixmap);
	painter.drawRect(QRect(_rect.x() + 3, _rect.y() + 3, _rect.width() - 6, _rect.height() - 6));
	QWidget::paintEvent(event);
}

void LELauncherProjectTemplateButton::enterEvent(QEvent* event) {
	this->m_isMouseEntered = true;
}

void LELauncherProjectTemplateButton::leaveEvent(QEvent* event) {
	this->m_isMouseEntered = false;
}

void LELauncherProjectTemplateButton::setProjectTemplateContext(QPixmap* pixmap, LEProjectTemplateType type) {
	this->m_pixmap = pixmap;
	this->m_type   = type;
}

LELauncherProjectTemplateButton::LELauncherProjectTemplateButton(QWidget* parent):
	m_isMouseEntered(false), m_checkBox(new QCheckBox(this)) {
	this->m_checkBox->setGeometry(95, 85,
	                              this->m_checkBox->width(), this->m_checkBox->height());
	this->m_checkBox->show();
	QObject::connect(this->m_checkBox, &QCheckBox::toggled, 
	                 this, &LELauncherProjectTemplateButton::onCheckBoxClicked);
}

LELauncherProjectTemplateButton::~LELauncherProjectTemplateButton() {
		
}

void LELauncherProjectTemplateButton::cancelSelection() {
	this->m_checkBox->setChecked(false);
	this->update();
}

bool LELauncherProjectTemplateButton::isSelected() const {
	return this->m_checkBox->isChecked();
}
