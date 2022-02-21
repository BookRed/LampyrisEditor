#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_colored_button.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_COLORED_BUTTON_H
#define LE_EDITOR_COLORED_BUTTON_H

// Qt Includes
#include <QToolButton>
// GLM Includes
#include <glm/glm.hpp>

// Used to select color or texture
class LEEditorColoredButton :public QToolButton {
	Q_OBJECT
protected:
	bool m_isMouseEntered = false;

	QColor m_displayColor;

	void      paintEvent(QPaintEvent* e);
		      
	void      mousePressEvent(QMouseEvent* e);
		      
	void      focusInEvent(QFocusEvent* e);
		      
	void      focusOutEvent(QFocusEvent* e);
		      
	void      enterEvent(QEvent* e);
		      
	void      leaveEvent(QEvent* e);
Q_SIGNALS:
	void      dispatchEvent();
public:
	          LEEditorColoredButton(QWidget* parent = nullptr);

	void      setColor(glm::vec4 color) {
		this->m_displayColor.setRedF(color.x);
		this->m_displayColor.setGreenF(color.y);
		this->m_displayColor.setBlueF(color.z);
		this->m_displayColor.setAlphaF(color.a);
	}

	glm::vec4 getColor() const {
		return { this->m_displayColor.redF(),
			     this->m_displayColor.greenF(),
			     this->m_displayColor.blueF(),
			     this->m_displayColor.alphaF() };
	}
};
#endif // !LE_EDITOR_COLORED_BUTTON_H
