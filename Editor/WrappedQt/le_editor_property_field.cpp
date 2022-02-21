/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_property_field.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_property_field.h"
// QT Includes
#include <QHBoxLayout>

LEEditorPropertyField::LEEditorPropertyField() {
	this->m_titleLabel = new QLabel(this);
}

LEEditorPropertyField::~LEEditorPropertyField() {
}

void LEEditorPropertyField::ensure(const QString& title, QWidget* control, QWidget* parent) {
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 3, 0, 3);
	layout->addWidget(this->m_titleLabel, 3);
	layout->addWidget(control, 7);
	this->setLayout(layout);
	this->m_titleLabel->setText(title);
	this->m_title = title;

}

void LEEditorPropertyField::setTitle(const QString& title) {
	this->m_titleLabel->setText(title);
	this->m_title = title;
}
