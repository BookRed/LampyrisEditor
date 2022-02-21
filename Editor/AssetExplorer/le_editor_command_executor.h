#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_command_executor.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_ABSTRACT_EDITOR_COMMAND_EXECUTOR_H
#define LE_ABSTRACT_EDITOR_COMMAND_EXECUTOR_H

#include <list>
#include <vector>
#include "../../Common/le_global.h"

#include "le_abstract_edtior_command.h"

class LEEditorCommandExecutor {
private:
	// Because we need to display all redo/undo command lists in the editor
	// so we use list instead of stack
	std::list<AbstractCommand*> mRedoStack;
	std::list<AbstractCommand*> mUndoStack;

	std::vector<QString> getCommandStrings(const std::list<AbstractCommand*>& stack) {
		std::vector<QString> returnValue;
		// the direction of traverse should be reverse
		for (auto iter = stack.rbegin(); iter!=stack.rend();iter++) {
			returnValue.push_back((*iter)->toString());
		}
		return returnValue;
	}

	void clearStack(std::list<AbstractCommand*>& stack) {
		for (auto iter = stack.begin(); iter != stack.end(); iter++) {
			LE_SAFE_RELEASE(*iter);
		}
		stack.clear();
	}
public:

	LEEditorCommandExecutor() {}
	~LEEditorCommandExecutor() {}

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
	inline std::vector<QString> getRedoCommandStrings() {
		return this->getCommandStrings(this->mRedoStack);
	}
	inline std::vector<QString> getUndoCommandStrings() {
		return this->getCommandStrings(this->mUndoStack);
	}

	void exec(AbstractCommand* command) {
		command->execute();
		this->mUndoStack.push_back(command);
	}
	void clearStack() {
		this->clearStack(mRedoStack);
		this->clearStack(mUndoStack);
	}

	LE_NONCOPYABLE(LEEditorCommandExecutor);
};
#endif //!LE_ABSTRACT_EDITOR_COMMAND_EXECUTOR_H
