#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_file_explorer_path_tool_bar.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#pragma once
#ifndef LE_FILE_EXPLORER_PATH_TOOL_BAR_H
#define LE_FILE_EXPLORER_PATH_TOOL_BAR_H

#include <QApplication>
#include <QFileInfo>
#include "le_file_line_edit.h"
#include <le_editor_events.h>
#include <le_editor_controls.h>

class LEEditorAssetExplorerPathToolBar:public QWidget {
	Q_OBJECT
private:
	enum class Mode {
		PathEdit,
		PathActions
	};
	QSpacerItem*              mSpaceItem;
	QHBoxLayout*              mLayout;
	QHBoxLayout*              mPathWidgetLayout;
	leLineEdit*               mPathEdit;
	QWidget*                  mPathWidget;
	QLabel*                   mLabelPathIcon;
	int                       mPathNodeCount;
	int                       mFirstDisplayIndex;
	std::vector<QPushButton*> mPathButtons;
	QString                   mCurrentDisplayPath;
	QStringList               mCurrentDisplayPathSplitted;

	void doSwitchMode(const Mode mode) {
		if (mode == Mode::PathEdit) {
			this->mPathEdit->setHidden(false);
			this->mPathEdit->setFocus();
			this->mPathWidget->setHidden(true);
		}
		else {
			this->mPathEdit->setHidden(true);
			this->mPathEdit->clearFocus();
			this->mPathWidget->setHidden(false);
		}
	}

	void doSignalConnection() {
		QObject::connect(this->mPathEdit, &leLineEdit::editingFinished,
			             this,&LEEditorAssetExplorerPathToolBar::slotPathFocusOutOrEditFinished);
		QObject::connect(this->mPathEdit, &leLineEdit::signalFocusOut,
					     this,&LEEditorAssetExplorerPathToolBar::slotPathFocusOutOrEditFinished);
	}

	void doPropertySetting() {
		this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	}

	void doLayoutCreation() {
		// Main Layout
		this->setLayout(this->mLayout);
		this->mLayout->setContentsMargins(0, 0, 0, 0);
		this->mLayout->addWidget(this->mLabelPathIcon);
		this->mLayout->addWidget(this->mPathEdit);
		this->mLayout->addWidget(this->mPathWidget);
		this->mPathEdit->setHidden(true);
		this->mPathWidget->setHidden(false);

		// Left Icon
		QString iconPath = ":/fe/icons/folder_ico.png";
		QPixmap icon(iconPath);
		icon = icon.scaled(25, 25);
		this->mLabelPathIcon->setPixmap(icon);

		// Path Widgets
		this->mPathWidget->setLayout(this->mPathWidgetLayout);
	
		this->mSpaceItem = new QSpacerItem(40, 20, QSizePolicy::Expanding);
		this->mPathWidgetLayout->setContentsMargins(0, 0, 0, 0);
	}

	void doClearPathButtons() {
		for (int i = 0; i < this->mPathButtons.size(); i++) {
			this->mPathWidgetLayout->removeWidget(this->mPathButtons[i]);
			delete this->mPathButtons[i];
		}
		this->mPathWidgetLayout->removeItem(this->mSpaceItem);
		this->mPathButtons.clear();
	}

	void slotPathButtonClicked(const int index) {
		if (index == this->mPathButtons.size() - 1) {
			// we don't have to change the path 
			// when you clicked the last path button
			return;
		}
		QString path;
		// combines the text from the first button to the button clicked 
		for (int i = 0; i <=index; i++) {
			path += (this->mPathButtons[i]->text());
			path += "/";
		}
		// may inform the asset browser to switch the displayed directory
		emit this->signalPathChanged(path);
	}

	void slotOmissionButtonClicked() {
		QMenu* menu = new QMenu;
		menu->addAction(new QAction("1232"));
		menu->exec(QCursor::pos());
	}

	void doPathButtonDisplay() {
		this->doClearPathButtons();
		if (!this->mCurrentDisplayPath.size()) {
			QPushButton* btn = new QPushButton("Disks");
			this->mPathButtons.push_back(btn);
			this->mPathWidgetLayout->addWidget(btn);
			this->mPathEdit->setText("Disks:/");
		}
		else {
			this->doPathButtonCreation();
			this->mPathEdit->setText(this->mCurrentDisplayPath) ;
		}
		this->mSpaceItem = new QSpacerItem(20, 20, QSizePolicy::Expanding);
		this->mPathWidgetLayout->addSpacerItem(this->mSpaceItem);
	}

	void slotPathFocusOutOrEditFinished() {
		this->doSwitchMode(Mode::PathActions);
	}

	void doPathButtonCreation() {
		if (this->mFirstDisplayIndex != 0) {
			QPushButton* btn = new QPushButton("...");
			QObject::connect(btn, &QPushButton::clicked, [=]() {
				this->slotOmissionButtonClicked();
				});
			this->mPathWidgetLayout->addWidget(btn);
		}
		for (int i = this->mFirstDisplayIndex; i < this->mPathNodeCount; i++) {
			QPushButton* btn = new QPushButton(this->mCurrentDisplayPathSplitted[i]);
			this->mPathButtons.push_back(btn);
			this->mPathWidgetLayout->addWidget(btn);
			QObject::connect(btn, &QPushButton::clicked, [=]() {
				this->slotPathButtonClicked(i);
				});
		}

	}

	void doPathNodeCountUpdate() {
		int actualSize = !QFileInfo(this->mCurrentDisplayPath).isDir() ||
						 this->mCurrentDisplayPathSplitted.last().size() == 0 ? this->mCurrentDisplayPathSplitted.size() - 1 : this->mCurrentDisplayPathSplitted.size();
		this->mPathNodeCount = actualSize;
	}

	void doDisplayCountUpdate() {
		int index = -1;
		// here are some situations:

		// 1) given path is "D:/" ,and the split result is ['D:','']
		// we have to discard the latter,so we display 'D:'

		// 2) given path is "D:/ABC/abc.txt",and the split return is ['D:','ABC','abc.txt']
		// we have to discard the last one because it is file's name,so we display 'D:' and 'ABC'

		// 3) given path is "D:/ABC",we display 'D:' and 'ABC'

		int limitedWidth = this->size().width() - 120;
		int usedWidth = 0;
		index = this->mPathNodeCount - 1;
		QFontMetrics fm(this->font());
		for (index; index >= 0; index--) {
			int textWidth = fm.width(this->mCurrentDisplayPathSplitted[index]) + 10;
			if (usedWidth + textWidth < limitedWidth) {
				usedWidth += textWidth;
			}
			else {
				break;
			}
		}
		index++;
		this->mFirstDisplayIndex = index;
	}

	bool isFirstIndexChangedAfterResize() {
		int previousIndex = this->mFirstDisplayIndex;
		this->doDisplayCountUpdate();
		return previousIndex != this->mFirstDisplayIndex;
	}
protected:
	virtual void mousePressEvent(QMouseEvent* event) {
		if (this->mPathEdit->hasFocus()) {
			this->mPathEdit->clearFocus();
		}
		else {
			this->doSwitchMode(Mode::PathEdit);
		}
	}

	virtual void resizeEvent(QResizeEvent* event) {
		if (this->isFirstIndexChangedAfterResize()) {
			this->doPathButtonDisplay();
		}
		QWidget::resizeEvent(event);
	}
signals:
	void signalPathChanged(const QString& );
public:
	LEEditorAssetExplorerPathToolBar():
		mPathEdit	     (new leLineEdit(this)),
		mPathWidget      (new QWidget(this)),
		mLayout          (new QHBoxLayout(this)),
		mLabelPathIcon   (new QLabel(this)),
		mPathWidgetLayout(new QHBoxLayout(this)){
		this->doPropertySetting();
		this->doLayoutCreation();
		this->doSignalConnection();
	}
	~LEEditorAssetExplorerPathToolBar() {

	}

	void setCurrentPath(const QString& path) {
		this->mCurrentDisplayPath = path;
		this->mCurrentDisplayPathSplitted = path.split("/");
		this->doPathNodeCountUpdate();
		this->doDisplayCountUpdate();
		this->doPathButtonDisplay();
		//this->doPathButtonCreation();
	}
};

#endif// !LE_FILE_EXPLORER_PATH_TOOL_BAR_H
