/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_wrapped_progress_bar.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_wrapped_progress_bar.h"

LEEditorWrappedProgressBar::LEEditorWrappedProgressBar(const QString& title, const int value):
	m_progressBar(new QProgressBar) {
	this->setValue(value);
	this->ensure(title, this->m_progressBar);
}

void LEEditorWrappedProgressBar::setValue(const int value) { this->m_progressBar->setValue(value); }
