/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_property_editor_widget.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_property_editor_widget.h"
#include "WrappedQt/le_editor_gui_layout.h"
#include "Runtime/Base/le_entity.h"
#include "Runtime/Math/le_transform.h"

LEEditorPropertyEditorWidget::LEEditorPropertyEditorWidget():m_pEntity(nullptr) {
	this->onGUI();
}

void LEEditorPropertyEditorWidget::onGUI() {
	LEEditorGUILayout::begin(this);
	LEEditorGUILayout::beginVLayout(__LINE__);
	if(this->m_pEntity != nullptr) {
		// entity basic information
		m_pEntity->setName(LEEditorGUILayout::stringField(__LINE__, "Name", m_pEntity->getName().c_str()).toStdString());
		m_pEntity->setActive(LEEditorGUILayout::checkBox(__LINE__, "Active",m_pEntity->isActive()));
		int index = LEEditorGUILayout::comboBox(__LINE__, "Layer", { "Default","UI","IgnorePhysical" });
		LEEditorGUILayout::splitter(__LINE__);
		if (LEEditorGUILayout::componentSection(__LINE__, "Transform", false)) {
			this->m_pEntity->getTransform()->setWorldPosition(LEEditorGUILayout::vector3Field(__LINE__, "Position", this->m_pEntity->getTransform()->getWorldPosition()));
			this->m_pEntity->getTransform()->setWorldEulerAngle(LEEditorGUILayout::vector3Field(__LINE__, "Rotation", this->m_pEntity->getTransform()->getWorldEulerAngle()));
			this->m_pEntity->getTransform()->setWorldScale(LEEditorGUILayout::vector3Field(__LINE__, "Scale", this->m_pEntity->getTransform()->getWorldScale()));
		}
		for(LEComponent* pComponent: this->m_pEntity->getAllComponents()) {
			pComponent->onPropertyEditorGUI();
		}
	}
	LEEditorGUILayout::endLayout();
	LEEditorGUILayout::end();
}

void LEEditorPropertyEditorWidget::setEntity(LEEntity* pEntity) {
	this->m_pEntity = pEntity;
	this->onGUI();
}

void LEEditorPropertyEditorWidget::setIdle() {
	this->m_pEntity = nullptr;
	this->onGUI();
	this->update();
}

void LEEditorPropertyEditorWidget::refresh() {
	this->onGUI();
}
