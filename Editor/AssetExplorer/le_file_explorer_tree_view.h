#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_file_explorer_list_view.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_FILE_EXPLORER_TREE_VIEW_H
#define LE_FILE_EXPLORER_TREE_VIEW_H

// Qt Includes
#include <QFile>
#include <QMenu>
#include <QCursor>
#include <QAction>
#include <QTreeView>
#include <QKeyEvent>
#include <QMimeData>
#include <QClipboard>
#include <QApplication>
#include <QInputDialog>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <QPainter>
#include <QHeaderView>
#include <QActionGroup>
// STL Includes
#include <unordered_set>

#include "../../Common/le_global.h"
// LE Includes
#include <le_string_hash.h>
#include <le_hash.h>
#include "le_file_logger.h"
#include "le_file_system_model.h"
#include "le_file_explorer_commands.h"
#include "le_editor_command_executor.h"
#include "le_file_recycle_bin.h"

static inline size_t le_model_index_hash(const LEEditorFileSystemModel* model,const QModelIndex &index) {
	assert(model != nullptr && index.isValid());
	int row = index.row();
	int column = index.column();
	size_t returnValue = le_hash_value(row, column);
	return returnValue;
}
class LE_API_EXPORT LEEditorAssetExplorerTreeView:public QTreeView {
	using FileIndexSet = std::unordered_set<size_t>;
private:
	QAction*             mCopyAction;
	QAction*             mCutAction;
	QAction*             mDeleteAction;
	QAction*             mPasteAction;
	QAction*             mRenameAction;
	QAction*             mMkDirAction;
	QAction*             mEditorScriptAction;
	QAction*             mMkCustomFileAction;
					     
	QMenu*               mSortMenu;
	QMenu*               mMainMenu;
	QMenu*               mCreateFileMenu;
	QMenu*               mEditorScriptMenu;
					     
	QString              mPathOnMousePos;
	FileLogger*          mLogger;
	LEEditorFileSystemModel*     mModel;
	LEEditorCommandExecutor*     mExecutor;
	FileRecycleBin*      mRecycleBin;
	FileIndexSet         mToBeCutFileIndexSet;
					     
	bool                 mModifierKeyCtrlPressed;
	bool                 mModifierKeyShiftPressed;
	bool                 mModifierKeyAltPressed;

	bool                 isToBeCutFileIndex(const QModelIndex& index) const;
		                 
	bool                 isPasteEnable() const;

	bool                 isFileOrDirSelected() const { return !this->mPathOnMousePos.isEmpty(); }

	void                 doModelCreation();
		                 
	void                 doPropertySetting();
		                 
	void                 doMenuActionCreation();
		                 
	void                 doCustomContextMenuCreation();
		                 
	void                 doSignalConnection();
		                 
	void                 doMenuActionEnableUpdate();

	void                 doClearToBeCutFileIndexSet();

	void                 setToBeCutFileIndexAfterCutPressed();

	void                 setMineDataAfterCopyOrCutPressed(PasteMode mode);

	std::vector<QString> getSelectedPaths() const;

	std::vector<QString> getPathsFromClipBoard() const;

	QString              getDirOnMousePosition() const;

	PasteMode            getPasteModeOfCurrentClipBoard() const;

	void                 slotCopy();

	void                 slotCut();

	void                 slotPaste();

	void                 slotMkDir();

	void                 slotMkFile();

	void                 slotRename();

	void                 slotSort();

	void                 slotDelete();

protected:
	virtual void         drawRow(QPainter* painter,
				  	        	 const QStyleOptionViewItem& options,
								 const QModelIndex& index) const;
				         
	virtual void         contextMenuEvent(QContextMenuEvent* event);
				         
	virtual void         keyPressEvent(QKeyEvent* event);

public:
	LEEditorAssetExplorerTreeView():mExecutor(new LEEditorCommandExecutor),
		                   mRecycleBin(new FileRecycleBin) {
		this->doPropertySetting();
		this->doMenuActionCreation();
		this->doSignalConnection();
		this->doCustomContextMenuCreation();
	}

	~LEEditorAssetExplorerTreeView() {
		
	}

	void setModel(QAbstractItemModel* model) override {
		this->mModel = (LEEditorFileSystemModel*)model;
		QTreeView::setModel(model);
	}
};

#endif //!LE_FILE_EXPLORER_TREE_VIEW_H