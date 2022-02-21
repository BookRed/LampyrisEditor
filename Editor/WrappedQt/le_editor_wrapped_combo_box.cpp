/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_wrapped_combo_box.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_wrapped_combo_box.h"

LEEditorWrappedComboBox::LEEditorWrappedComboBox(const QString& title, const std::initializer_list<QString>& items) :
	m_comboBox(new QComboBox(this)) {
	this->m_comboBox->addItems(items);
	QObject::connect(this->m_comboBox, (void (QComboBox::*)(int)) & QComboBox::currentIndexChanged, [=](int index) {
		emit signalEventDispatch(this);
		});
	this->ensure(title, m_comboBox);
}

int LEEditorWrappedComboBox::currentIndex() {
	return this->m_comboBox->currentIndex();
}

void LEEditorWrappedComboBox::setCurrentIndex(const int index) {
	this->m_comboBox->setCurrentIndex(index);
}

QString LEEditorWrappedComboBox::itemText(const int index) {
	return this->m_comboBox->itemText(index);
}

void LEEditorWrappedComboBox::clear() {
}
