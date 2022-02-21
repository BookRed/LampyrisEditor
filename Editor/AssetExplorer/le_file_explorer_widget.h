#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_file_explorer_widget.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_FILE_EXPLORER_WIDGET_H
#define LE_FILE_EXPLORER_WIDGET_H

#include <QFile>
#include <QThread>
#include <QApplication>

#include <vector>

#include "../../Common/le_global.h"
#include <le_editor_events.h>
#include <le_editor_controls.h>
#include <QToolBar>
#include <QVBoxLayout>
#include <QSplitter>

#include "le_file_system_model.h"
#include "le_file_explorer_tree_view.h"
#include "le_file_explorer_list_view.h"
#include "le_file_explorer_path_tool_bar.h"

static void le_get_all_folders_in_given_path(const QString &dirPath, QVector<QString>& folderList) {
	QDir dir(dirPath);
	dir.setFilter(QDir::Dirs);
	foreach(QFileInfo fullDir, dir.entryInfoList()) {
		if (fullDir.fileName() == "." || fullDir.fileName() == "..") continue;
		folderList.push_back(fullDir.absoluteFilePath());
		le_get_all_folders_in_given_path(fullDir.absoluteFilePath(), folderList);
	}
}

enum class HistoryInfoType {
	DiskDir,
	SearchResult
};
struct HistoryInfo {
	HistoryInfoType  type;
	QString          pathOrSerachName;

	bool friend operator == (const HistoryInfo& x, const HistoryInfo& y) {
		return x.type == y.type && x.pathOrSerachName == y.pathOrSerachName;
	}
};

class FileSerachThread :public QThread {
private:
	std::vector<QString> mPath;
signals:
	void signalSearchUpdated(const QString& path);
	void signalSearchFinished(const std::vector<QString>& paths);
public:
	virtual void run() {

	}
};

class LEEditorAssetExplorerWidget:public QWidget
{
	using HistoryInfoList = std::vector<HistoryInfo>;
private:
    QSplitter*		                  mSplitter;
	QVBoxLayout*	                  mVerticalLayoutMain;
	QHBoxLayout*	                  mHorizontalLayoutBottom;
	QWidget*		                  mWidgetTop;
	QWidget*		                  mWidgetBottom;
	QLineEdit*		                  mEditSearchFile;
	LEEditorAssetExplorerTreeView*    mTreeView;
	LEEditorAssetExplorerListView*    mListView;
	QToolBar*		                  mToolBarTop;
	QFileSystemModel*                 mTreeModel;
	QFileSystemModel*                 mListModel;
					                  
	QAction*                          mBackAction;
	QAction*                          mForwardAction;
	QAction*                          mHomeAction;
	QAction*                          mParentDirAction;
	QAction*                          mCollapseAction;
	QAction*                          mExpandAction;
	QAction*                          mFilterAction;
			         		          
	HistoryInfoList			          mHistoryInfoList;

	LEEditorAssetExplorerPathToolBar* mPathBar;

	// if path is 'D:/dir_xxx/file_xxx'
	// the depth of path is 2
	int                               mHistoryIndex = 0;

	void doTopToolBarCreation();

	void doLayoutCreation();

	void doFileViewsCreation();

	void doUpdateActionEnabled();

	void doSignalConnection();

	void slotBackAction();

	void slotForwardAction();

	void slotSearchFile();

	void slotBackToParentDir();

	void slotCollapse();

	void slotExpand();

	void slotFilter();

	void slotListViewDoubleClicked(const QModelIndex& index);

	void slotPathChanged(const QString& targetPath);

	QString getCurrentPath();

	void setRootPath(const QString& path);

public:
	virtual void mousePressEvent(QMouseEvent* event);

	LEEditorAssetExplorerWidget(QWidget* parent = nullptr);

    ~LEEditorAssetExplorerWidget();
};

#endif //!LE_FILE_EXPLORER_WIDGET_H