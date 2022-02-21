/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_file_explorer_widget.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#include "le_file_explorer_widget.h"

void LEEditorAssetExplorerWidget::doTopToolBarCreation() {
	// the layout of toolBar will be:
	// ----------------------------------------------------------------------------
	// | back | forward | home | search file icon/edit| collapse | expand |filter |
	// ----------------------------------------------------------------------------
		
	// Property
	this->mToolBarTop->setIconSize(QSize(20, 20));
	// Action Back and Forward
	QString basePath = "res\\file\\";
	QIcon icoBack, icoForward;
	icoBack.addFile(basePath + "back_arrow.png", QSize(),
	                QIcon::Normal, QIcon::On);
	icoBack.addFile(basePath + "back_arrow_dis.png", QSize(),
	                QIcon::Mode::Disabled, QIcon::On);
	QAction* backAction = new QAction(icoBack, "Back");
	this->mToolBarTop->addAction(backAction);

	icoForward.addFile(basePath + "forward_arrow.png", QSize(),
	                   QIcon::Normal, QIcon::On);
	icoForward.addFile(basePath + "forward_arrow_dis.png", QSize(),
	                   QIcon::Disabled, QIcon::On);
	QAction* forwardAction = new QAction(icoForward, "Forward");
	this->mToolBarTop->addAction(forwardAction);

	// home
	QIcon icoHome(basePath + "home.png");
	QAction* homeAction = new QAction(icoHome, "Home");
	this->mToolBarTop->addAction(homeAction);

	// parent dir
	QIcon icoParentDir;
	icoParentDir.addFile(basePath + "parent_dir.png", QSize(),
	                     QIcon::Normal, QIcon::On);
	icoParentDir.addFile(basePath + "parent_dir_dis.png", QSize(),
	                     QIcon::Disabled, QIcon::On);
	QAction* parentDirAction = new QAction(icoParentDir, "Back to parent");
	this->mToolBarTop->addAction(parentDirAction);

	// Search File Edit
	QWidget* widgetSpacer = new QWidget;
	this->mToolBarTop->addWidget(widgetSpacer);
	this->mToolBarTop->addWidget(this->mEditSearchFile);

	QString editSearchFileQSS = QString("QLineEdit{"
		"background-image: url(\"%1search.png\");"
		"background - repeat: no - repeat;"
		"background - position: right;}").arg(basePath);
	this->mEditSearchFile->setStyleSheet(editSearchFileQSS);

	QIcon icoCollapse(basePath + "collapse.png");
	QAction* collapseAction = new QAction(icoCollapse, "Collapse All");
	this->mToolBarTop->addAction(collapseAction);

	QIcon icoExpand(basePath + "expand.png");
	QAction* expandAction = new QAction(icoExpand, "Expand All");
	this->mToolBarTop->addAction(expandAction);

	// Filter
	QIcon icoFilter(basePath + "filter.png");
	QAction* filterAction = new QAction(icoFilter, "Filter");
	this->mToolBarTop->addAction(filterAction);

	this->mBackAction      = backAction;
	this->mForwardAction   = forwardAction;
	this->mHomeAction      = homeAction;
	this->mParentDirAction = parentDirAction;
	this->mCollapseAction  = collapseAction;
	this->mExpandAction    = expandAction;
	this->mFilterAction    = filterAction;

	this->mBackAction->setEnabled(false);
	this->mForwardAction->setEnabled(false);
	this->mToolBarTop->show();
}

void LEEditorAssetExplorerWidget::doLayoutCreation() {
	// the layout of file explore will be:
	// ------------------
	// |     toolBar    |  
	// ------------------
	// | tree   |  icon |   Vertical  
	// | view   |  view |  Layout Main
	// |        |       |
	// ------------------
	// Horizontal Layout 

	// [1] mWidgetTop contains toolBar
	// [2] mWidgetBottom contains splitter 
	//     and splitter contains treeView and tableView(aka iconView)

	this->setLayout(this->mVerticalLayoutMain);

	// MainLayout:toolBar,WidgetBottom
	this->mVerticalLayoutMain->setContentsMargins(9, 0, 9, 0);
	this->mVerticalLayoutMain->addWidget(this->mToolBarTop);
	this->mPathBar->setCurrentPath("");
	this->mVerticalLayoutMain->addWidget(mPathBar);
	this->mVerticalLayoutMain->addWidget(this->mWidgetBottom);

	// BottomLayout:splitter
	this->mWidgetBottom->setLayout(this->mHorizontalLayoutBottom);
	this->mHorizontalLayoutBottom->setContentsMargins(2, 0, 2, 0);
	this->mHorizontalLayoutBottom->addWidget(this->mSplitter);
	this->mSplitter->addWidget(this->mTreeView);
	this->mSplitter->addWidget(this->mListView);
	this->mSplitter->setStretchFactor(0, 3);
	this->mSplitter->setStretchFactor(1, 5);
}

void LEEditorAssetExplorerWidget::doFileViewsCreation() {
	this->mTreeModel = new LEEditorFileSystemModel;
	this->mTreeModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);

	this->mListModel = new LEEditorFileSystemModel;
	this->mListModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);

	this->mTreeView->setModel(this->mTreeModel);
	this->mListView->setModel(this->mListModel);
	this->mListView->setViewMode(QListView::ViewMode::IconMode);
	this->mListView->setIconSize(QSize(50,50));
	this->mListView->setLayoutMode(QListView::Batched);
	// this->mListView->setMovement(QListView::Static);
	this->mListView->setResizeMode(QListView::Adjust);
	// this->mListView->setContentsMargins(50, 50, 50, 50);
	this->mListView->setUniformItemSizes(true);;
	this->mListView->setBaseSize(30, 30);
	this->mListView->setWordWrap(true);
	this->mListView->setUniformItemSizes(true);
	this->mTreeView->sortByColumn(0, Qt::AscendingOrder);
	for (int i = 1; i < this->mTreeModel->columnCount(); i++)
		this->mTreeView->setColumnHidden(i, true);
}

void LEEditorAssetExplorerWidget::doUpdateActionEnabled() {
	this->mBackAction->setEnabled(this->mHistoryIndex > 0);
	this->mForwardAction->setEnabled(this->mHistoryIndex < this->mHistoryInfoList.size());
}

void LEEditorAssetExplorerWidget::doSignalConnection() {
	QObject::connect(this->mListView, &QListView::doubleClicked,
	                 this, &LEEditorAssetExplorerWidget::slotListViewDoubleClicked);

	QObject::connect(this->mBackAction, &QAction::triggered,
	                 this, &LEEditorAssetExplorerWidget::slotBackAction);

	QObject::connect(this->mForwardAction, &QAction::triggered,
	                 this, &LEEditorAssetExplorerWidget::slotForwardAction);

	QObject::connect(this->mParentDirAction, &QAction::triggered,
	                 this, &LEEditorAssetExplorerWidget::slotBackToParentDir);

	QObject::connect(this->mEditSearchFile, &QLineEdit::editingFinished,
	                 this, &LEEditorAssetExplorerWidget::slotSearchFile);

	QObject::connect(this->mPathBar, &LEEditorAssetExplorerPathToolBar::signalPathChanged,
	                 this, &LEEditorAssetExplorerWidget::slotPathChanged);
}

void LEEditorAssetExplorerWidget::slotBackAction() {
	assert(this->mHistoryIndex > 0);
	QString subPath = this->mListModel->filePath(this->mListView->rootIndex());
	this->mListView->setRootIndex(this->mListModel->index(this->mHistoryInfoList[--this->mHistoryIndex].pathOrSerachName));
	this->mPathBar->setCurrentPath(this->mHistoryInfoList[this->mHistoryIndex].pathOrSerachName);
	this->mHistoryInfoList[this->mHistoryIndex].pathOrSerachName = subPath;
	this->doUpdateActionEnabled();
}

void LEEditorAssetExplorerWidget::slotForwardAction() {
	assert(this->mHistoryIndex < this->mHistoryInfoList.size());
	this->mListView->setRootIndex(this->mListModel->index(this->mHistoryInfoList[this->mHistoryIndex].pathOrSerachName));
	this->mPathBar->setCurrentPath(this->mHistoryInfoList[this->mHistoryIndex].pathOrSerachName);
	this->mHistoryInfoList[this->mHistoryIndex++].pathOrSerachName = this->mListModel->filePath(this->mListView->rootIndex().parent());
	this->doUpdateActionEnabled();
}

void LEEditorAssetExplorerWidget::slotSearchFile() {

}

void LEEditorAssetExplorerWidget::slotBackToParentDir() {
	this->mListView->setRootIndex(this->mListView->rootIndex().parent());
	HistoryInfo historyInfo;
	historyInfo.type             = HistoryInfoType::DiskDir;
	historyInfo.pathOrSerachName = this->getCurrentPath();
	this->mHistoryInfoList.push_back(historyInfo);
	this->mPathBar->setCurrentPath(this->mListModel->rootPath());
	this->mHistoryIndex++;
	this->doUpdateActionEnabled();
}

void LEEditorAssetExplorerWidget::slotCollapse() {

}

void LEEditorAssetExplorerWidget::slotExpand() {

}

void LEEditorAssetExplorerWidget::slotFilter() {

}

void LEEditorAssetExplorerWidget::slotListViewDoubleClicked(const QModelIndex& index) {
	QString clickedPath = this->mListModel->filePath(index);
	QFileInfo info(clickedPath);
	if (info.isDir()) {
		this->slotPathChanged(clickedPath);
	}
}

void LEEditorAssetExplorerWidget::slotPathChanged(const QString& targetPath) {
	HistoryInfo historyInfo;
	historyInfo.type = HistoryInfoType::DiskDir;
	historyInfo.pathOrSerachName = this->getCurrentPath();
	if (this->mHistoryIndex < this->mHistoryInfoList.size()) {
		this->mHistoryInfoList[this->mHistoryIndex] = historyInfo;
	}
	else {
		this->mHistoryInfoList.push_back(historyInfo);
	}
	this->mListView->setRootIndex(this->mListModel->index(targetPath));
	this->mPathBar->setCurrentPath(targetPath);
	this->mHistoryIndex++;
	this->doUpdateActionEnabled();
}

QString LEEditorAssetExplorerWidget::getCurrentPath() {
	return this->mListModel->filePath(this->mListView->rootIndex());
}

void LEEditorAssetExplorerWidget::setRootPath(const QString& path) {
	this->mListModel->setRootPath(path);
	this->mListView->setRootIndex(this->mListModel->index(path));
	this->mTreeModel->setRootPath(path);
	this->mTreeView->setRootIndex(this->mTreeModel->index(path));
}

void LEEditorAssetExplorerWidget::mousePressEvent(QMouseEvent* event) {
	QWidget::mousePressEvent(event);
}

LEEditorAssetExplorerWidget::LEEditorAssetExplorerWidget(QWidget *parent) :
	QWidget                     (parent),
	mSplitter                   (new QSplitter),
	mVerticalLayoutMain		    (new QVBoxLayout),
	mHorizontalLayoutBottom	    (new QHBoxLayout),
	mWidgetTop				    (new QWidget),
	mWidgetBottom			    (new QWidget),
	mEditSearchFile			    (new QLineEdit),
	mTreeView				    (new LEEditorAssetExplorerTreeView),
	mListView				    (new LEEditorAssetExplorerListView),
	mToolBarTop				    (new QToolBar),
	mPathBar                    (new LEEditorAssetExplorerPathToolBar) {

	this->doTopToolBarCreation();
	this->doLayoutCreation();
	this->doFileViewsCreation();
	this->doSignalConnection();
	this->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
	this->setRootPath("D:\\Software\\");
}

LEEditorAssetExplorerWidget::~LEEditorAssetExplorerWidget() {

}
