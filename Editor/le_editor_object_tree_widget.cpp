#pragma once
/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_object_tree_widget.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_object_tree_widget.h"
// QT Includes
#include <QDebug>
#include <QPushButton>
#include <QLineEdit>

#include "le_editor_manager.h"
#include "le_interface_property_editor_widget.h"
#include "Runtime/Base/le_assert.h"
#include "Runtime/Base/le_entity.h"
#include "Runtime/LevelManage/le_level.h"
#include "Runtime/LevelManage/le_world.h"

LEEditorObjectTreeList::LEEditorObjectTreeList(QWidget* parent):
	QTreeWidget(parent),
	m_visibleIcon("res\\icon_eye.png"),
	m_lockIcon("res\\icon_lock.png") {
	this->initProperties();
	this->initItemMenu();
	this->expandAll();
	connect(this, &LEEditorObjectTreeList::currentItemChanged, this,
	        [=](QTreeWidgetItem* current, QTreeWidgetItem* previous)
	        {
		        auto item = current;
		        if (!item) return;
		        auto item_parent = item->parent();
	        });

	connect(this, &LEEditorObjectTreeList::itemSelectionChanged,
	        this, &LEEditorObjectTreeList::handleSelectionChanged);

	connect(this, &LEEditorObjectTreeList::itemChanged,
		   this, &LEEditorObjectTreeList::handleItemChanged);
}

LEEditorObjectTreeList::~LEEditorObjectTreeList() {
}

void LEEditorObjectTreeList::initProperties() {
	// enable drag & drop
	this->setDragEnabled(true);
	this->setAcceptDrops(true);
	this->setDefaultDropAction(Qt::MoveAction);
	this->setDragDropMode(InternalMove);
	// columns: entity name, visible, lock
	this->setColumnCount(3);
	// header
	// this->header()->hide();
	this->header()->hide();
	this->header()->setStretchLastSection(false);
	this->header()->setSectionResizeMode(0, QHeaderView::Stretch);
	this->header()->setSectionResizeMode(1, QHeaderView::Fixed);
	this->header()->setSectionResizeMode(2, QHeaderView::Fixed);
	this->setColumnWidth(1, 15);
	this->setColumnWidth(2, 15);
	// others
	this->setMouseTracking(true);
	this->setEditTriggers(SelectedClicked);
	this->setSelectionMode(ExtendedSelection);
}

void LEEditorObjectTreeList::initItemMenu() {
	this->m_pMenuRoot = new QMenu(this);
	this->m_pMenuRoot->setMinimumSize(QSize(120, 10));

	connect(m_pMenuRoot, &QMenu::triggered,
		this, &LEEditorObjectTreeList::handleMenuItemEvent);

	this->m_pActionCopy = new QAction(QStringLiteral("Copy"), this);
	this->m_pActionPaste = new QAction(QStringLiteral("Paste"), this);
	this->m_pActionDuplicate = new QAction(QStringLiteral("Duplicate"), this);
	this->m_pActionRename = new QAction(QStringLiteral("Rename"), this);
	this->m_pActionDelete = new QAction(QStringLiteral("Delete"), this);

	this->m_pActionCopy->setEnabled(false);
	this->m_pActionRename->setEnabled(false);
	this->m_pActionDuplicate->setEnabled(false);
	this->m_pActionDelete->setEnabled(false);

	this->m_pMenuCreate = new QMenu(QStringLiteral("Create"), this->m_pMenuRoot);

	this->m_pMenuRoot->addAction(this->m_pActionCopy);
	this->m_pMenuRoot->addAction(this->m_pActionPaste);
	this->m_pMenuRoot->addAction(this->m_pActionDuplicate);

	this->m_pMenuRoot->addSeparator();
	this->m_pMenuRoot->addAction(this->m_pActionRename);
	this->m_pMenuRoot->addAction(this->m_pActionDelete);

	this->m_pMenuRoot->addSeparator();
	this->m_pMenuRoot->addMenu(this->m_pMenuCreate);

	QObject::connect(this->m_pActionCopy, &QAction::triggered, this, &LEEditorObjectTreeList::handleMenuItemEvent);
	QObject::connect(this->m_pActionPaste, &QAction::triggered, this, &LEEditorObjectTreeList::handleMenuItemEvent);
	QObject::connect(this->m_pActionDuplicate, &QAction::triggered, this, &LEEditorObjectTreeList::handleMenuItemEvent);
	QObject::connect(this->m_pActionRename, &QAction::triggered, this, &LEEditorObjectTreeList::handleMenuItemEvent);
	QObject::connect(this->m_pActionDelete, &QAction::triggered, this, &LEEditorObjectTreeList::handleMenuItemEvent);

	/* create objects */
	auto primitiveInfoList = LELevel::getBuildInPrimitiveInfoList();
	std::unordered_map<std::string, QMenu*> memuMap;

	auto primitiveCreateFunc = [](int ID){
		LEWorld::getInstance()->getActiveLevel()->createPrimitiveEntity(ID);
	};
	for(auto& primitiveInfo: primitiveInfoList) {
		QAction* pAction = nullptr;
		if(primitiveInfo.category.empty()) {
			pAction = new QAction(QString::fromStdString(primitiveInfo.name), this->m_pMenuCreate);
			this->m_pMenuCreate->addAction(pAction);
		} else {
			QMenu* pMenu = nullptr;
			pAction = new QAction(QString::fromStdString(primitiveInfo.name), pMenu);
			if(memuMap.contains(primitiveInfo.category)) {
				pMenu = memuMap[primitiveInfo.category];
				
			} else {
				pMenu = new QMenu(QString::fromStdString(primitiveInfo.category), this);
				this->m_pMenuCreate->addMenu(pMenu);
				memuMap[primitiveInfo.category] = pMenu;
			}
			pMenu->addAction(pAction);
		}
		if(pAction) {
			QObject::connect(pAction,&QAction::triggered, [primitiveInfo]() {
				LEWorld::getInstance()->getActiveLevel()->createPrimitiveEntity(primitiveInfo.ID);
			});
		}
	}
}

QTreeWidgetItem* LEEditorObjectTreeList::createItem(const QString& name, LEEntityTreeWidgetItem* pParent) {
	LEEntityTreeWidgetItem* pItem = nullptr;
	if(pParent == nullptr) {
		pItem = new LEEntityTreeWidgetItem(this);
	}
	else {
		pItem = new LEEntityTreeWidgetItem(pParent);
	}
	pItem->setText(0, name);
	this->setItemWidget(pItem, 1, new QPushButton(this->m_visibleIcon, ""));
	this->setItemWidget(pItem, 2, new QPushButton(this->m_lockIcon, ""));
	pItem->setFlags(Qt::ItemFlag::ItemIsEnabled |
		            Qt::ItemFlag::ItemIsEditable |
		            Qt::ItemFlag::ItemIsSelectable |
		            Qt::ItemFlag::ItemIsDragEnabled |
		            Qt::ItemFlag::ItemIsDropEnabled);
	// ensure the parent relationship of entity
	LEEntity* pEntity = LEWorld::getInstance()->getActiveLevel()->createEntity(name.toStdString());
	if(pParent) {
		LE_ASSERT_MSG(pParent->pEntity != nullptr, "Associated entity can not be nullptr");
		pEntity->setParent(pParent->pEntity);
	}
	pEntity->pTreeItem = pItem;
	pItem->pEntity = pEntity;
	return pItem;
}

QTreeWidgetItem* LEEditorObjectTreeList::createItemFromEntity(LEEntity* pEntity) {
	LEEntityTreeWidgetItem* pItem = nullptr;
	if (pEntity->getParent() == nullptr) {
		pItem = new LEEntityTreeWidgetItem(this);
	}
	else {
		pItem = new LEEntityTreeWidgetItem(pEntity->getParent()->pTreeItem);
	}
	pItem->setText(0, QString::fromStdString(pEntity->getName()));
	this->setItemWidget(pItem, 1, new QPushButton(this->m_visibleIcon, ""));
	this->setItemWidget(pItem, 2, new QPushButton(this->m_lockIcon, ""));
	pItem->setFlags(Qt::ItemFlag::ItemIsEnabled |
		Qt::ItemFlag::ItemIsEditable |
		Qt::ItemFlag::ItemIsSelectable |
		Qt::ItemFlag::ItemIsDragEnabled |
		Qt::ItemFlag::ItemIsDropEnabled);
	pEntity->pTreeItem = pItem;
	pItem->pEntity = pEntity;
	return pItem;
}

bool LEEditorObjectTreeList::isAncestorRelationship(QTreeWidgetItem* lhs, QTreeWidgetItem* rhs) const {
	QTreeWidgetItem* pCur = lhs;
	while(pCur) {
		pCur = pCur->parent();
		if(pCur == rhs) {
			return true;
		}
	}
	return false;
}

void LEEditorObjectTreeList::removeUnnecessaryCopiedItem() {
	uint32_t validCount = this->m_copiedItems.size();
	uint32_t swapCount  = 0;
	bool     loop       = true;
	for (uint32_t i = 0; i < validCount && loop;) {
		for (uint32_t j = i + 1; j < validCount;) {
			auto& pItemLeft = this->m_copiedItems[i];
			auto& pItemRight = this->m_copiedItems[j];
			if (this->isAncestorRelationship(pItemLeft, pItemRight)) {
				// left item should be removed
				std::swap(pItemLeft, this->m_copiedItems[this->m_copiedItems.size() - 1 - swapCount++]);
				if(--validCount == 1) {
					loop = false;
				}
				continue;
			}
			if (this->isAncestorRelationship(pItemRight, pItemLeft)) {
				// right item should be removed
				std::swap(pItemRight, this->m_copiedItems[this->m_copiedItems.size() - 1 - swapCount++]);
				if (--validCount == 1) {
					loop = false;
				}
				else {
					continue;
				}
			}
			j++;
		}
		i++;
	}
	ItemList_t newList(this->m_copiedItems.begin(), this->m_copiedItems.begin() + validCount);
	this->m_copiedItems = std::move(newList);
}

// qt-native events
void LEEditorObjectTreeList::dragEnterEvent(QDragEnterEvent* event) {
	this->m_needRecoverItems.clear();
	auto items = this->selectedItems();
	for (auto& pItem : items) {
		this->m_needRecoverItems.append(this->getAllChildren(pItem));
		this->m_needRecoverItems.append(pItem);
	}
	for (auto& pItem : this->m_needRecoverItems) {
		this->removeItemWidget(pItem, 1);
		this->removeItemWidget(pItem, 2); 
	}
	QTreeWidget::dragEnterEvent(event);
}

void LEEditorObjectTreeList::dragMoveEvent(QDragMoveEvent* event) {
	QTreeWidget::dragMoveEvent(event);
}

void LEEditorObjectTreeList::dragLeaveEvent(QDragLeaveEvent* event) {
	QTreeWidget::dragLeaveEvent(event);
	auto items = this->selectedItems();
	for (auto& pItem : this->m_needRecoverItems) {
		this->setItemWidget(pItem, 1, new QPushButton(this->m_visibleIcon, ""));
		this->setItemWidget(pItem, 2, new QPushButton(this->m_lockIcon, ""));
	}
}

void LEEditorObjectTreeList::dropEvent(QDropEvent* event) {
	this->m_pDragEndItem = itemAt(event->pos());
	LEEntity* pParent = this->m_pDragEndItem ?static_cast<LEEntityTreeWidgetItem*>(this->m_pDragEndItem)->pEntity:nullptr;
	if (this->m_pDragBeginItem && this->m_pDragEndItem) {
		// parent changed event
		// m_dragBeinItem's parent is set to the m_pDragEndItem
		this->expandItem(this->m_pDragBeginItem);
		this->expandItem(this->m_pDragEndItem);
#if defined(LE_EDITOR)
		LEEntity* pEntity = static_cast<LEEntityTreeWidgetItem*>(this->m_pDragBeginItem)->pEntity;
		LEEntity* pEntityParent = static_cast<LEEntityTreeWidgetItem*>(this->m_pDragEndItem)->pEntity;
		this->disableNotify();
		pEntity->setParent(pEntityParent);
		this->enableNotify();
#endif // LE_EDITOR
		emit parentChanged(this->m_pDragEndItem, this->m_pDragBeginItem);
	}
	auto items = this->selectedItems();
	QTreeWidget::dropEvent(event);

	for (auto& pItem : this->m_needRecoverItems) {
		this->setItemWidget(pItem, 1, new QPushButton(this->m_visibleIcon, ""));
		this->setItemWidget(pItem, 2, new QPushButton(this->m_lockIcon, ""));
		static_cast<LEEntityTreeWidgetItem*>(pItem)->pEntity->setParent(pParent);
	}
}

void LEEditorObjectTreeList::keyPressEvent(QKeyEvent* event) {
	if (!hasFocus()) {
		return;
	}
	if (event->key() == Qt::Key_C && event->modifiers() == (Qt::ControlModifier)) {
		this->recordSelectedItems();
	}
	else if (event->key() == Qt::Key_V && event->modifiers() == (Qt::ControlModifier)) {
		this->copySelectedItems();
	}
	else if (event->key() == Qt::Key_A && event->modifiers() == (Qt::ControlModifier)) {
		this->selectAll();
	}
	else if (event->key() == Qt::Key_Delete) {
		this->deleteSelected();
	}
}

void LEEditorObjectTreeList::mousePressEvent(QMouseEvent* event) {
	auto index = this->indexAt(event->pos());
	if (index.row() == -1) {
		this->m_pDragBeginItem = nullptr;
		emit this->clickedCancel();
		this->setCurrentIndex(index);
#if defined(LE_EDITOR)
		LampyrisEditorManager::getInstance()->getPropertyEditorWidget().setIdle();
#endif // £¡LE_EDITOR
	}
	else {
		LEEntityTreeWidgetItem* pItem = static_cast<LEEntityTreeWidgetItem*>(this->itemAt(event->pos()));
		this->m_pDragBeginItem = pItem;
		// notify property editor
#if defined(LE_EDITOR)
		LampyrisEditorManager::getInstance()->getPropertyEditorWidget().setEntity(pItem->pEntity);
#endif // £¡LE_EDITOR
	}
	QTreeWidget::mousePressEvent(event);
}

void LEEditorObjectTreeList::mouseReleaseEvent(QMouseEvent* event) {
	QTreeWidget::mouseReleaseEvent(event);
}

void LEEditorObjectTreeList::contextMenuEvent(QContextMenuEvent* event) {
	this->m_pMenuRoot->popup(QCursor::pos());
	event->accept();
}

void LEEditorObjectTreeList::commitData(QWidget* editor) {
	auto edit = (QLineEdit*)editor;
	QString strText = edit->text();
	if (strText.isEmpty()) {
		// If the text is empty, do nothing
		return;
	}
	//QTreeWidgetItem * obj = (QTreeWidgetItem*)(editor->parent()->parent());
	QTreeWidget::commitData(editor);
}

void LEEditorObjectTreeList::deleteSelected() {
	QList<QTreeWidgetItem*> List = this->selectedItems();
	this->cancelSelection();
	while (!List.empty()) {
		QTreeWidgetItem* currentItem = List.front();
		if (currentItem == Q_NULLPTR) {
			return;
		}
		QList<QTreeWidgetItem*> allitems = getAllChildren(currentItem);
		for (int k = 0; k < allitems.count(); k++) {
			List.removeAt(List.indexOf(allitems[k], 0));
		}
		// setCurrentIndex(indexFromItem(currentItem));
		// delete current node directly,if it didn't have parent node.
		if constexpr (true) {
			//currentItem->parent() == Q_NULLPTR) {
			int idx = this->m_copiedItems.indexOf(currentItem);
			if (idx != -1) {
				currentItem->setHidden(true);
			}
			else {
				LE_SAFE_RELEASE(currentItem);
			}
		}
		else {
			uint32_t count = currentItem->childCount();
			for (int j = 0; j < count; j++) {
				currentItem->removeChild(currentItem->child(0));
			}
			int i = currentIndex().row();
			auto deleteItem = currentItem->parent()->takeChild(currentIndex().row());
			QString str = deleteItem->text(0);
			LE_SAFE_RELEASE(deleteItem);
		}
		List.pop_front();
	}
}

void LEEditorObjectTreeList::handleMenuItemEvent() {
	QObject* sender = QObject::sender();
	// Inherent Menu Items 
	if (sender == this->m_pActionCopy) {
		this->recordSelectedItems();
	}
	else if (sender == this->m_pActionPaste) {
		this->copySelectedItems();
	}
	else if (sender == this->m_pActionDuplicate) {
		this->duplicateSelectedItems();
	}
	else if (sender == this->m_pActionRename) {
		this->renameSelectedEntities();
	}
	else if (sender == this->m_pActionDelete) {
		this->deleteSelected();
	}
	else {
		/* unknown sender */
	}
}

void LEEditorObjectTreeList::handleItemChanged(QTreeWidgetItem* item, int column) {
	auto* pItem = static_cast<LEEntityTreeWidgetItem*>(item);
	if(pItem->pEntity) {
		this->disableNotify();
		pItem->pEntity->setName(item->text(0).toStdString());
		this->enableNotify();
	}
}

void LEEditorObjectTreeList::recordSelectedItems() {
	for (uint32_t i = 0; i < this->m_copiedItems.size(); i++) {
		if (this->m_copiedItems[i]->isHidden()) {
			LE_SAFE_RELEASE(this->m_copiedItems[i]);
		}
	}
	this->m_copiedItems = selectedItems();
}

void LEEditorObjectTreeList::copySelectedItems() {
	LE_ASSERT(this->m_copiedItems.count() != 0);
	this->removeUnnecessaryCopiedItem();
	// if no nodes selected, paste nodes on the top level.
	if (selectedItems().count() == 0) {
		for (uint32_t i = 0; i < m_copiedItems.count(); i++) {
			auto root = this->createItem(this->m_copiedItems[i]->text(0));
			this->addTopLevelItem(root);
			this->copyChildItems(m_copiedItems[i], root);
		}
	}
	else {
		auto firstItem = selectedItems().first();
		auto parent = firstItem->parent();
		for (uint32_t i = 0; i < m_copiedItems.count(); i++) {
			auto root = this->createItem(this->m_copiedItems[i]->text(0), static_cast<LEEntityTreeWidgetItem*>(parent));
			if (parent) {
				parent->insertChild(0, root);
			}
			else {
				this->addTopLevelItem(root);
			}
			this->copyChildItems(this->m_copiedItems[i], root);
		}
	}
	this->expandAll();
}

void LEEditorObjectTreeList::pasteSelectedItems() {

}

void LEEditorObjectTreeList::duplicateSelectedItems() {
	this->m_copiedItems.clear();
	auto pItem = this->m_pDragBeginItem;
	LE_ASSERT(pItem != nullptr);
	this->m_copiedItems.append(pItem);
	this->copySelectedItems();
	this->m_copiedItems.clear();
}

LEEditorObjectTreeList::ItemList_t LEEditorObjectTreeList::getAllChildren() {
	QList<QTreeWidgetItem*> itemlist;

	return itemlist;
}

void LEEditorObjectTreeList::renameSelectedEntities() {
	LE_ASSERT(this->selectedItems().size() == 1);
	// only show the edit at the first selected item
	this->editItem(this->selectedItems().front());
}

void LEEditorObjectTreeList::handleSelectionChanged() {
	const uint32_t selectionCount = this->selectedItems().size();
	const bool     hasSelection   = selectionCount != 0;
	const uint32_t copiedCount    = this->m_copiedItems.size();
	const bool     hasCopied      = copiedCount != 0;
	const bool     hasCoveredItem = this->m_pDragBeginItem != nullptr;
	this->m_pActionCopy->setEnabled(hasSelection);
	this->m_pActionPaste->setEnabled(hasCopied);
	this->m_pActionRename->setEnabled(hasSelection);
	this->m_pActionDuplicate->setEnabled(hasSelection || hasCoveredItem);
	this->m_pActionDelete->setEnabled(hasSelection);
}

void LEEditorObjectTreeList::copyChildItems(QTreeWidgetItem* oldRoot,
                                            QTreeWidgetItem* newRoot) {
	int childcount = oldRoot->childCount();
	for (int i = 0; i < childcount; i++) {
		auto childItem = this->createItem(oldRoot->child(i)->text(0), static_cast<LEEntityTreeWidgetItem*>(newRoot));
		// newRoot->insertChild(0, childItem);
		this->copyChildItems(oldRoot->child(i), childItem);
	}
}

QTreeWidgetItem* LEEditorObjectTreeList::copyRootItem(QTreeWidgetItem* srcitem) {
	auto item = new QTreeWidgetItem;
	item->setFlags(Qt::ItemFlag::ItemIsEnabled |
		           Qt::ItemFlag::ItemIsEditable |
		           Qt::ItemFlag::ItemIsSelectable |
		           Qt::ItemFlag::ItemIsDragEnabled |
		           Qt::ItemFlag::ItemIsDropEnabled);
	item->setText(0, srcitem->text(0));
	this->addTopLevelItem(item);
	return item;
}

QList<QTreeWidgetItem*> LEEditorObjectTreeList::getAllChildren(QTreeWidgetItem* root) {
	QList<QTreeWidgetItem*> itemlist;
	QQueue<QTreeWidgetItem*> itemqueue;
	itemqueue.push_back(root);
	while (!itemqueue.empty()) {
		QTreeWidgetItem* item = itemqueue.front();
		itemqueue.pop_front();
		for (int i = 0; i < item->childCount(); i++) {
			itemlist.push_back((item->child(i)));
			itemqueue.push_back(item->child(i));
		}
	}
	return itemlist;
}

void LEEditorObjectTreeList::deleteAllChidren(QTreeWidgetItem* root) {
}

void LEEditorObjectTreeList::cancelSelection() {
	QList<QTreeWidgetItem*> List = selectedItems();
	for (int i = 0; i < List.count(); i++) {
		List[i]->setSelected(false);
	}
	auto index = this->indexAt(QPoint(-1, -1));
	if (index.row() == -1) {
		setCurrentIndex(index);
	}
}

bool LEEditorObjectTreeList::checkEntityNameExist(const QString& name) {
	auto it_this = new QTreeWidgetItemIterator(this);
	while (**it_this) {
		if (name == (**it_this)->text(0)) {
			return true;
		}
		(*it_this).operator++();
	}
	return false;
}

QString LEEditorObjectTreeList::assignEntityName(const QString& name) {
	QString formatedName;
	int count = 1;
	if (!this->checkEntityNameExist(name)) {
		return name;
	}
	formatedName = name + "(%1)";
	while (true) {
		if (!checkEntityNameExist(formatedName.arg(count++)))
			return formatedName.arg(count - 1);
	}
}

QList<QTreeWidgetItem*> LEEditorObjectTreeList::selectedItems() const {
	QList<QTreeWidgetItem*> newlist;
	QList<QTreeWidgetItem*> oldlist = QTreeWidget::selectedItems();
	for (auto iter = oldlist.begin(); iter != oldlist.end(); ++iter) {
		newlist.append(iter.i->t());
	}
	return newlist;
}

// interface methods
void LEEditorObjectTreeList::onAddEntity(LEEntity* pEntity) {
	if (!checkNotifyEnabled()) return;
	// create item and activate edit mode  
	QTreeWidgetItem* pItem = this->createItemFromEntity(pEntity);
	this->editItem(pItem);
	// associate the entity with tree item
	pEntity->pTreeItem = pItem;
}

void LEEditorObjectTreeList::onRenameEntity(LEEntity* pEntity) {
	if (!checkNotifyEnabled()) return;
	LE_ASSERT_MSG(pEntity->pTreeItem != nullptr, "Associated tree item can not be nullptr");
	pEntity->pTreeItem->setText(0,QString::fromStdString(pEntity->getName()));
}

void LEEditorObjectTreeList::onDeleteEntity(LEEntity* pEntity) {
	if (!checkNotifyEnabled()) return;
}

void LEEditorObjectTreeList::onEntityLockStateChanged(LEEntity* pEntity) {
	if (!checkNotifyEnabled()) return;
}

void LEEditorObjectTreeList::onEntityVisibleStateChanged(LEEntity* pEntity) {
	if (!checkNotifyEnabled()) return;
}

void LEEditorObjectTreeList::onEntityParentRelationshipChanged(LEEntity* pEntity) {
	if (!checkNotifyEnabled()) return;
}

void LEEditorObjectTreeList::onPickupEntity(LEEntity* pEntity) {
	if (!checkNotifyEnabled()) return;
	this->cancelSelection();
	this->setItemSelected(pEntity->pTreeItem,true);
}
