#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_wrapped_component_section.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_WRAPPED_COMPONENT_SECTION_H
#define LE_EDITOR_WRAPPED_COMPONENT_SECTION_H

// QT Includes
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QCheckBox>

class LEEditorWrappedComponentSection:public QWidget {
	Q_OBJECT
private:
	static QPixmap*  ms_iconExpand;
	static QPixmap*  ms_iconCollapse;
	QWidget*         m_container;
	bool             m_isOpened;
	QHBoxLayout*     m_layout;
	QHBoxLayout*     m_containerlayout;
	QCheckBox*       m_checkBox;
	QLabel*          m_componentNameLabel;
	QLabel*          m_iconLabel;
				    
	void             createLayout();
				    
	void             connectSignal();
protected:		    
	void             mousePressEvent(QMouseEvent* event) override;
public:			    
	                 LEEditorWrappedComponentSection(const QString& componentName,bool canSetEnabled = true);
				    
	virtual          ~LEEditorWrappedComponentSection();
				    
	bool             isOpened() const;
Q_SIGNALS:		    
	void             signalEventDispatch(QWidget* object);
};
#endif // !LE_EDITOR_WRAPPED_COMPONENT_SECTION_H