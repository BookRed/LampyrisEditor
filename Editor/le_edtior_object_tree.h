#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor/
 * \File:    le_editor_object_tree.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_EDITOR_OBJECT_TREE_H
#define LE_EDITOR_OBJECT_TREE_H

// abstract class,it's a bridge between gameobject manager and object tree widget
class LEEditorObjectTree {
public:
	// Selection-relative 
	virtual LEGameObject* selectedObject() = 0;

	virtual std::vector<LEGameObject*> selectedObjects() = 0;

	virtual LESize selectedCount() = 0;

	virtual void cancelSelection() = 0;

	virtual void deleteSelection() = 0;

	virtual void doSelectionChanged() = 0;

	virtual void selectAll() = 0;

	virtual void selectFirstByName() = 0;

	virtual void selectObject() = 0;

	// Copy-relative
	virtual void copySelection() = 0;

	// Delete-relative
	virtual void deleteSelection() = 0;

	// Rename-relative
	virtual void renameObject() = 0;
};
#endif // !LE_EDITOR_OBJECT_TREE_H
