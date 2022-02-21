/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_file_explorer_list_view.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#include "le_file_explorer_tree_view.h"

bool LEEditorAssetExplorerTreeView::isToBeCutFileIndex(const QModelIndex& index) const {
	auto hashValue = le_model_index_hash(mModel, index);
	return this->mToBeCutFileIndexSet.find(hashValue) !=
		this->mToBeCutFileIndexSet.end();
}

bool LEEditorAssetExplorerTreeView::isPasteEnable() const {
	QClipboard* clipBoard = QApplication::clipboard();
	const QMimeData* mimeData = clipBoard->mimeData();
	return mimeData->hasUrls();
}

void LEEditorAssetExplorerTreeView::doPropertySetting() {
	this->setMouseTracking(true);
	this->setDragEnabled(true);
	this->setContextMenuPolicy(Qt::DefaultContextMenu);
	this->setSelectionMode(QAbstractItemView::SingleSelection);
	this->header()->hide();
	this->setEditTriggers(EditTriggers::enum_type::DoubleClicked);
}

void LEEditorAssetExplorerTreeView::doMenuActionCreation() {
	mCopyAction         = new QAction("Copy");
	mCutAction          = new QAction("Cut");
	mDeleteAction       = new QAction("Delete");
	mPasteAction        = new QAction("Paste");
	mRenameAction       = new QAction("Rename");
	mMkDirAction        = new QAction("Create Directory");
	mMkCustomFileAction = new QAction("Custom File");
	mSortMenu           = new QMenu("Sort By");
	mCreateFileMenu     = new QMenu("Create File");
	mEditorScriptMenu   = new QMenu("Scripting");

	mCreateFileMenu->addAction(mMkCustomFileAction);
}

void LEEditorAssetExplorerTreeView::doCustomContextMenuCreation() {
	mMainMenu = new QMenu;
	mMainMenu->addMenu(mSortMenu);
	mMainMenu->addSeparator();
	mMainMenu->addAction(mCopyAction);
	mMainMenu->addAction(mCutAction);
	mMainMenu->addAction(mDeleteAction);
	mMainMenu->addAction(mPasteAction);
	mMainMenu->addAction(mRenameAction);
	mMainMenu->addSeparator();
	mMainMenu->addAction(mMkDirAction);
	mMainMenu->addMenu(mEditorScriptMenu);
}

void LEEditorAssetExplorerTreeView::doSignalConnection() {
	QObject::connect(this->mCopyAction, &QAction::triggered,
		this, &LEEditorAssetExplorerTreeView::slotCopy);
	QObject::connect(this->mCutAction, &QAction::triggered,
		this, &LEEditorAssetExplorerTreeView::slotCut);
	QObject::connect(this->mDeleteAction, &QAction::triggered,
		this, &LEEditorAssetExplorerTreeView::slotDelete);
	QObject::connect(mPasteAction, &QAction::triggered,
		this, &LEEditorAssetExplorerTreeView::slotPaste);
	QObject::connect(this->mRenameAction, &QAction::triggered,
		this, &LEEditorAssetExplorerTreeView::slotRename);
	QObject::connect(this->mMkCustomFileAction, &QAction::triggered,
		this, &LEEditorAssetExplorerTreeView::slotMkFile);
	QObject::connect(this->mMkDirAction, &QAction::triggered,
		this, &LEEditorAssetExplorerTreeView::slotMkDir);
}

void LEEditorAssetExplorerTreeView::doMenuActionEnableUpdate() {
	this->mCopyAction->setEnabled(this->isFileOrDirSelected());
	this->mCutAction->setEnabled(this->isFileOrDirSelected());
	this->mPasteAction->setEnabled(this->isFileOrDirSelected() && this->isPasteEnable());
	this->mDeleteAction->setEnabled(this->isFileOrDirSelected());
	this->mRenameAction->setEnabled(this->isFileOrDirSelected());
}

void LEEditorAssetExplorerTreeView::doClearToBeCutFileIndexSet() {
	this->mToBeCutFileIndexSet.clear();
}

void LEEditorAssetExplorerTreeView::setMineDataAfterCopyOrCutPressed(PasteMode mode) {
	auto filePaths = this->getSelectedPaths();
	QClipboard* clipBoard = QApplication::clipboard();
	QMimeData* mineData = new QMimeData;
	QList<QUrl> urls;

	for (QString path : filePaths) {
#if defined( _WIN32)
		QUrl url = QUrl("file:///" + path);
		urls.push_back(url);
#endif
	}

	mineData->setUrls(urls);
#if defined( _WIN32)
	QByteArray array;
	if (mode == PasteMode::Copy) {
		array.append('\x5');
	}
	else {
		array.append('\x2');
	}
	array.append('\x0');
	array.append('\x0');
	array.append('\x0');
	mineData->setData("Preferred DropEffect", array);
#endif 
	clipBoard->setMimeData(mineData);
}

void LEEditorAssetExplorerTreeView::setToBeCutFileIndexAfterCutPressed() {
	this->mToBeCutFileIndexSet.clear();
	QModelIndexList selectedIndexList = this->selectionModel()->selectedRows(0);
	for (const auto& index : selectedIndexList) {
		auto hashValue = le_model_index_hash(mModel, index);
		this->mToBeCutFileIndexSet.insert(hashValue);
	}
}

std::vector<QString> LEEditorAssetExplorerTreeView::getSelectedPaths() const {
	QModelIndexList selectedIndexList = this->selectionModel()->selectedRows(0);
	std::vector<QString> returnValue;
	returnValue.reserve(selectedIndexList.size());
	for (const auto& index : selectedIndexList) {
		QString filePath = mModel->filePath(index);
		returnValue.push_back(filePath);
	}
	return returnValue;
}

std::vector<QString> LEEditorAssetExplorerTreeView::getPathsFromClipBoard() const {
	std::vector<QString> returnValue;
	QClipboard* clipBoard = QApplication::clipboard();
	const QMimeData* mimeData = clipBoard->mimeData();
	if (mimeData->hasUrls()) {
		auto urls = mimeData->urls();
		for (const auto& url : urls) {
#ifdef _WIN32
			QString filePath = url.toString();
			if (!filePath.startsWith("file:///")) {
				continue;
			}
			returnValue.push_back(filePath.mid(8));
#endif
		}
	}
	return returnValue;
}

QString LEEditorAssetExplorerTreeView::getDirOnMousePosition() const {
	QModelIndex index = this->indexAt(this->viewport()->mapFromGlobal(QCursor::pos()));
	if (index.isValid()) {
		QString filePath = mModel->filePath(index);
		QFileInfo fileInfo(filePath);
		if (fileInfo.isFile()) {
			index = index.parent();
			return mModel->filePath(index);
		}
		else if (fileInfo.isDir()) {
			return filePath;
		}
	}
	return QString();
}

PasteMode LEEditorAssetExplorerTreeView::getPasteModeOfCurrentClipBoard() const {
	QClipboard* clipBoard = QApplication::clipboard();
	const QMimeData* mimeData = clipBoard->mimeData();
#if defined( _WIN32)
	QByteArray dropData = mimeData->data("Preferred DropEffect");
	if (dropData[0] == '\x5') {
		return PasteMode::Copy;
	}
	else if (dropData[0] == '\x2') {
		return PasteMode::Cut;
	}
	else {
		return PasteMode::Undefined;
	}
#endif 
}

void LEEditorAssetExplorerTreeView::slotCopy() {
	this->setMineDataAfterCopyOrCutPressed(PasteMode::Copy);
	this->doClearToBeCutFileIndexSet();
}

void LEEditorAssetExplorerTreeView::slotCut() {
	this->setMineDataAfterCopyOrCutPressed(PasteMode::Cut);
	this->setToBeCutFileIndexAfterCutPressed();
}

void LEEditorAssetExplorerTreeView::slotPaste() {
	//PasteCommand* command = new PasteCommand(this->getDirOnMousePosition(),
	//	this->getPathsFromClipBoard(),
	//	this->getPasteModeOfCurrentClipBoard());
	//this->mExecutor->exec(command);
	//this->doClearToBeCutFileIndexSet();
}

void LEEditorAssetExplorerTreeView::slotMkDir() {
	QString dirName = QInputDialog::getText(nullptr,
		QStringLiteral("Create Directory"),
		QStringLiteral("Please input the name of directory"));
	QString currentDir = mModel->filePath(this->currentIndex());
	QDir dir(currentDir);
	if (!dir.isRoot()) {
		currentDir = mModel->filePath(this->currentIndex().parent());
	}
	dir.mkdir(currentDir + dirName);

	this->doClearToBeCutFileIndexSet();
}

void LEEditorAssetExplorerTreeView::slotMkFile() {
	QString fileName = QInputDialog::getText(nullptr,
		QStringLiteral("Create File"),
		QStringLiteral("Please input the name of file you want to create"));
	if (fileName.size()) {
		MkFileCommand* command = new MkFileCommand(mPathOnMousePos, fileName);
		this->mExecutor->exec(command);
	}
	this->doClearToBeCutFileIndexSet();
}

void LEEditorAssetExplorerTreeView::slotRename() {
	QModelIndex currentIndex = this->currentIndex();
	this->openPersistentEditor(currentIndex);
	return;
	QString fileName = QInputDialog::getText(nullptr,
		QStringLiteral("Rename File"),
		QStringLiteral("Please input the name of file"));

	QDir dir;
	QString newName = QFileInfo(mModel->filePath(currentIndex)).path() + "\\" + fileName;
	if (dir.rename(mModel->filePath(currentIndex), newName)) {
		return;
	}
}

void LEEditorAssetExplorerTreeView::slotSort() {

}

void LEEditorAssetExplorerTreeView::slotDelete() {
	DeleteCommand* command = new DeleteCommand(mRecycleBin,
		getSelectedPaths(),
		mModifierKeyShiftPressed ? DeleteMode::MoveToReCycle :
		DeleteMode::Completely);
	this->mExecutor->exec(command);
}

void LEEditorAssetExplorerTreeView::drawRow(QPainter* painter, 
	                               const QStyleOptionViewItem& options, 
	                               const QModelIndex& index) const {
	float opacity = this->isToBeCutFileIndex(index) ? 0.5f : 1.0f;
	painter->setOpacity(opacity);
	QTreeView::drawRow(painter, options, index);
	painter->setOpacity(1.0f);
}

void LEEditorAssetExplorerTreeView::contextMenuEvent(QContextMenuEvent* event) {
	// record it to make sure you can do something 
	// on the path that you click the mouse
	this->mPathOnMousePos = this->getDirOnMousePosition();
	this->doMenuActionEnableUpdate();
	this->mMainMenu->exec(QCursor::pos());
}

void LEEditorAssetExplorerTreeView::keyPressEvent(QKeyEvent* event) {
	this->mModifierKeyShiftPressed = event->modifiers() & Qt::SHIFT;
	this->mModifierKeyCtrlPressed = event->modifiers() & Qt::CTRL;
	this->mModifierKeyAltPressed = event->modifiers() & Qt::ALT;

	if (this->mModifierKeyCtrlPressed) {
		switch (event->key()) {
		case Qt::Key_X:
			slotCut();
			break;
		case Qt::Key_C:
			slotCopy();
			break;
		case Qt::Key_V:
			slotPaste();
			break;
		case Qt::Key_Y:
			mExecutor->executeSingleRedo();
			break;
		case Qt::Key_Z:
			mExecutor->executeSingleUndo();
			break;
		default:
			break;
		}
	}
	else {
		switch (event->key()) {
		case Qt::Key_Delete:
			slotDelete();
			break;
		default:
			break;
		}
	}
}
