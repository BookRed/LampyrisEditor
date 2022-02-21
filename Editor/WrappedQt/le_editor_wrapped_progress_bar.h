#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_wrapped_progress_bar.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_WRAPPED_PROGRESS_BAR_H
#define LE_EDITOR_WRAPPED_PROGRESS_BAR_H

// QT Includes
#include <QProgressBar>
// LE Includes
#include "le_editor_property_field.h"

class LEEditorWrappedProgressBar :public LEEditorPropertyField {
	Q_OBJECT
private:
	QProgressBar* m_progressBar;
public:
	              LEEditorWrappedProgressBar(const QString& title, 
		                                     const int value);

	inline void   setValue(const int value);
};
#endif // !LE_EDITOR_WRAPPED_PROGRESS_BAR_H
