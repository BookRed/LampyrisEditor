/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Base
 * \File:    le_entity.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_entity.h"
#include "../Math/le_transform.h"
#include "Editor/le_interface_property_editor_widget.h"
#if defined(LE_EDITOR)
#include "Editor/le_editor_manager.h"
#include "Editor/le_editor_object_tree_widget.h"
#endif // !LE_EDITOR
void LEEntity::removeChild(LEEntity* pChild) {
	if (pChild == nullptr) {
		return;
	}
	this->m_children.erase(std::find(this->m_children.begin(),
		this->m_children.end(),
		pChild));
}

LEEntity::LEEntity(const std::string& _name):
	m_name(_name), m_active(true), m_layer(0),
	m_pParent(nullptr), m_pTransform(new LETransform) {
	m_pTransform->m_pAttachedEntity = this;

	/* notify editor to update object tree */
	/* update world manager */
}

LEEntity::~LEEntity() {
	/* notify editor to update object tree */

	/* notify level manager */

	/* notify GCManager*/
}

void LEEntity::addComponent(LEComponent* pComponent) {
	for (int i = 0; i < this->m_pComponents.size(); i++) {
		if (this->m_pComponents[i] == pComponent) {
			/*already exists*/
			return;
		}
	}
	this->m_pComponents.push_back(pComponent);
	pComponent->m_pAttachedEntity = this;
}

void LEEntity::removeComponent(LEComponent* pComponent) {
	std::erase(this->m_pComponents, pComponent);
	pComponent->m_pAttachedEntity = nullptr;
}

LEComponent* LEEntity::getComponent(const std::string& name) {
	for (int i = 0; i < this->m_pComponents.size(); i++) {
		if (this->m_pComponents[i]->getComponentName() == name) {
			return this->m_pComponents[i];
		}
	}
	return nullptr;
}

LETransform* LEEntity::getTransform() const {
	return this->m_pTransform;
}

void LEEntity::setParent(LEEntity* pParent) {
	if (this->m_pParent == pParent) {
		return;
	}
	// remove self from parent's children
	if(this->m_pParent) {
		this->m_pParent->m_children.erase(std::ranges::find(this->m_pParent->m_children, this));
	}
	this->m_pParent = pParent;
	if(this->m_pParent) {
		pParent->m_children.push_back(this);
	}
	this->m_pTransform->onParentChanged();
#if defined(LE_EDITOR)
	// notify entity tree to change relationship of entity
	LampyrisEditorManager::getInstance()->getEntityTreeWidget().onEntityParentRelationshipChanged(this);
#endif // !LE_EDITOR
}

LEEntity* LEEntity::getParent() const {
	return this->m_pParent;
}

LEEntity* LEEntity::findFirstByName() {
	return nullptr;
}

LEEntity* LEEntity::findByName() {
	return nullptr;
}

void LEEntity::destorySelf() {

}

void LEEntity::destorySelfLater(float delayTimeMs) {

}

bool LEEntity::isActive() const {
	return this->m_active;
}

void LEEntity::setActive(bool value) {
	this->m_active = value;
}

const std::vector<LEComponent*>& LEEntity::getAllComponents() {
	return this->m_pComponents;
}

const std::vector<LEEntity*>& LEEntity::children() {
	return this->m_children;
}

void LEEntity::setName(const std::string& name) {
	if(this->m_name != name) {
		this->m_name = name;
#if defined(LE_EDITOR)
		LampyrisEditorManager::getInstance()->getEntityTreeWidget().onRenameEntity(this);
		// update name
		LampyrisEditorManager::getInstance()->getPropertyEditorWidget().refresh();
#endif // !LE_EDITOR
	}
}

const std::string& LEEntity::getName() const {
	return this->m_name;
}