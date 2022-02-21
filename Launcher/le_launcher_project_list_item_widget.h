#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor/Launcher
 * \File:    le_launcher_project_list_item_widget.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_LAUNCHER_PROJECT_LIST_ITEM_WIDGET
#define LE_LAUNCHER_PROJECT_LIST_ITEM_WIDGET

// LE Includes
#include "../Common/le_global.h"

// QT Includes
#include <QFile>
#include <QDate>
#include <QLabel>
#include <QLayout>

// The layout of project list item:

//   -------------
//   |			 |  Project name
//   | thumbnail |	FilePath: D:/xxx/xxx/xxx.leproj
//   |			 |	Last Modified: 20xx/xx/xx xx:xx
//   -------------


class LELauncherProjectListItemWidget:public QWidget {
	Q_OBJECT
private:
	QLabel*  m_label_projectName;
	QLabel*  m_label_projectPath;
	QLabel*  m_label_lastModified;
	QWidget* m_widget_thumbnail;

	void doLayoutCreation() {
		QHBoxLayout* hLayout = new QHBoxLayout(this);
		QVBoxLayout* vLayout = new QVBoxLayout(this);
		QWidget* widget_Right = new QWidget(this);
		this->setLayout(hLayout);
		hLayout->addWidget(this->m_widget_thumbnail);
		hLayout->addWidget(widget_Right);
		widget_Right->setLayout(vLayout);
		vLayout->addWidget(this->m_label_projectName);
		vLayout->addWidget(this->m_label_projectPath);
		vLayout->addWidget(this->m_label_lastModified);
	}
public:
	LELauncherProjectListItemWidget(const QString& projectName,
		                            const QString& filePath,
									const QString& lastModifed,
		                            const QString& thumbnailPath): 
		m_label_projectName(new QLabel(projectName, this)),
		m_label_projectPath(new QLabel(filePath, this)),
		m_label_lastModified(new QLabel(lastModifed, this)),
	    m_widget_thumbnail(new QWidget(this)) {
		
		this->doLayoutCreation();
		this->m_widget_thumbnail->setStyleSheet(QString("border-image: url(%1);").arg(thumbnailPath));
		this->m_widget_thumbnail->setFixedSize(80, 80);
	}

	~LELauncherProjectListItemWidget() {

	}

	LE_FORCEINLINE QString getProjectName() const {
		return this->m_label_projectName->text();
	}

	LE_FORCEINLINE QString getProjectPath() const {
		return this->m_label_projectPath->text();
	}
};
#endif // !LE_LAUNCHER_PROJECT_LIST_ITEM_WIDGET


