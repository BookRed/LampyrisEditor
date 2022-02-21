/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Base
 * \File:    le_component.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_hash.h"
#include "le_component.h"

LEComponent::LEComponent():
	m_flags(ComponentFLag::CF_Enabled),m_pAttachedEntity(nullptr) {}

LEComponent::~LEComponent() {}

void LEComponent::destorySelf() {

}

void LEComponent::destorySelfLater(float delayTimeMs) {

}

void LEComponent::setEnabled(bool value) {
	if (value) {
		this->m_flags |= (int)ComponentFLag::CF_Enabled;
	}
	else {
		this->m_flags &= ((0xff) ^ (int)ComponentFLag::CF_Enabled);
	}
	/* notify editor to update*/
}

bool LEComponent::isEnabled() const {
	return this->m_flags & (int)ComponentFLag::CF_Enabled;
}

bool LEComponent::isDeletable() const {
	return this->m_flags & (int)ComponentFLag::CF_IS_NOT_DELETEABLE;
}

bool LEComponent::isBuildIn() const {
	return this->m_flags & (int)ComponentFLag::CF_IS_BUILD_IN;
}

LEEntity* LEComponent::getAttachedEntity() const {
	return this->m_pAttachedEntity;
}

bool LEComponent::isRenderer() const {
	return false;
}

#if defined(LE_EDITOR)
#include "Editor/WrappedQt/le_editor_gui_layout.h"
void LEComponent::onPropertyEditorGUI() {
	uint32_t hash = le_hash_value(std::hash<std::string>().operator()("Component"), __LINE__);
	LEEditorGUILayout::componentSection(hash, "Component", true);
}
#endif // !LE_EDITOR