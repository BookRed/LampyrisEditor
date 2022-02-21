#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Editor
 * \File:    le_editor_sand_box_widget.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_interface_sandbox_widget.h"
#include "Runtime/Renderer/le_render_target_window.h"
// QT Includes
#include <QResizeEvent>
#include <QTime>
#include <QWidget>
// GLM Includes
#include <glm/glm.hpp>

class LELevel;
class LEEntity;
class LECamera;
class LESkyBoxRenderer;
class LEGridRenderer;

class LEEditorSandBoxWidget:public QWidget,
                            public ISandBoxWidget {
	Q_OBJECT
private:
	LERenderTargetWindow* m_window;
	LELevel*              m_pVirtualLevel;
	LEEntity*             m_pEntity;
	LECamera*             m_pCamera;
	LESkyBoxRenderer*     m_pSkyBoxRenderer;
	LEGridRenderer*       m_pGridRenderer;

	float                 m_mousePressTime;
	QTimer*               m_resizeTimer;

	/* key states*/
	bool                  m_wsadPressed[4]{ false };
	bool                  m_mousePressed[2] {false};
	glm::vec2             m_mousePreviousPos{0,0};
	glm::vec2             m_mouseCurrentPos{0,0};

	/* camera properties*/
	float                 m_cameraBaseSpeed = 1.0f;
	float                 m_cameraCurrentSpeed = 1.0f;
	float                 m_cameraSpeedBoost = 0.01f;
	float                 m_cameraRotationSpeed = 0.15f;

	void                  updateSandBoxCamera();

	void                  update();
protected:
	void                  resizeEvent(QResizeEvent* event) override;

	void                  mousePressEvent(QMouseEvent* event) override;

	void                  mouseReleaseEvent(QMouseEvent* event) override;

	void                  mouseMoveEvent(QMouseEvent* event) override;
public:
	void                  keyPressEvent(QKeyEvent* event) override;

	void                  keyReleaseEvent(QKeyEvent* event) override;

	explicit              LEEditorSandBoxWidget(QWidget* parent = Q_NULLPTR);
					     
	virtual              ~LEEditorSandBoxWidget();
					     
	void                  createSandBoxObjects();
					     
	void                  createSandBoxWorld();
					     
	void                  createSkyBox();
					     
	void                  createWanderingCamera();
					     
	void                  createGrid();
					     
	void                  createGizmo();
					     
	void                  createPixelLevelPickupSystem();

	void                  createRenderThread();
};
