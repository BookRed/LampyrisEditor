/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_wrapped_slider.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_wrapped_slider.h"

LEEditorWrappedSlider::LEEditorWrappedSlider(const QString& title, const int min, const int max, const int step, QWidget* parent):
	m_slider(new QSlider(this)),
	m_edit(new LEEditorWrappedLineEdit(QString(), 0)),
	m_layout(new QHBoxLayout(this)),
	m_widget(new QWidget(this)) {
	this->m_widget->setLayout(this->m_layout);
	this->m_layout->addWidget(this->m_slider, 8);

	this->m_layout->addWidget(this->m_edit, 2);
	this->m_layout->setContentsMargins(0, 0, 0, 0);
	this->m_layout->setSpacing(0);
	QObject::connect(this->m_slider, &QSlider::valueChanged, [=]() {
		this->m_edit->setText(QString::number(this->m_slider->value()));
		emit signalEventDispatch(this);
		});
	QObject::connect(this->m_edit, &LEEditorWrappedLineEdit::signalEventDispatch, [=]() {
		this->m_slider->setValue(this->m_edit->text().toInt());
		emit signalEventDispatch(this);
		});
	this->m_slider->setOrientation(Qt::Orientation::Horizontal);
	this->setMin(min);
	this->setMax(max);
	this->setStep(step);
	this->ensure(title, m_widget);
}
