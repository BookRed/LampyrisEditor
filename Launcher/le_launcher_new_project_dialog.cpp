/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_launcher_new_project_dialog.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_launcher_new_project_dialog.h"

void LELauncherNewProjectDialog::doSignalConnection() {
	QObject::connect(this->m_ui.button_cancel, &QPushButton::clicked, [=]() {
		this->m_shouldCreate = false;
		this->close();
		});
	QObject::connect(this->m_ui.button_create, &QPushButton::clicked, [=]() {
		this->m_shouldCreate = true;
		this->close();
		});
	QObject::connect(this->m_ui.button_openPath, &QPushButton::clicked, [=]() {
		this->m_projectInfo.path = QFileDialog::getExistingDirectory(nullptr, "Please select the path of new project");
		this->m_ui.lineEdit_projectPath->setText(this->m_projectInfo.path);
		});
	for (const auto& i : this->m_templateButton) {
		QObject::connect(i, &LELauncherProjectTemplateButton::buttonClickSignal, [=](LEProjectTemplateType type, bool checked) {
			if (checked) {
				// The checked states of the three buttons are exclusive 
				if (type == LEProjectTemplateType::PTT_2D) {
					this->m_templateButton[1]->cancelSelection();
					this->m_templateButton[2]->cancelSelection();
					
				}
				else if (type == LEProjectTemplateType::PTT_3D) {
					this->m_templateButton[0]->cancelSelection();
					this->m_templateButton[2]->cancelSelection();
				}
				else {
					this->m_templateButton[0]->cancelSelection();
					this->m_templateButton[1]->cancelSelection();
				}
				this->m_projectInfo.type = type;
			}
			// if no template selected, button 'create' should be disabled
			this->m_ui.button_create->setEnabled(checked);
		});
	}
}

LELauncherNewProjectDialog::LELauncherNewProjectDialog() :m_shouldCreate(false) {
	this->m_ui.setupUi(this);
	this->m_templateButton[0] = this->m_ui.widget_2d;
	this->m_templateButton[0]->setParent(this);
	this->m_templateButton[1] = this->m_ui.widget_3d;
	this->m_templateButton[1]->setParent(this);
	this->m_templateButton[2] = this->m_ui.widget_demo;
	this->m_templateButton[2]->setParent(this);
	this->m_templateButton[0]->setProjectTemplateContext(
		new QPixmap(QPixmap("res\\launcher\\icon_2d.png").scaled(106, 106)), LEProjectTemplateType::PTT_2D);
	this->m_templateButton[1]->setProjectTemplateContext(
		new QPixmap(QPixmap("res\\launcher\\icon_3d.png").scaled(106, 106)), LEProjectTemplateType::PTT_3D);
	this->m_templateButton[2]->setProjectTemplateContext(
		new QPixmap(QPixmap("res\\launcher\\icon_demo.png").scaled(106, 106)), LEProjectTemplateType::PTT_DEMO);
	this->doSignalConnection();
}

LELauncherNewProjectDialog::~LELauncherNewProjectDialog() {

}

const LENewProjectInfo* LELauncherNewProjectDialog::askNewProjectInfo() {
	if (this->m_shouldCreate) {
		this->m_projectInfo.name = this->m_ui.lineedit_projectName->text();
		return &this->m_projectInfo;
	}
	return nullptr;
}
