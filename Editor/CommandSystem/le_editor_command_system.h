#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_command_system.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_EDITOR_COMMAND_SYSTEM_H
#define LE_EDITOR_COMMAND_SYSTEM_H

#include <list>
#include <vector>
// LE Includes
#include "le_editor_command.h"
#include "../../Common/le_global.h"

class LEEditorCommandSystem {
private:
	// Because we need to display all redo/undo command lists in the editor
	// so we use list instead of stack
	std::list<LEEditorCommand*> mRedoStack;
	std::list<LEEditorCommand*> mUndoStack;

	std::vector<QString> getCommandStrings(const std::list<LEEditorCommand*>& stack) {
		std::vector<QString> returnValue;
		// the direction of traverse should be reverse
		for (auto iter = stack.rbegin(); iter != stack.rend(); iter++) {
			returnValue.push_back((*iter)->toString());
		}
		return returnValue;
	}

	void clearStack(std::list<LEEditorCommand*>& stack) {
		for (auto iter = stack.begin(); iter != stack.end(); iter++) {
			LE_SAFE_RELEASE(*iter);
		}
		stack.clear();
	}
public:

	LEEditorCommandSystem() {}
	~LEEditorCommandSystem() {}

	void executeSingleRedo() {
		if (!this->mRedoStack.size()) {
			return;
		}
		auto command = this->mRedoStack.back();
		command->execute();
		this->mRedoStack.pop_back();
		this->mUndoStack.push_back(command);
	}
	void executeSingleUndo() {
		if (!this->mUndoStack.size()) {
			return;
		}
		auto command = this->mUndoStack.back();
		command->cancel();
		this->mUndoStack.pop_back();
		this->mRedoStack.push_back(command);
	}
	// two methods followed call by editor's undo/redo list
	void executeRedos(const unsigned int count) {
		assert(count <= this->mRedoStack.size());
		for (unsigned int i = 0; i < count; i++) {
			auto command = this->mRedoStack.back();
			command->execute();
			this->mRedoStack.pop_back();
			this->mUndoStack.push_back(command);
		}
	}
	void executeUndos(const unsigned int count) {
		assert(count <= this->mUndoStack.size());
		for (unsigned int i = 0; i < count; i++) {
			auto command = this->mUndoStack.back();
			command->cancel();
			this->mUndoStack.pop_back();
			this->mRedoStack.push_back(command);
		}
	}
	LE_FORCEINLINE std::vector<QString> getRedoCommandStrings() {
		return this->getCommandStrings(this->mRedoStack);
	}
	LE_FORCEINLINE std::vector<QString> getUndoCommandStrings() {
		return this->getCommandStrings(this->mUndoStack);
	}

	void exec(LEEditorCommand* command) {
		command->execute();
		this->mUndoStack.push_back(command);
	}
	void clearStack() {
		this->clearStack(mRedoStack);
		this->clearStack(mUndoStack);
	}
};

#endif // !LE_EDITOR_COMMAND_SYSTEM_H