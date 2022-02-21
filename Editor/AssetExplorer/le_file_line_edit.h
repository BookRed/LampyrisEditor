#pragma once
#include <QLineEdit>
#include <le_editor_controls.h>

class leLineEdit :public QLineEdit {
	Q_OBJECT
Q_SIGNALS:
	void signalFocusOut();
protected:
	virtual void keyPressEvent(QKeyEvent* event) {
		if (event->key() == Qt::Key_Return) {
			this->clearFocus();
			emit editingFinished();
		}
		else {
			QLineEdit::keyPressEvent(event);
		}
	}
	void focusInEvent(QFocusEvent* event) {
		this->setReadOnly(false);
		QLineEdit::focusInEvent(event);
	}

	void focusOutEvent(QFocusEvent* e) {
		this->setCursorPosition(this->text().length());
		this->setReadOnly(true);
		emit signalFocusOut();
	}

public:
	explicit leLineEdit(QWidget* parent = nullptr) :
		QLineEdit(parent) {}
};

