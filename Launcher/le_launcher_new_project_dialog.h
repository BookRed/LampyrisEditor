#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_launcher_new_project_dialog.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// Ui Includes
#ifndef LE_LAUNCHER_NEW_PROJECT_DIALOG_H
#define LE_LAUNCHER_NEW_PROJECT_DIALOG_H
// QT Includes
#include <QCheckBox>
#include <QFileDialog>
#include <QPainter>
// Ui Includes
#include "le_launcher_project_template_button.h"
#include "ui_le_editor_new_project.h"

class LELauncherNewProjectDialog:public QDialog {
	Q_OBJECT
private:
	Ui::LENewProjectDialog           m_ui;
	LENewProjectInfo                 m_projectInfo;
	bool                             m_shouldCreate;
	LELauncherProjectTemplateButton* m_templateButton[3];

	void                             doSignalConnection();
public:
	                                 LELauncherNewProjectDialog();

	virtual                         ~LELauncherNewProjectDialog();

	const                            LENewProjectInfo* askNewProjectInfo();
};

#endif // !LE_LAUNCHER_NEW_PROJECT_DIALOG_H