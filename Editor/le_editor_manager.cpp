#include "le_editor_manager.h"

LampyrisEditorManager::LampyrisEditorManager(): m_pEntityTreeWidget(nullptr),
                                                m_pPropertyEditorWidget(nullptr),
                                                m_pAssetExplorerWidget(nullptr),
                                                m_pSandBoxWidget(nullptr),
                                                m_pDisplayWidget(nullptr) {}

LampyrisEditorManager::~LampyrisEditorManager() {
		
}

void LampyrisEditorManager::initEditorWidgets() {
		
}

void LampyrisEditorManager::destoryEditorWidgets() {
}

IEntityTreeWidget& LampyrisEditorManager::getEntityTreeWidget() const {
	return *this->m_pEntityTreeWidget;
}

IPropertyEditorWidget& LampyrisEditorManager::getPropertyEditorWidget() const {
	return *this->m_pPropertyEditorWidget;
}

IAssetExplorerWidget& LampyrisEditorManager::getAssetExplorerWidget() const {
	return *this->m_pAssetExplorerWidget;
}

ISandBoxWidget& LampyrisEditorManager::getSandBoxWidget() const {
	return *this->m_pSandBoxWidget;
}

IDisplayWidget& LampyrisEditorManager::getDisplayWidget() const {
	return *this->m_pDisplayWidget;
}
