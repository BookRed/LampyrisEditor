#pragma once
#ifndef GameObjectItem_H
#define GameObjectItem_H
//class GameObject;
#include "GameObject.h"
#include "QTreeWidgetItem"
#include <string>
#include <quuid.h>

#define GameObjectItemFlag Qt::ItemFlag::ItemIsEnabled | \
                       Qt::ItemFlag::ItemIsEditable| \
					   Qt::ItemFlag::ItemIsSelectable| \
					   Qt::ItemFlag::ItemIsDragEnabled| \
					   Qt::ItemFlag::ItemIsDropEnabled

class GameObjectItem :public QTreeWidgetItem
{
private:
	QUuid Uid;
	GameObject* RelativeObject;
public:
	/*GameObjectItem(const string& name)
	{
		this->RelativeObject->name = name;
		this->Uid= QUuid::createUuid().toString().toStdString();
		this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
	}*/
	GameObjectItem(GameObject* obj)
	{
		this->setText(0, QString(obj->name.data()));
		this->RelativeObject = obj;
		this->Uid = QUuid::createUuid();
		this->setFlags(GameObjectItemFlag);
	}
	GameObject *GetObjectPtr()
	{
		return RelativeObject;
	}
	QUuid GetGuid()
	{
		return Uid;
	}
	GameObjectItem * child(int index)
	{
		return (GameObjectItem*)QTreeWidgetItem::child(index);
	}
};
#endif // !GameObjectItem_H
