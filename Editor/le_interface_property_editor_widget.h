#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_interface_property_editor_widget.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

class LEEntity;

class IPropertyEditorWidget {
public:
	virtual void setEntity(LEEntity* pEntity) = 0;

	virtual void setIdle() = 0;

	virtual void refresh() = 0;

};