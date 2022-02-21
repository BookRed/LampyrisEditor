#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_lampyris_editor_launcher_window.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_LAMPYRIS_EDITOR_LAUNCHER_WINDOW_H
#define LE_LAMPYRIS_EDITOR_LAUNCHER_WINDOW_H

// QT Includes
#include <QFileDialog>

// LE Includes
#include "../Runtime/Utility/le_xml.h"
#include "../Runtime/Utility/le_path_utility.h"
#include "le_launcher_project_list_item_widget.h"
#
// Ui Includes
#include "le_launcher_new_project_dialog.h"
#include "ui_le_editor_launcher.h"

class LELampyrisEditorLauncherWindow :public QWidget {
	Q_OBJECT
Q_SIGNALS:
	void enterEditorSignal(const QString& projectPath);
private:
	struct ProjectInfo {
		QString projectName;
		QString projectPath;
		QString LastModified;
	};
	Ui::LEEditorLauncher       m_launcherUI;
	LEXml*                     m_xml;
	std::vector<ProjectInfo*>  m_projectInfo;

	// read the project info from xml file
	// and display project info on the list
	void                       doProjectListDisplay();

	// remove entering project info,and insert it to the front.
	void                       doUpdateProjectInfo(const QString& enteringProjectName);

	void                       doAppendProjectInfo(const QString &projectFileName);

	void                       doEnterEditor(const QString& projectPath);

	void                       doSignalConnection();

	void                       createProjectRecordXml(const char* path);

	void                       createProjectThumbnailDir(const char* path);

	void                       createProjectDocumentSaveDir(const char* path);

	void                       createProjectFromProjectInfo(const LENewProjectInfo* pProjectInfo);

	void                       showNewProjectDialog();
public:
	void                       doClearProjectInfo();

	void                       appendProjectInfo(const QString& projectName,
	                                             const QString& projectPath,
	                                             const QString& lastModifed,
	                                             const QString& thumbnailPath);

	                           LELampyrisEditorLauncherWindow();

	                          ~LELampyrisEditorLauncherWindow();
};
#endif // !LE_LAMPYRIS_EDITOR_LAUNCHER_WINDOW_H
