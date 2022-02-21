/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_lampyris_editor_launcher_window.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_lampyris_editor_launcher_window.h"
#include "../Runtime/Utility/le_file.h"
// QT Includes
#include <QMessageBox>
#include "Editor/le_lampyris_editor_main_window.h"

void LELampyrisEditorLauncherWindow::doProjectListDisplay() {
	// The template of project info 
	/*
		<?xml version="1.0" encoding="UTF-8"?>
		<LEProjectXML>
			<LEProjectCount>3</LEProjectCount>
			<LEProject>
				<ProjectName>Game 1</ProjectName>
				<ProjectPath>C:/xxxx</FilePath>
				<LastModified>YYYY/MM/DD hh:mm</LastModified>
			</LEProject>
			<LEProject>
				<ProjectName>Game 2</ProjectName>
				<ProjectPath>C:/xxxx</FilePath>
				<LastModified>YYYY/MM/DD hh:mm9</LastModified>
			</LEProject>
			...
		</LEProjectXML>
	*/
	std::string path = LEPathUtility::getAppDataSavePath();
	if (!LEFile::isExists(path)) {
		this->createProjectDocumentSaveDir(path.c_str());
	}
	// get the full path where 'project.xml' located at.
	std::string xmlPath = (path + "le_project.xml");
	if(!LEFile::isExists(xmlPath)) {
		this->createProjectRecordXml(xmlPath.c_str());
	}
	std::string thumbnailPath = path + "thumbnail";
	if (!LEFile::isExists(thumbnailPath)) {
		this->createProjectThumbnailDir(thumbnailPath.c_str());
	}
	auto [xml, errorStr] = LEXml::open(xmlPath.c_str());
	this->m_xml = xml;
	LE_ASSERT_MSG(xml != nullptr, L"Read le_project.xml failed!");
	// then, get the count of projects,
	// and finally, we read each project info,and display them on the listWidget 
	xml->moveToFirstChild("LEProjectCount");
	int projectCount = xml->readInt();
	xml->moveToParent();
	xml->moveToFirstChild("LEProject");
	for (uint32_t i = 0; i < projectCount; i++) {
		QString projectName   = xml->readStringElement("ProjectName");
		QString projectPath   = xml->readStringElement("ProjectPath");
		QString LastModified  = xml->readStringElement("LastModified");
		QString thumbnailPath = QString::fromStdString(path) + 
			                    QString("\\thumbnail\\") + projectName + ".png";
		this->appendProjectInfo(projectName, 
		                        projectPath, 
		                        LastModified, 
		                        thumbnailPath);
		xml->moveToNext();
	}
}

void LELampyrisEditorLauncherWindow::doUpdateProjectInfo(const QString& enteringProjectName) {
	this->m_xml->moveToRoot();
	this->m_xml->moveToFirstChild("LEProjectCount");
	int projectCount = this->m_xml->readInt();
	this->m_xml->moveToParent();

	for (uint32_t i = 0; i < projectCount; i++) {
		this->m_xml->moveToFirstChild("LEProject");
		const LEChar* projectName = this->m_xml->readStringElement("ProjectName");
		const LEChar* projectPath = this->m_xml->readStringElement("ProjectPath");
		const LEChar* LastModified = this->m_xml->readStringElement("LastModified");
		if (enteringProjectName == projectName) {
			this->m_xml->removeCurrent();
			this->m_xml->moveToFirstChild("LEProjectCount");
			this->m_xml->insertAfterCurrent("LEProject",nullptr,true);
			this->m_xml->insertBack("ProjectName", projectName,false);
			this->m_xml->insertBack("ProjectPath", projectPath,false);
			this->m_xml->insertBack("LastModified", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm").toStdString().c_str(),false);
			break;
		}
		this->m_xml->moveToNext();
	}
}

void LELampyrisEditorLauncherWindow::doAppendProjectInfo(const QString& projectFileName) {
	auto fileInfo = QFileInfo(projectFileName);
	QString folderPath = fileInfo.absoluteFilePath();
	QString fileBaseName = fileInfo.baseName();
	this->m_xml->moveToRoot();
	this->m_xml->moveToFirstChild("LEProjectCount");
	int projectCount = this->m_xml->readInt();
	char buffer[64];
	::itoa(projectCount + 1,buffer,10);
	this->m_xml->setCurrentElement(buffer);
	this->m_xml->insertAfterCurrent("LEProject", nullptr, true);
	this->m_xml->insertBack("ProjectName", fileBaseName.toStdString().c_str(), false);
	this->m_xml->insertBack("ProjectPath", folderPath.toStdString().c_str(), false);
	this->m_xml->insertBack("LastModified", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm").toStdString().c_str(), false);
	this->m_xml->save();
}

void LELampyrisEditorLauncherWindow::doEnterEditor(const QString& projectPath) {
	// hide this widget,and create main window
	this->hide();
	this->m_xml->save();
	// emit this->enterEditorSignal(projectPath);
	LELampyrisEditorMainWindow* mainWindow = new LELampyrisEditorMainWindow;
	mainWindow->showMaximized();
}

void LELampyrisEditorLauncherWindow::doSignalConnection() {
	// enter the editor after item clicked
	// before it, we need to get the project path of clicked project info item
	QObject::connect(this->m_launcherUI.List_Project, &QListWidget::itemClicked, 
		[=](QListWidgetItem* item) {
	    auto widget = static_cast<LELauncherProjectListItemWidget*>(this->m_launcherUI.List_Project->itemWidget(item));	
	    // remember the entering project's 'lastModified' must be set to current time. 
	    this->doUpdateProjectInfo(widget->getProjectName());
	    this->doEnterEditor(widget->getProjectPath());}
	);

	// ask the path of project file
	QObject::connect(this->m_launcherUI.Button_OpenProject,&QPushButton::clicked,
		[=]() {
	    QString path = QFileDialog::getOpenFileName(Q_NULLPTR, 
	                                                "Please select your project file", 
	                                                QString(), 
	                                                "Lampyris Project (*.leproj)");
	    // whether it already exists
	    for (uint32_t i = 0; i < this->m_projectInfo.size(); i++) {
	        if (this->m_projectInfo[i]->projectPath == path) {
	            auto result = QMessageBox::information(Q_NULLPTR,
	                                                   "Lampyris Game Engine Message",
	                                                   "The project you opened is already exists, open it?");
	    
	            if (result == QMessageBox::StandardButton::Yes) {
	                this->doAppendProjectInfo(path);
	            }
	            this->doEnterEditor(this->m_projectInfo[i]->projectPath);
	            return;
	        }
	    }
	    this->doAppendProjectInfo(path);
	});

	QObject::connect(this->m_launcherUI.Button_NewProject, &QPushButton::clicked, 
		this, &LELampyrisEditorLauncherWindow::showNewProjectDialog);

}

void LELampyrisEditorLauncherWindow::createProjectRecordXml(const char* path) {
	LE_ASSERT_MSG(LEFile::createFile(path), "create project record file failed!");
	LEFile file(path);
	file.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	                  "<LEProjectXML>\n\t"
		                   "<LEProjectCount>0</LEProjectCount>\n"
		              "</LEProjectXML>");
	file.flush();
	file.close();
}

void LELampyrisEditorLauncherWindow::createProjectThumbnailDir(const char* path) {
	LE_ASSERT_MSG(LEFile::create(path), "create project thumbnail directory failed!");
}

void LELampyrisEditorLauncherWindow::createProjectDocumentSaveDir(const char* path) {
	LE_ASSERT_MSG(LEFile::create(path), "create project document directory failed!");
}

void LELampyrisEditorLauncherWindow::createProjectFromProjectInfo(const LENewProjectInfo* pProjectInfo) {
	this->doEnterEditor(pProjectInfo->path);
}

void LELampyrisEditorLauncherWindow::showNewProjectDialog() {
	LELauncherNewProjectDialog dialog;
	dialog.exec();
	const LENewProjectInfo* pProjectInfo = dialog.askNewProjectInfo();
	if(pProjectInfo != nullptr) {
		// should create new project
		this->createProjectFromProjectInfo(pProjectInfo);
	}
}

void LELampyrisEditorLauncherWindow::doClearProjectInfo() {
	this->m_launcherUI.List_Project->clear();
	for (uint32_t i = 0; i < this->m_projectInfo.size(); i++) {
		LE_SAFE_RELEASE(this->m_projectInfo[i]);
	}
}

void LELampyrisEditorLauncherWindow::appendProjectInfo(const QString& projectName, const QString& projectPath,
	const QString& lastModifed, const QString& thumbnailPath) {
	auto* item = new QListWidgetItem;
	item->setSizeHint(QSize(200, 100));
	auto w = new LELauncherProjectListItemWidget(projectName,
	                                             projectPath, 
	                                             lastModifed, 
	                                             thumbnailPath);
	this->m_launcherUI.List_Project->addItem(item);
	this->m_launcherUI.List_Project->setItemWidget(item, w);

	this->m_projectInfo.push_back(new ProjectInfo{ projectName,
		projectPath,
		lastModifed });
}

LELampyrisEditorLauncherWindow::LELampyrisEditorLauncherWindow() {
	this->setWindowFlags(this->windowFlags() &
		~Qt::WindowMaximizeButtonHint & 
		~Qt::WindowMinimizeButtonHint);
	this->m_launcherUI.setupUi(this);
	this->m_launcherUI.List_Project->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	this->doProjectListDisplay();
	this->doSignalConnection();
}

LELampyrisEditorLauncherWindow::~LELampyrisEditorLauncherWindow() {

}
