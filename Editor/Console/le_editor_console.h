#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_console.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// QT Includes
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
// LE Includes
#include "le_editor_console_definitions.h"
#include "le_editor_console_list_widget.h"
#include "../WrappedQt/le_editor_line_edit.h"

using namespace Lampyris::Detail;
class LEEditorConsole : public QWidget {
	using MessageVector = QVector<ConsoleMessage>;
	Q_OBJECT
private:
	// [Message]
	MessageVector              m_messageVector;
	int                        m_messageCount[3] {0, 0, 0};
	bool                       m_showInfo;
	bool                       m_showWarning;
	bool                       m_showError;
	// [Controls]
	QLabel*                    m_labelMessageCount;
	QPushButton*               m_buttonFilter;
	QPushButton*               m_buttonSearch;
	QPushButton*               m_buttonClear;
	QCheckBox*                 m_isAutoRoll;
	LEEditorConsoleListWidget* m_listWidget;

	QLabel*                    m_labelInfoIcon;
	QLabel*                    m_labelWarnIcon;
	QLabel*                    m_labelErrorIcon;

	QLabel*                    m_labelInfoCount;
	QLabel*                    m_labelWarnCount;
	QLabel*                    m_labelErrorCount;

	LEEditorLineEdit*          m_searchEdit;
	QPushButton*               m_clearButton;

	void                       initLayout();

	void                       printInternal(const QString& calledInfo, const QString& message, ConsoleMessageType type);

	void                       updateCountLabel();

	void                       onEnabledStageChanged();

	void                       onClickSearch();

	void                       onClickClear();
protected:
	virtual void               keyPressEvent(QKeyEvent* e);
public:
	explicit                   LEEditorConsole(QWidget* parent = nullptr);

	                          ~LEEditorConsole();
	// interface methods
	void                       printInfo(const char* calledInfo, const char* message);

	void                       printWarning(const char* calledInfo, const char* message);

	void                       printError(const char* calledInfo, const char* message);
};