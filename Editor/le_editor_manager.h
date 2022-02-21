#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_manager.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "Runtime/Base/le_singleton.h"

class IEntityTreeWidget;
class IPropertyEditorWidget;
class IAssetExplorerWidget;
class ISandBoxWidget;
class IDisplayWidget;

class LampyrisEditorManager:public Singleton<LampyrisEditorManager> {
private:
	IEntityTreeWidget*      m_pEntityTreeWidget;
	IPropertyEditorWidget*  m_pPropertyEditorWidget;
	IAssetExplorerWidget*   m_pAssetExplorerWidget;
	ISandBoxWidget*         m_pSandBoxWidget;
	IDisplayWidget*         m_pDisplayWidget;
public:
	                        LampyrisEditorManager();

	virtual                ~LampyrisEditorManager();

	void                    initEditorWidgets();

	void                    destoryEditorWidgets();

	IEntityTreeWidget&      getEntityTreeWidget() const;

	IPropertyEditorWidget&  getPropertyEditorWidget() const;

	IAssetExplorerWidget&   getAssetExplorerWidget() const;

	ISandBoxWidget&         getSandBoxWidget() const;

	IDisplayWidget&         getDisplayWidget() const;

	friend class LELampyrisEditorMainWindow;
};
