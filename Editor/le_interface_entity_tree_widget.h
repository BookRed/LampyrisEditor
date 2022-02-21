#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_interface_entity_tree_widget.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
class LEEntity;

class IEntityTreeWidget {
protected:
	// If 'm_notifyDisabled' is true,
	// all updates that are notified to the editor by the methods of the runtime class are ignored.
	// For example, class 'LEEntity' has a method named 'setParent',
	// If you drag the item of entity tree on LampyrisEditor,
	// method 'setParent' will be called, and 'setParent' will notify the editor to update entity tree,
	// which will cause an infinite loop, so that's the function of 'm_notifyDisabled'.
	bool         m_notifyDisabled{false};

	void         disableNotify() { this->m_notifyDisabled = true; }

	void         enableNotify() { this->m_notifyDisabled = false; }

	bool         checkNotifyEnabled() const { return !this->m_notifyDisabled; }
public:
	virtual void onAddEntity(LEEntity* pEntity) = 0;

	virtual void onRenameEntity(LEEntity* pEntity) = 0;

	virtual void onDeleteEntity(LEEntity* pEntity) = 0;

	virtual void onEntityLockStateChanged(LEEntity* pEntity) = 0;

	virtual void onEntityVisibleStateChanged(LEEntity* pEntity) = 0;

	virtual void onEntityParentRelationshipChanged(LEEntity* pEntity) = 0;

	virtual void onPickupEntity(LEEntity* pEntity) = 0;
};