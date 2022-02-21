#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_wrapped_tip_box.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_WRAPPED_TIP_BOX_H
#define LE_EDITOR_WRAPPED_TIP_BOX_H
// QT Includes
#include <QHBoxLayout>
#include <QGroupBox>
// LE Includes
#include "le_editor_property_field.h"

class LEEditorWrappedTipBox :public QGroupBox {
	Q_OBJECT
public:
	enum TipIcon {
		None,
		Error,
		Warning,
		Information
	};
private:
	QLabel*           m_tipLabel;
	QLabel*           m_iconLabel;
	QGridLayout*      m_layout;
	void              ensureIcon(const TipIcon iconType);
public:
	                  LEEditorWrappedTipBox(const QString& tipText,
	                  	                    const TipIcon  icon = TipIcon::Information);

	void              setText(const QString& tipText);
};
#endif // !LE_EDITOR_WRAPPED_TIP_BOX_H