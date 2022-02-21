#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_launcher_project_template_button.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include <QCheckBox>
#include <QWidget>
#include <QPainter>

enum LEProjectTemplateType {
	PTT_2D = 0,
	PTT_3D = 1,
	PTT_DEMO = 2,
	PTT_MAX_ENUM = 0x7fffffff
};
struct LENewProjectInfo {
	LEProjectTemplateType type;
	QString               name;
	QString               path;
};
class LELauncherProjectTemplateButton :public QWidget {
	Q_OBJECT
private:
	bool                  m_isMouseEntered;
	QPixmap*              m_pixmap;
	QCheckBox*            m_checkBox;
	LEProjectTemplateType m_type;

	void                  onButtonClicked(bool isChecked);
		                  
	void                  onCheckBoxClicked(bool isChecked);
protected:
	void                  mousePressEvent(QMouseEvent* event) override;
		                  
	void                  paintEvent(QPaintEvent* event) override;
		                  
	void                  enterEvent(QEvent* event) override;
		                  
	void                  leaveEvent(QEvent* event) override;
public:
	void                  setProjectTemplateContext(QPixmap * pixmap, LEProjectTemplateType type);
			              
	explicit              LELauncherProjectTemplateButton(QWidget* parent = nullptr);
			              
	virtual              ~LELauncherProjectTemplateButton();
			              
	void                  cancelSelection();
			              
	bool                  isSelected() const;
Q_SIGNALS:	              
	void                  buttonClickSignal(LEProjectTemplateType type,bool selected);
};
