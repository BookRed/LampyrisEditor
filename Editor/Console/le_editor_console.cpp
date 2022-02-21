/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_console.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_console.h"
#include <Lampyris/Common/le_safe_release.h>
// QT Includes
#include <QMenu>
#include <QKeyEvent>
#include <QLineEdit>
#include <QVBoxLayout>

void LEEditorConsole::initLayout() {
	this->setLayout(new QVBoxLayout(this));
	this->layout()->setContentsMargins(2, 0, 2, 0);
	QWidget* toolWidget = new QWidget(this);
	this->layout()->addWidget(toolWidget);
	this->layout()->addWidget(this->m_listWidget);
	toolWidget->setLayout(new QHBoxLayout(this));
	toolWidget->layout()->setContentsMargins(2, 9, 2, 0);
	toolWidget->layout()->addWidget(this->m_buttonClear);
	toolWidget->layout()->addWidget(this->m_searchEdit);
	toolWidget->layout()->addWidget(this->m_labelInfoIcon);
	toolWidget->layout()->addWidget(this->m_labelInfoCount);
	toolWidget->layout()->addWidget(this->m_labelWarnIcon);
	toolWidget->layout()->addWidget(this->m_labelWarnCount);
	toolWidget->layout()->addWidget(this->m_labelErrorIcon);
	toolWidget->layout()->addWidget(this->m_labelErrorCount);
	toolWidget->layout()->addItem(new QSpacerItem(40,20,QSizePolicy::Expanding));
	this->m_buttonClear->setText("Clear");
	this->m_labelInfoIcon->setPixmap(QPixmap("res\\console_info.png").scaled(15,15));
	this->m_labelWarnIcon->setPixmap(QPixmap("res\\console_warning.png").scaled(15, 15));
	this->m_labelErrorIcon->setPixmap(QPixmap("res\\console_error.png").scaled(15, 15));
	this->m_labelInfoCount->setText("0");
	this->m_labelWarnCount->setText("0");
	this->m_labelErrorCount->setText("0");
	this->m_listWidget->setMessagePtr(&this->m_messageVector);
	for(int i = 0;i<100;i++) {
		this->printInfo(__FILE__, std::to_string(i).c_str());
	}
	return;
	QMenu* menu = new QMenu(this->m_listWidget);
	QAction* action_info = new QAction("Information", menu);
	action_info->setCheckable(true);
	action_info->setChecked(true);
	QAction* action_warning = new QAction("Warning", menu);
	action_warning->setCheckable(true);
	action_warning->setChecked(true);
	QAction* action_eror = new QAction("Error", menu);
	action_eror->setCheckable(true);
	action_eror->setChecked(true);
	QObject::connect(action_info, &QAction::toggled, this, [=](bool value) {
		this->m_showInfo = value;
		this->onEnabledStageChanged();
	});
	QObject::connect(action_warning, &QAction::toggled, this, [=](bool value) {
		this->m_showInfo = value;
		this->onEnabledStageChanged();
	});
	QObject::connect(action_eror, &QAction::toggled, this, [=](bool value) {
		m_showError = value;
		onEnabledStageChanged();
	});
	QObject::connect(this->m_buttonSearch, &QPushButton::clicked, this, &LEEditorConsole::onClickSearch);
	QObject::connect(this->m_buttonClear, &QPushButton::clicked, this, &LEEditorConsole::onClickClear);
	QObject::connect(this->m_isAutoRoll, &QCheckBox::toggled, this->m_listWidget, &LEEditorConsoleListWidget::setAutoRoll);
	menu->addActions({action_info, action_warning, action_eror});
	menu->addSeparator();
	this->m_buttonFilter->setMenu(menu);
	this->m_listWidget->setMessagePtr(&this->m_messageVector);
}

void LEEditorConsole::onClickSearch() {
	QString content;
	static QWidget* dialog = nullptr;
	if (dialog) {
		dialog->show();
		return;
	}
	dialog = new QWidget;
	dialog->setParent(m_listWidget);
	dialog->setStyleSheet("QDialog{background-color: rgb(255, 255, 255);border: 1px solid gray}");
	dialog->setGeometry(m_listWidget->size().width() - 250, 0, 230, 50);

	QLabel* l = new QLabel("Find");
	l->setParent(dialog);
	l->setGeometry(10, 15, 100, 20);
	l->setAttribute(Qt::WA_DeleteOnClose);

	QLineEdit* edit = new QLineEdit;
	edit->setGeometry(50, 15, 100, 20);
	edit->setParent(dialog);
	edit->setAttribute(Qt::WA_DeleteOnClose);

	QPushButton* icon_Serach = new QPushButton(QIcon("res/icon_search.png"),QString());
	QPushButton* icon_Close = new QPushButton(QIcon("res/icon_close.png"), QString());

	icon_Serach->setParent(dialog);
	icon_Serach->setGeometry(170, 15, 20, 20);
	icon_Serach->setFixedSize(20, 20);

	icon_Close->setParent(dialog);
	icon_Close->setFixedSize(20, 20);
	icon_Close->setGeometry(200, 15, 20, 20);

	dialog->show();

	// icon_Close->addClickEvent([=]()
	// {
	// 	dialog->close();
	// });
	// 
	// icon_Serach->addClickEvent([=]()
	// {
	// 	QVector<int> indexes;
	// 	for (int i = 0; i < m_messageVector.size(); i++) {
	// 		if (m_messageVector[i].serach(edit->text())) {
	// 			indexes.push_back(i);
	// 		}
	// 	}
	// 	m_listWidget->setValidIndexes(indexes);
	// 	m_listWidget->refreshAfterIndexesChanged();
	// });
}

void LEEditorConsole::onClickClear() {
	m_listWidget->clear();
}

void LEEditorConsole::printInfo(const char* calledInfo, const char* message) {
	this->printInternal(calledInfo, message, ConsoleMessageType::Info);
}

void LEEditorConsole::printWarning(const char* calledInfo, const char* message) {
	this->printInternal(calledInfo, message, ConsoleMessageType::Warning);
}

void LEEditorConsole::printError(const char* calledInfo, const char* message) {
	this->printInternal(calledInfo, message, ConsoleMessageType::Error);
}

void LEEditorConsole::printInternal(const QString& calledInfo, const QString& message, ConsoleMessageType type) {
	if ((type == ConsoleMessageType::Info && !m_showInfo) ||
		(type == ConsoleMessageType::Warning && !m_showWarning) ||
		(type == ConsoleMessageType::Error && !m_showError)) {
		return;
	}
	this->m_messageVector.push_back(ConsoleMessage(calledInfo, message, type));
	this->m_listWidget->refreshAfterPrint(m_messageVector.size() - 1);
}

void LEEditorConsole::updateCountLabel() {
	this->m_labelMessageCount->setText(
		QString("通知： %1  错误：%2  警告：%3").arg(m_messageCount[0]).arg(m_messageCount[1]).arg(m_messageCount[2]));
}

void LEEditorConsole::onEnabledStageChanged() {
	QVector<int> indexes;
	for (int i = 0; i < m_messageVector.size(); i++) {
		if ((m_messageVector.at(i).type == ConsoleMessageType::Info && !m_showInfo) ||
			(m_messageVector.at(i).type == ConsoleMessageType::Warning && !m_showWarning) ||
			(m_messageVector.at(i).type == ConsoleMessageType::Error && !m_showError)) {
			continue;
		}
		indexes.push_back(i);
	}
	m_listWidget->setValidIndexes(indexes);
	m_listWidget->refreshAfterIndexesChanged();
}

void LEEditorConsole::keyPressEvent(QKeyEvent* e) {
	if (m_listWidget->hasFocus()) {
		if (e->modifiers() == Qt::KeyboardModifiers::enum_type::ControlModifier && e->key() == Qt::Key::Key_F) {
			this->onClickSearch();
		}
	}
}

LEEditorConsole::LEEditorConsole(QWidget* parent):QWidget(parent),
    m_showInfo(true), m_showWarning(true), m_showError(true), m_labelMessageCount(new QLabel),
	m_buttonFilter(new QPushButton), m_buttonSearch(new QPushButton),m_buttonClear(new QPushButton),
    m_isAutoRoll(new QCheckBox),m_listWidget(new LEEditorConsoleListWidget), m_labelInfoIcon(new QLabel), m_labelWarnIcon(new QLabel),
	m_labelErrorIcon(new QLabel), m_labelInfoCount(new QLabel), m_labelWarnCount(new QLabel),m_labelErrorCount(new QLabel),
    m_searchEdit(new LEEditorLineEdit(QString())),m_clearButton(new QPushButton) {

	this->initLayout();
}

LEEditorConsole::~LEEditorConsole() {

}