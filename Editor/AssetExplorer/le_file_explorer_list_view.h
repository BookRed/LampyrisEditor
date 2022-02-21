#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_file_explorer_list_view.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_FILE_EXPLORER_TABLE_VIEW_H
#define LE_FILE_EXPLORER_TABLE_VIEW_H

#include <QListView>
#include <QKeyEvent>

#include "le_file_system_model.h"

class LEEditorAssetExplorerListView:public QListView {
private:
	bool                 mModifierKeyCtrlPressed;
	bool                 mModifierKeyShiftPressed;
	bool                 mModifierKeyAltPressed;
	void doPropertySetting() {
		this->setWordWrap(true);
		this->setEditTriggers(QAbstractItemView::NoEditTriggers);
		this->setMouseTracking(true);
	}
	void doModelCreation() {
		this->setModel(new LEEditorFileSystemModel);
	}
	void doMenuActionCreation() {

	}
	void doSignalConnection() {

	}
	void doCustomContextMenuCreation() {

	}

	void slotUpdateIconSize() {
		this->setIconSize(QSize(20, 20));
		this->viewport()->update();
	}
protected:
	virtual void wheelEvent(QWheelEvent* event) {
		if (!this->mModifierKeyCtrlPressed) {
			QListView::wheelEvent(event);
		}
		else {
			QPoint numDegrees = event->angleDelta() / 8;
			QSize deltaSize;
			// up direction
			if (numDegrees.y() > 0) {
				deltaSize = QSize(10, 10);
			}
			// down direction
			else if (numDegrees.y() < 0) {
				deltaSize = QSize(-10, -10);
			}
			QSize targetSize = this->iconSize() + deltaSize;
			if (targetSize.width() < 20) {
				targetSize = QSize(20, 20);
			}
			else if (targetSize.width() > 150) {
				targetSize = QSize(150, 150);
			}
			this->setIconSize(targetSize);
			this->viewport()->update();
		}	
	}
	virtual void contextMenuEvent(QContextMenuEvent* event) {

	}
	virtual void keyPressEvent(QKeyEvent* event) {
		this->mModifierKeyShiftPressed = event->key() == Qt::Key_Shift;
		this->mModifierKeyCtrlPressed = event->key() == Qt::Key_Control;
		this->mModifierKeyAltPressed = event->key() == Qt::Key_Alt;

		if (mModifierKeyCtrlPressed) {
			switch (event->key()) {
			case Qt::Key_X:
				//slotCut();
				break;
			case Qt::Key_C:
				//slotCopy();
				break;
			case Qt::Key_V:
				//slotPaste();
				break;
			case Qt::Key_Y:
				//mExecutor->executeSingleRedo();
				break;
			case Qt::Key_Z:
				//mExecutor->executeSingleUndo();
				break;
			default:
				break;
			}
		}
		else {
			switch (event->key()) {
			case Qt::Key_Delete:
				//slotDelete();
				break;
			default:
				break;
			}
		}
		if (this->mModifierKeyCtrlPressed) {
			this->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);
		}
	}
	virtual void keyReleaseEvent(QKeyEvent* event) {
		this->mModifierKeyShiftPressed = !(event->key() == Qt::Key_Shift);
		this->mModifierKeyCtrlPressed = !(event->key() == Qt::Key_Control);
		this->mModifierKeyAltPressed = !(event->key() == Qt::Key_Alt);
		if (!this->mModifierKeyCtrlPressed) {
			this->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
		}
	}
public:
	LEEditorAssetExplorerListView():
		mModifierKeyShiftPressed(false),
		mModifierKeyCtrlPressed(false),
		mModifierKeyAltPressed(false) { 
		this->doPropertySetting();
		this->doModelCreation();
		this->doMenuActionCreation();
		this->doSignalConnection();
		this->doCustomContextMenuCreation();
	}

	~LEEditorAssetExplorerListView() {

	}
};
#endif // !LE_FILE_EXPLORER_TABLE_VIEW_H

