#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor/Command
 * \File:    le_abstract_editor_command.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_ABSTRACT_EDITOR_COMMAND_H
#define LE_ABSTRACT_EDITOR_COMMAND_H

#include <QString>

class AbstractCommand {
private:
	virtual void    execute()  = 0;
	virtual void    cancel()   = 0;
	virtual QString toString() = 0;

	friend class LEEditorCommandExecutor;
};
#endif //!LE_ABSTRACT_EDITOR_COMMAND_H