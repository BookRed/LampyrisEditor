/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_editor_sand_box_widget.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_editor_sand_box_widget.h"
#include "Runtime/LevelManage/le_level.h"
#include "Runtime/LevelManage/le_world.h"
#include "Runtime/Renderer/le_camera.h"
#include "Runtime/Math/le_transform.h"
#include "Runtime/Base/le_entity.h"
#include "Runtime/Primitive/le_sky_box_renderer.h"
#include "Runtime/Renderer/le_inherent_ubo.h"
#include "Runtime/Renderer/le_vk_graphics_pipeline.h"
#include "Runtime/Resource/le_material.h"
#include "Runtime/Primitive/le_grid_renderer.h"
// QT Includes
#include <QHBoxLayout>
#include <QTimer>
#include <QDebug>
// STD Includes
#include <thread>
#include <chrono>

#include "le_editor_manager.h"
#include "le_interface_entity_tree_widget.h"
#include "le_interface_property_editor_widget.h"
#include "Runtime/Renderer/le_pick_up_executor.h"

void LEEditorSandBoxWidget::updateSandBoxCamera() {
	glm::vec3 camFront = this->m_pCamera->getDirection();
	// update wsad
	if (this->m_wsadPressed[0] || this->m_wsadPressed[1] || this->m_wsadPressed[2] || this->m_wsadPressed[3]) {
		this->m_cameraCurrentSpeed = this->m_cameraCurrentSpeed + this->m_cameraSpeedBoost;
	} else {
		this->m_cameraCurrentSpeed = this->m_cameraBaseSpeed;
	}
	if(this->m_wsadPressed[0]) {
		this->m_pEntity->getTransform()->translate(camFront * this->m_cameraCurrentSpeed);
	}
	if(this->m_wsadPressed[1]) {
		this->m_pEntity->getTransform()->translate(-camFront * this->m_cameraCurrentSpeed);
	}
	if (this->m_wsadPressed[2]) {
		this->m_pEntity->getTransform()->translate(glm::normalize(glm::cross(camFront, glm::vec3(0.0f, 1.0f, 0.0f))) * this->m_cameraCurrentSpeed);
	}
	if (this->m_wsadPressed[3]) {
		this->m_pEntity->getTransform()->translate(-glm::normalize(glm::cross(camFront, glm::vec3(0.0f, 1.0f, 0.0f))) * this->m_cameraCurrentSpeed);
	}
	// update camera rotation
	if (this->m_mousePressed[1]) {
		glm::vec2 delta = this->m_mouseCurrentPos - this->m_mousePreviousPos;
		delta *= this->m_cameraRotationSpeed;
		this->m_pEntity->getTransform()->rotateAroundSelf(glm::vec3(delta.y, delta.x, 0.0f));
		this->m_mousePreviousPos = this->m_mouseCurrentPos;
	}
}

void LEEditorSandBoxWidget::update() {
	this->updateSandBoxCamera();
	LERenderTarget::setActiveRenderTarget(this->m_window);
	// draw screen
	this->m_window->acquireNextFrame();
	LELevel* pActiveLevel = LEWorld::getInstance()->getActiveLevel();
	this->m_pCamera->resetRenderContext();
	pActiveLevel->computeVisibleRenderer(this->m_pCamera);
	this->m_pCamera->render();
	this->m_window->present();
	LEInherentUBO::getInstance()->updateLight();
	return;
	// draw grid 
	this->m_pGridRenderer->getDefaultMaterial()->getGraphicsPipeline(0);

	// draw skybox
	this->m_pSkyBoxRenderer->getDefaultMaterial()->getGraphicsPipeline(0);

	this->m_window->present();

	if(this->m_window->isResized()) {
		this->m_window->setResized(false);
	}
	LERenderTarget::setActiveRenderTarget(nullptr);

	qDebug() << m_mouseCurrentPos.x << " " << m_mouseCurrentPos.y;
	
	QWidget::update();
}

void LEEditorSandBoxWidget::keyPressEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_W) { this->m_wsadPressed[0] = true; }
	if (event->key() == Qt::Key_S) { this->m_wsadPressed[1] = true; }
	if (event->key() == Qt::Key_A) { this->m_wsadPressed[2] = true; }
	if (event->key() == Qt::Key_D) { this->m_wsadPressed[3] = true; }
}

void LEEditorSandBoxWidget::resizeEvent(QResizeEvent* event) {
	auto func = [=]() {
		HDC hd = GetDC(NULL);
		int horDPI      = GetDeviceCaps(hd, LOGPIXELSX);
		int verticalDPI = GetDeviceCaps(hd, LOGPIXELSY);
		float scaleRate = horDPI / 96.0f;
		this->m_window->resize(this->size().width()* scaleRate, this->size().height()* scaleRate);
		this->m_window->setResized(true);
		this->m_resizeTimer->stop();
		this->m_pCamera->setPerspective(glm::radians(45.0f), (float)this->size().width()/this->size().height(), 0.1f, 100000);
		this->m_pCamera->setRenderTarget(this->m_window);
		// this->createPixelLevelPickupSystem();
	};
	// re-start time
	if(this->m_resizeTimer->isActive()) {
		this->m_resizeTimer->stop();
	}
	this->m_resizeTimer->start(250);
	QObject::connect(this->m_resizeTimer, &QTimer::timeout, func);
	QWidget::resizeEvent(event);
}

void LEEditorSandBoxWidget::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {  this->m_mousePressed[0] = true; }
	if (event->button() == Qt::RightButton) {
		this->m_mousePressed[1] = true;
		this->m_mousePreviousPos = this->m_mouseCurrentPos = { event->pos().x(),event->pos().y() };
	}
	QWidget::mousePressEvent(event);
}

void LEEditorSandBoxWidget::mouseReleaseEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton)  { this->m_mousePressed[0] = false; }
	if (event->button() == Qt::RightButton) { this->m_mousePressed[1] = false; }
	QWidget::mouseReleaseEvent(event);
}

void LEEditorSandBoxWidget::mouseMoveEvent(QMouseEvent* event) {
	if (this->m_mousePressed[1]) {
		this->m_mouseCurrentPos = { event->pos().x(),event->pos().y() };
	}
	QWidget::mouseMoveEvent(event);
}

LEEditorSandBoxWidget::LEEditorSandBoxWidget(QWidget* parent):
	m_window(new LERenderTargetWindow), m_resizeTimer(new QTimer(this)) {
	// init layout
	QHBoxLayout* pLayout = new QHBoxLayout(this);
	pLayout->setContentsMargins(0, 0, 0, 0);
	pLayout->addWidget(this->m_window->getQtWindow());
	this->createSandBoxObjects();
}

LEEditorSandBoxWidget::~LEEditorSandBoxWidget() {

}

void LEEditorSandBoxWidget::createSandBoxObjects() {
	this->createSandBoxWorld();
	this->createWanderingCamera();
	this->createSkyBox();
	this->createGrid();
	this->createGizmo();
	this->createRenderThread();
	// this->m_window->createVulkanContext();
	// this->createPixelLevelPickupSystem();
}

void LEEditorSandBoxWidget::keyReleaseEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_W) { this->m_wsadPressed[0] = false; }
	if (event->key() == Qt::Key_S) { this->m_wsadPressed[1] = false; }
	if (event->key() == Qt::Key_A) { this->m_wsadPressed[2] = false; }
	if (event->key() == Qt::Key_D) { this->m_wsadPressed[3] = false; }
}

void LEEditorSandBoxWidget::createSandBoxWorld() {
	this->m_pVirtualLevel = LEWorld::getInstance()->createEmptyLevel();
	this->m_pEntity = this->m_pVirtualLevel->createSandBoxEntity();
}

void LEEditorSandBoxWidget::createWanderingCamera() {
	this->m_pCamera = new LECamera;
	this->m_pEntity->addComponent(this->m_pCamera);
	this->m_pEntity->getTransform()->setWorldPosition(0, 0, -10);
	this->m_pCamera->setProjectionMode(LECamera::PM_Perspective);
	this->m_pCamera->setLookAt(0, 0, 1, 0, 1, 0);
}

void LEEditorSandBoxWidget::createGrid() {
	LEShader* vsShaderGrid = LEShaderCompiler::getInstance()->compile("shaders\\le_grid.glsl", VK_SHADER_STAGE_VERTEX_BIT);
	if (!vsShaderGrid) {
		std::cout << "compile shaders\\le_grid.vert failed: \n" << std::endl;
	}
	LEShader* fsShaderGrid = LEShaderCompiler::getInstance()->compile("shaders\\le_grid.glsl", VK_SHADER_STAGE_FRAGMENT_BIT);
	if (!fsShaderGrid) {
		std::cout << "compile shaders\\le_grid.frag failed: \n" << std::endl;
	}
	LEMaterial* pMaterial = new LEMaterial;
	pMaterial->addSubPass(vsShaderGrid, fsShaderGrid);

	this->m_pGridRenderer = new LEGridRenderer;
	this->m_pGridRenderer->setMaterial(pMaterial);
	this->m_pEntity->addComponent(this->m_pGridRenderer);
	this->m_pGridRenderer->setOrderInLayer(-0xff + 1);
}

void LEEditorSandBoxWidget::createGizmo() {
}

void LEEditorSandBoxWidget::createPixelLevelPickupSystem() {
	LEPickUpExecutor::getInstance()->create(this->m_window);
}

void LEEditorSandBoxWidget::createSkyBox() {
	LECubemap* pTextureCube = new LECubemap(2048, {
										 "testData\\skybox\\sea_left.JPG",
										 "testData\\skybox\\sea_right.JPG",
										 "testData\\skybox\\sea_up.JPG",
										 "testData\\skybox\\sea_down.JPG",
										 "testData\\skybox\\sea_front.JPG",
										 "testData\\skybox\\sea_back.JPG" });
	LEMaterial *pMaterial = new LEMaterial;
	LEEntity* pEntitySkyBox = this->m_pVirtualLevel->createSandBoxEntity();
	std::string errorString;
	auto vsShaderSkyBox = LEShaderCompiler::getInstance()->compile("shaders\\le_sky_box.vert",VK_SHADER_STAGE_VERTEX_BIT);
	if (!vsShaderSkyBox) {
		std::cout << "compile shaders\\le_sky_box.vert failed: \n" << std::endl;
		std::cout << errorString << std::endl;;
	}
	auto fsShaderSkyBox = LEShaderCompiler::getInstance()->compile("shaders\\le_sky_box.frag", VK_SHADER_STAGE_FRAGMENT_BIT);
	if (!fsShaderSkyBox) {
		std::cout << "compile shaders\\le_sky_box.frag failed: \n" << std::endl;
		std::cout << errorString << std::endl;;
	}
	pMaterial->addSubPass(vsShaderSkyBox, fsShaderSkyBox);
	this->m_pSkyBoxRenderer =  new LESkyBoxRenderer;
	this->m_pSkyBoxRenderer->setMaterial(pMaterial);
	pEntitySkyBox->addComponent(this->m_pSkyBoxRenderer);
	this->m_pSkyBoxRenderer->setSkyBoxTexture(pTextureCube);
	this->m_pSkyBoxRenderer->setOrderInLayer(-0xff);
	this->m_pSkyBoxRenderer->setVisible(true);
	this->m_pSkyBoxRenderer->setCamera(this->m_pCamera);
}

void LEEditorSandBoxWidget::createRenderThread() {
	std::thread th([=]() {
		while (true) {
			if (this->m_window->isVulkanContextInitialized()) {
				if (this->m_resizeTimer->isActive()) {
					// not ready for rendering after resize, although vulkan context is created,
					// because it should be recreated.
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					continue;
				}
				// this->m_pEntity->getTransform()->rotateAroundSelf({ 0,-.25,0 });
				this->update();
			}
			else {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
	});
	th.detach();
}