/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_vector_field.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_vector_field.h"

void LEEditorVector2DField::ensureLayout() {
	this->m_layout->setContentsMargins(0, 3, 0, 3);
	this->m_layout->addWidget(this->m_labelX);
	this->m_layout->addWidget(this->m_editX);
	this->m_layout->addWidget(this->m_labelY);
	this->m_layout->addWidget(this->m_editY);
	this->m_layout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
	this->m_container->setLayout(this->m_layout);
}

void LEEditorVector2DField::setControlProperty() {
	this->m_editX->setValidator(new QDoubleValidator);
	this->m_editY->setValidator(new QDoubleValidator);
}

void LEEditorVector2DField::connectSignal() {
	auto function = [=]() {emit signalEventDispatch(this); };
	QObject::connect(this->m_editX, &QLineEdit::editingFinished, function);
}

LEEditorVector2DField::LEEditorVector2DField() {

}

LEEditorVector2DField::~LEEditorVector2DField() {

}

void LEEditorVector2DField::setValue(float x, float y) {
	this->m_editX->setText(QString::number(x, 'g', 3));
	this->m_editY->setText(QString::number(y, 'g', 3));
}

float LEEditorVector2DField::getX() { return this->m_editX->text().toFloat(); }

float LEEditorVector2DField::getY() { return this->m_editY->text().toFloat(); }

glm::vec2 LEEditorVector2DField::get() {
	return { getX(),getY() };
}

LEEditorVector2DField::LEEditorVector2DField(const QString& title, float x, float y) :
	m_labelX(new QLabel("X")),
	m_labelY(new QLabel("Y")),
	m_editX(new LEEditorLineEdit("0")),
	m_editY(new LEEditorLineEdit("0")),
	m_container(new QWidget),
	m_layout(new QHBoxLayout) {
	this->setValue(x, y);
	this->ensure(title, this->m_container);
	this->ensureLayout();
	this->setControlProperty();
	QObject::connect(this->m_editX, &LEEditorLineEdit::editingFinished, [=]() { emit signalEventDispatch(this); });
	QObject::connect(this->m_editY, &LEEditorLineEdit::editingFinished, [=]() { emit signalEventDispatch(this); });
}

void LEEditorVector3DField::ensureLayout() {
	this->m_layout->setContentsMargins(0, 3, 0, 3);
	this->m_layout->addWidget(this->m_labelX);
	this->m_layout->addWidget(this->m_editX);
	this->m_layout->addWidget(this->m_labelY);
	this->m_layout->addWidget(this->m_editY);
	this->m_layout->addWidget(this->m_labelZ);
	this->m_layout->addWidget(this->m_editZ);
	this->m_container->setLayout(this->m_layout);
}

void LEEditorVector3DField::setControlProperty() {
	this->m_editX->setValidator(new QDoubleValidator);
	this->m_editY->setValidator(new QDoubleValidator);
	this->m_editZ->setValidator(new QDoubleValidator);
}

void LEEditorVector3DField::connectSignal() {
	auto function = [=]() {emit signalEventDispatch(this); };
	QObject::connect(this->m_editX, &QLineEdit::editingFinished, function);
}

void LEEditorVector3DField::setValue(float x, float y, float z) {
	this->m_editX->setText(QString::number(x, 'g', 3));
	this->m_editY->setText(QString::number(y, 'g', 3));
	this->m_editZ->setText(QString::number(z, 'g', 3));
}

float LEEditorVector3DField::getX() { return this->m_editX->text().toFloat(); }

float LEEditorVector3DField::getY() { return this->m_editY->text().toFloat(); }

float LEEditorVector3DField::getZ() { return this->m_editZ->text().toFloat(); }

glm::vec3 LEEditorVector3DField::get() {
	return { getX(),getY(),getZ()};
}

LEEditorVector3DField::LEEditorVector3DField(const QString& title, float x, float y, float z) :
	m_labelX(new QLabel("X")),
	m_labelY(new QLabel("Y")),
	m_labelZ(new QLabel("Z")),
	m_editX(new LEEditorLineEdit("0")),
	m_editY(new LEEditorLineEdit("0")),
	m_editZ(new LEEditorLineEdit("0")),
	m_container(new QWidget),
	m_layout(new QHBoxLayout) {
	this->setValue(x, y, z);
	this->ensure(title, this->m_container);
	this->ensureLayout();
	this->setControlProperty();
	QObject::connect(this->m_editX, &LEEditorLineEdit::editingFinished, [=]() { emit signalEventDispatch(this); });
	QObject::connect(this->m_editY, &LEEditorLineEdit::editingFinished, [=]() { emit signalEventDispatch(this); });
	QObject::connect(this->m_editZ, &LEEditorLineEdit::editingFinished, [=]() { emit signalEventDispatch(this); });
}

void LEEditorVector4DField::ensureLayout() {
	//              v-layout
	// h-layout: | x | edit | y | edit |
	// h-layout: | z | edit | w | edit |
	this->m_container_main->setLayout(this->m_vlayout);
	this->m_container_top->setLayout(this->m_hlayout_top);
	this->m_container_bottom->setLayout(this->m_hlayout_bottom);

	this->m_vlayout->setContentsMargins(0, 0, 0, 0);
	this->m_vlayout->setSpacing(0);
	this->m_container_top->setContentsMargins(0, 0, 0, 0);
	this->m_container_bottom->setContentsMargins(0, 0, 0, 0);

	this->m_vlayout->addWidget(this->m_container_top);
	this->m_vlayout->addWidget(this->m_container_bottom);

	this->m_hlayout_top->addWidget(this->m_labelX);
	this->m_hlayout_top->addWidget(this->m_editX);
	this->m_hlayout_top->addWidget(this->m_labelY);
	this->m_hlayout_top->addWidget(this->m_editY);
	this->m_hlayout_bottom->addWidget(this->m_labelZ);
	this->m_hlayout_bottom->addWidget(this->m_editZ);
	this->m_hlayout_bottom->addWidget(this->m_labelW);
	this->m_hlayout_bottom->addWidget(this->m_editW);
}

void LEEditorVector4DField::setControlProperty() {
	this->m_editX->setValidator(new QDoubleValidator);
	this->m_editY->setValidator(new QDoubleValidator);
	this->m_editZ->setValidator(new QDoubleValidator);
	this->m_editW->setValidator(new QDoubleValidator);
}

void LEEditorVector4DField::connectSignal() {
	auto function = [=]() {emit signalEventDispatch(this); };
	QObject::connect(this->m_editX, &QLineEdit::editingFinished, function);
}

void LEEditorVector4DField::setValue(float x, float y, float z, float w) {
	this->m_editX->setText(QString::number(x, 'g', 3));
	this->m_editY->setText(QString::number(y, 'g', 3));
	this->m_editZ->setText(QString::number(z, 'g', 3));
	this->m_editW->setText(QString::number(w, 'g', 3));
}

float LEEditorVector4DField::getX() { return this->m_editX->text().toFloat(); }

float LEEditorVector4DField::getY() { return this->m_editY->text().toFloat(); }

float LEEditorVector4DField::getZ() { return this->m_editZ->text().toFloat(); }

float LEEditorVector4DField::getW() { return this->m_editW->text().toFloat(); }

glm::vec4 LEEditorVector4DField::get() {
	return { getX(),getY(),getZ(),getW()};
}

LEEditorVector4DField::LEEditorVector4DField(const QString& title, float x, float y, float z, float w) :
	m_labelX(new QLabel("X")),
	m_labelY(new QLabel("Y")),
	m_labelZ(new QLabel("Z")),
	m_labelW(new QLabel("W")),
	m_editX(new LEEditorLineEdit("0")),
	m_editY(new LEEditorLineEdit("0")),
	m_editZ(new LEEditorLineEdit("0")),
	m_editW(new LEEditorLineEdit("0")),
	m_vlayout(new QVBoxLayout),
	m_hlayout_top(new QHBoxLayout),
	m_hlayout_bottom(new QHBoxLayout),
	m_container_main(new QWidget),
	m_container_top(new QWidget),
	m_container_bottom(new QWidget) {
	this->setValue(x, y, z, w);
	this->ensure(title, this->m_container_main);
	this->ensureLayout();
	this->setControlProperty();
	QObject::connect(this->m_editX, &LEEditorLineEdit::editingFinished, [=]() { emit signalEventDispatch(this); });
	QObject::connect(this->m_editY, &LEEditorLineEdit::editingFinished, [=]() { emit signalEventDispatch(this); });
	QObject::connect(this->m_editZ, &LEEditorLineEdit::editingFinished, [=]() { emit signalEventDispatch(this); });
	QObject::connect(this->m_editW, &LEEditorLineEdit::editingFinished, [=]() { emit signalEventDispatch(this); });
}
