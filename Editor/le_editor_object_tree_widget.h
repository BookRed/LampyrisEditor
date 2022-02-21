#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_object_tree_widget.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_EDITOR_OBJECT_TREE_WIDGET_H
#define LE_EDITOR_OBJECT_TREE_WIDGET_H

// QT Includes
#include <QMenu>
#include <QQueue>
#include <QTreeWidget>
#include <QHeaderView>
#include <QDragMoveEvent>
// LE Includes
#include <Lampyris/Common/le_singleton.h>
#include "le_interface_entity_tree_widget.h"

class LEEntityTreeWidgetItem:public QTreeWidgetItem {
public:
	explicit LEEntityTreeWidgetItem(QTreeWidget* treeview, int type = Type):
	QTreeWidgetItem(treeview, type) {}
	explicit  LEEntityTreeWidgetItem(QTreeWidgetItem* parent, int type = Type):
	QTreeWidgetItem(parent,type) {}
	LEEntity* pEntity{nullptr};
};
class LEEditorObjectTreeList:public QTreeWidget,
                             public Singleton<LEEditorObjectTreeList>,
                             public IEntityTreeWidget {
	Q_OBJECT
	typedef  QList<QTreeWidgetItem*> ItemList_t;
private:
	// declaration of inherent menu items
	QMenu   *        m_pMenuRoot;
	QAction *        m_pActionCopy;
	QAction *        m_pActionPaste;
	QAction *        m_pActionDuplicate;
	QAction *        m_pActionRename;
	QAction *        m_pActionDelete;
	QMenu   *        m_pMenuCreate;
	// related fields for drag & drop operation
	QTreeWidgetItem* m_pDragBeginItem;
	QTreeWidgetItem* m_pDragEndItem;

	ItemList_t       m_copiedItems;
	// resources
	QIcon            m_visibleIcon;
	QIcon            m_lockIcon;
	// others
	ItemList_t       m_needRecoverItems;
public:
	explicit         LEEditorObjectTreeList(QWidget *parent = Q_NULLPTR);

	virtual         ~LEEditorObjectTreeList();

	void             initProperties();                           	

	void             initItemMenu();

	QTreeWidgetItem* createItem(const QString& name, LEEntityTreeWidgetItem* pParent = nullptr);

	QTreeWidgetItem* createItemFromEntity(LEEntity* pEntity);

	// whether rhs is the ancestor of lhs
	bool             isAncestorRelationship(QTreeWidgetItem* lhs, QTreeWidgetItem* rhs) const;

	void             removeUnnecessaryCopiedItem();
protected:
	// qt-native events
	void             dragEnterEvent(QDragEnterEvent *event); 
			        
	void             dragMoveEvent(QDragMoveEvent *event);

	void             dragLeaveEvent(QDragLeaveEvent* event);

	void             dropEvent(QDropEvent *event);
			        
	void             keyPressEvent(QKeyEvent *event);
			        
	void             mousePressEvent(QMouseEvent* event);
			        
	void             mouseReleaseEvent(QMouseEvent* event);
			        
	void             commitData(QWidget * editor);
			        
	void             contextMenuEvent(QContextMenuEvent* event);
public:
	bool             checkEntityNameExist(const QString &name);
			         
	QString          assignEntityName(const QString &name);

	ItemList_t       selectedItems() const;

	QTreeWidgetItem* copyRootItem(QTreeWidgetItem* srcitem);

	void             copyChildItems(QTreeWidgetItem* srcitem, QTreeWidgetItem* destitem);

	void             recordSelectedItems();

	void             copySelectedItems();

	void             pasteSelectedItems();

	void             duplicateSelectedItems();

	// selection operation for entities
	ItemList_t       getAllChildren(QTreeWidgetItem* root);

	ItemList_t       getAllChildren();

	void             cancelSelection();

	void             renameSelectedEntities();

	void             handleSelectionChanged();

	void             deleteAllChidren(QTreeWidgetItem* root);
			         
	void             deleteSelected();
Q_SIGNALS:
	void             clickedCancel();
				     
	void             parentChanged(QTreeWidgetItem* parent, QTreeWidgetItem* child);
public slots:
	void             handleMenuItemEvent();

	void             handleItemChanged(QTreeWidgetItem* item, int column);
	// interface methods
	void             onAddEntity(LEEntity* pEntity) override;
		             
	void             onRenameEntity(LEEntity* pEntity) override;
		             
	void             onDeleteEntity(LEEntity* pEntity) override;
		             
	void             onEntityLockStateChanged(LEEntity* pEntity) override;
		             
	void             onEntityVisibleStateChanged(LEEntity* pEntity) override;

	void             onEntityParentRelationshipChanged(LEEntity* pEntity) override;

	void             onPickupEntity(LEEntity* pEntity) override;
};

#endif // ! LE_EDITOR_OBJECT_TREE_WIDGET_H


