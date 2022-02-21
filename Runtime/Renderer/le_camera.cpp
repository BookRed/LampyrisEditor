/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_camera.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_camera.h"
#include "le_vk_render_pass.h"
#include "le_inherent_ubo.h"
#include "le_render_target.h"
#include "le_vk_graphics_pipeline.h"
#include "le_render_target_texture.h"
#include "le_pick_up_executor.h"
#include "le_render_target.h"
#include "le_render_target_window.h"
#include "le_vk_command_buffer.h"
#include "../Primitive/le_outline_renderer.h"
#include "../Interact/le_input_system.h"
#include "../Primitive/le_mesh_renderer.h"
#include "../Resource/le_material.h"
#include "../Resource/le_resource_manager.h"
#include "../Base/le_entity.h"
#include "../Resource/le_mesh.h"
#include "../Math/le_transform.h"
#include "../LevelManage/le_level.h"
#include "../LevelManage/le_world.h"

#if defined(near) && defined(far)
#pragma push_macro("near")
#pragma push_macro("far")
#undef near
#undef far
#define NEAR_FAR_MACRO_CANCELED
#endif 

LECamera* LECamera::ms_pMainCamera = nullptr;
LECamera* LECamera::ms_pRenderingCamera = nullptr;

LECamera::LECamera() {
	if (!ms_pMainCamera) {
		ms_pMainCamera = this;
	}
	this->m_frustumPlanes.resize(6);
	this->m_pRenderQueue  = new LERenderQueue;
	this->m_pCommandBuffer = LEVkCommandBuffer::createBuffer();
	this->createZPrePass();
	this->createGridPass();
	LEWorld::getInstance()->getActiveLevel()->addCamera(this);
}

LECamera::~LECamera() {
	LEWorld::getInstance()->getActiveLevel()->removeCamera(this);
}

void LECamera::setRenderTarget(LERenderTarget* pRenderTarget) {
	this->m_pRenderTarget = pRenderTarget;
}

void LECamera::setProjectionMode(ProjectionMode mode) {
	this->m_projectionMode = mode;
}

LECamera::ProjectionMode LECamera::getProjectionMode() const {
	return this->m_projectionMode;
}

void LECamera::setFOV(float fov) {
	this->m_fieldOfView = glm::radians(fov);
	this->m_projectMatrix = glm::perspective(this->m_fieldOfView,
		                                     this->m_width,
		                                     this->m_near,
		                                     this->m_far);
}		                                     

float LECamera::getFOV() const {
	return glm::degrees(this->m_fieldOfView);
}

void LECamera::setNear(float near) {
	if (this->m_projectionMode == PM_Perspective) {
		this->setPerspective(this->m_fieldOfView,
			                 this->m_width / this->m_height,
			                 near,
			                 this->m_far);
	}
	else {
		this->setOrthographic(this->m_size,
			                  near,
			                  this->m_far);
	}
}

float LECamera::getNear() const {
	return this->m_near;
}

void LECamera::setFar(float far) {
	if (this->m_projectionMode == PM_Perspective) {
		this->setPerspective(this->m_fieldOfView,
			this->m_width / this->m_height,
			this->m_near,
			far);
	}
	else {
		this->setOrthographic(this->m_size,
			this->m_near,
			far);
	}
}

float LECamera::getFar() const {
	return this->m_far;
}

void LECamera::setViewport(float x, float y, float w, float h) {
	this->m_viewport.x = x;
	this->m_viewport.y = y;
	this->m_viewport.z = w;
	this->m_viewport.w = h;
}

glm::vec4 LECamera::getViewPort() {
	return this->m_viewport;
}

void LECamera::setPerspective(float fov, float aspect, float near, float far) {
	this->m_near = near;
	this->m_far  = far;
	this->m_projectMatrix = glm::perspective(fov, aspect, near, far);
	this->m_projectMatrix[1][1] *= -1.0f;
	this->m_projectMatrix[0][0] *= -1.0f;
}

void LECamera::setLookAt(float centerX, float centerY, float centerZ, float upX, float upY, float upZ) {
	glm::vec3 eyePos = this->m_pAttachedEntity->getTransform()->getWorldPosition();
	glm::vec3 centerPos = glm::vec3{ centerX, centerY, centerZ };
	this->m_lookDirection = glm::normalize(centerPos - eyePos);
	this->m_upDirection = glm::vec3{ upX,upY,upZ };
}

void LECamera::setOrthographic(float size, float near, float far) {
	float widthHalf = size;
	float heightHalf = size * (this->m_height / this->m_width);
	this->m_projectMatrix = glm::ortho(-widthHalf, widthHalf, -heightHalf, heightHalf, near, far);
}

glm::mat4 LECamera::getVPMatrix() {
	return this->m_projectMatrix * this->getViewMatrix();
}

glm::mat4 LECamera::getProjectionMatrix() const {
	return this->m_projectMatrix;
}

glm::mat4 LECamera::getViewMatrix() {
     glm::vec3 eyePos   = this->m_pAttachedEntity->getTransform()->getLocalPosition();
     glm::vec3 rot{ glm::radians(this->m_pAttachedEntity->getTransform()->getLocalEulerAngle().x), 
     	            glm::radians(this->m_pAttachedEntity->getTransform()->getLocalEulerAngle().y), 
     	            0.0f};
	 this->m_lookDirection = glm::normalize(glm::rotate(glm::qua(rot), {0,0,1}));
	 this->m_upDirection = glm::normalize(glm::rotate(glm::qua(rot), { 0,1,0 }));

     this->m_viewMatrix = glm::lookAt(eyePos,
     	                              eyePos +  this->m_lookDirection,
     	                              this->m_upDirection);
	return this->m_viewMatrix;
}

bool LECamera::testVisiable(LEPrimitiveRenderer* pRenderer) const {
	int rendererType = pRenderer->getRendererType();
	if(rendererType == LE_RENDERER_TYPE_POST_EFFECT || rendererType == LE_RENDERER_TYPE_SKY_BOX) {
		// always visible
		return true;
	}
	// frustum culling test
	LEMesh* pMesh = pRenderer->getMesh();
	if(pMesh == nullptr) {
		return false;
	}
	LEAABB* pAABB = pMesh->getAABB();
	auto vertices = pAABB->getVertices();

	for(int i = 0;i<vertices.size();i++) {
		
	}
	return true;
}

void LECamera::addRenderContext(LEPrimitiveRenderer* pRenderer) {
	this->m_pRenderQueue->addRenderContext(pRenderer);
}

int LECamera::getDepth() const {
	return this->m_depth;
}

void LECamera::setDepth(int depth) {
	this->m_depth = depth;
}

void LECamera::clearRenderTargetColorDepthStencil(LERenderTarget* pRenderTarget) {
	VkClearColorValue clearColorValue;
	clearColorValue.float32[0] = this->m_clearColor.r;
	clearColorValue.float32[1] = this->m_clearColor.g;
	clearColorValue.float32[2] = this->m_clearColor.b;
	clearColorValue.float32[3] = this->m_clearColor.a;

	VkClearDepthStencilValue clearDepthStencilValue;
	clearDepthStencilValue.depth   = this->m_clearDepth;
	clearDepthStencilValue.stencil = this->m_clearStencil;

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = pRenderTarget->getRenderImage();
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	// color
	this->m_pCommandBuffer->setupPipelineBarrier(barrier, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);
	this->m_pCommandBuffer->clearColorImage(pRenderTarget->getRenderImage(), 
		                                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
		                                    clearColorValue);
	barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	this->m_pCommandBuffer->setupPipelineBarrier(barrier, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

	// depth & stencil
	barrier.image = pRenderTarget->getDepthStencilImage();
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	this->m_pCommandBuffer->setupPipelineBarrier(barrier, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);
	this->m_pCommandBuffer->clearDepthStencilImage(pRenderTarget->getDepthStencilImage(),
		                                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
		                                           clearDepthStencilValue);
	barrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	this->m_pCommandBuffer->setupPipelineBarrier(barrier, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);
}

void LECamera::createZPrePass() {
	LEShader* pVSShader = LEShaderCompiler::getInstance()->compile("shaders\\le_z_pre_pass.glsl", VK_SHADER_STAGE_VERTEX_BIT);
	LEShader* pFSShader = LEShaderCompiler::getInstance()->compile("shaders\\le_z_pre_pass.glsl",VK_SHADER_STAGE_FRAGMENT_BIT);
	this->m_ZPreRenderPipeline = LEVkGraphicsPipeline::createFromShader(pVSShader, pFSShader);
}

void LECamera::renderZPrePass() {
	// this->m_pCommandBuffer->beginRenderPass(this->m_ZPreRenderPipeline->getRenderPass());
	// this->m_pCommandBuffer->bindGraphicsPipeline(this->m_ZPreRenderPipeline);
}

void LECamera::createGridPass() {
	LEShader* vsShaderGrid = LEShaderCompiler::getInstance()->compile("shaders\\le_grid.glsl", VK_SHADER_STAGE_VERTEX_BIT);
	if (!vsShaderGrid) {
		std::cout << "compile shaders\\le_grid.vert failed: \n" << std::endl;
	}
	LEShader* fsShaderGrid = LEShaderCompiler::getInstance()->compile("shaders\\le_grid.glsl", VK_SHADER_STAGE_FRAGMENT_BIT);
	if (!fsShaderGrid) {
		std::cout << "compile shaders\\le_grid.frag failed: \n" << std::endl;
	}
	this->m_gridPipeline = LEVkGraphicsPipeline::createFromShader(vsShaderGrid, fsShaderGrid);
}

void LECamera::renderGridPass() {
}

void LECamera::createShadowPass() {
	this->m_shadowMap = new LERenderTargetTexture(this->m_width, this->m_height, VK_FORMAT_R8G8B8A8_UNORM);
	LEShader* vsShaderGrid = LEShaderCompiler::getInstance()->compile("shaders\\le_shadow_depth.glsl", VK_SHADER_STAGE_VERTEX_BIT);
	if (!vsShaderGrid) {
		std::cout << "compile shaders\\le_shadow_depth.vert failed: \n" << std::endl;
	}
	LEShader* fsShaderGrid = LEShaderCompiler::getInstance()->compile("shaders\\le_shadow_depth.glsl", VK_SHADER_STAGE_FRAGMENT_BIT);
	if (!fsShaderGrid) {
		std::cout << "compile shaders\\le_shadow_depth.frag failed: \n" << std::endl;
	}
	this->m_shadowMapPipeline = LEVkGraphicsPipeline::createFromShader(vsShaderGrid, fsShaderGrid);
}

void LECamera::render() {
	if(!this->m_pRenderTarget) {
		return;
	}
	this->ms_pRenderingCamera = this;
	// update inherent UBO
	LEInherentUBO::getInstance()->updateRenderingCamera(this);
	LERenderTarget::setActiveRenderTarget(this->m_pRenderTarget);
	this->m_pRenderQueue->sort();
	auto& renderContext = this->m_pRenderQueue->getRenderContext();

	// clear color + Z + stencil
	this->m_pCommandBuffer->startRecord();
	this->clearRenderTargetColorDepthStencil(this->m_pRenderTarget);

	// firstly drawIndexed depth pass for early-z fragment test
	this->m_ZPreRenderPipeline->buildPipeline();
	this->m_pCommandBuffer->beginRenderPass();
	this->m_pCommandBuffer->bindGraphicsPipeline(this->m_ZPreRenderPipeline);
	this->m_pCommandBuffer->bindDescriptorSet(VK_PIPELINE_BIND_POINT_GRAPHICS,
		                                      this->m_ZPreRenderPipeline->getPipelineLayout(),
		                                      0,
		                                      1,
		                                      &(this->m_ZPreRenderPipeline->getDescriptorSet(0)));
	VkViewport viewport;
	viewport.x        = 0;
	viewport.y        = 0;
	viewport.width    = this->m_pRenderTarget->getWidth();
	viewport.height   = this->m_pRenderTarget->getHeight();
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	this->m_pCommandBuffer->setViewPort(viewport);

	VkRect2D scissor;
	scissor.offset.x = 0.0f;
	scissor.offset.y = 0.0f;
	scissor.extent.width = viewport.width;
	scissor.extent.height = viewport.height;
	this->m_pCommandBuffer->setScissor(scissor);

	for (uint32_t i = 0; i < renderContext.size(); i++) {
		this->m_ZPreRenderPipeline->pushConstantMatrix("modelMatrix",
			                               VK_SHADER_STAGE_VERTEX_BIT,
			                               renderContext[i].pRenderer->getAttachedEntity()->getTransform()->getWorldMatrix());
		this->m_pCommandBuffer->drawMesh(renderContext[i].pRenderer->getMesh());
	}
	this->m_pCommandBuffer->endRenderPass();

	struct RenderContextBatch {
		LEMesh*                 mesh;
		LEVkGraphicsPipeline*   graphicsPipeline;
		uint32_t                firstIndex;
		std::vector<glm::mat4*> modelMatrix;
		std::vector<uint32_t>   descriptorSetIndices;
	};
	std::vector<RenderContextBatch> batches;

	LEMesh*               pLastMesh           = nullptr;
	LEVkGraphicsPipeline* pLastPipeline       = nullptr;
	uint32_t              primitiveCount      = renderContext.size();
	
	std::vector<uint32_t>                     descriptorIndices(primitiveCount);

	// dynamic batching stage
	for (uint32_t i = 0; i < primitiveCount; i++) {
		LEMaterial* pMaterial = renderContext[i].pRenderer->getMaterial();
		LEMesh* pMesh         = renderContext[i].pRenderer->getMesh();
		uint32_t passCount    = 1;
		for (uint32_t j = 0; j < passCount; j++) {
			uint32_t descriptorIndex        = pMaterial->getDescriptorIndex(j);
			LEVkGraphicsPipeline* pPipeline = pMaterial->getGraphicsPipeline(j);
			if (pPipeline == pLastPipeline && pLastMesh == pMesh) {
				batches.back().descriptorSetIndices.push_back(pMaterial->getDescriptorIndex(j));
			}
			else {
				RenderContextBatch batch;
				batch.graphicsPipeline  = pPipeline;
				batch.mesh              = pMesh;
				batch.firstIndex        = i;
				batch.descriptorSetIndices.push_back(pMaterial->getDescriptorIndex(j));
				batches.push_back(batch);
				pLastPipeline = pPipeline;
				pLastMesh     = pMesh;
				pPipeline->buildPipeline();
			}
			glm::mat4 *pMat4 = &(renderContext[i].pRenderer->getAttachedEntity()->getTransform()->getWorldMatrix());
			batches.back().modelMatrix.push_back(pMat4);
			descriptorIndices[i] = descriptorIndex;
		}
	}

	// drawIndexed each batches
	for(auto & batch:batches) {
		// const VkDeviceSize offset = batch.descriptorSetIndices.size() * sizeof(VkDrawIndexedIndirectCommand);
		this->m_pCommandBuffer->beginRenderPass();
		this->m_pCommandBuffer->bindGraphicsPipeline(batch.graphicsPipeline);
		const uint32_t mask = batch.graphicsPipeline->getRequiredVertexFormatMask();
		this->m_pCommandBuffer->bindVertexBuffer(batch.mesh->getVertexBuffer(), 0, mask);
		if(batch.mesh->getIndexBuffer()) {
			this->m_pCommandBuffer->bindIndexBuffer(batch.mesh->getIndexBuffer(), 0);
		}
		for (uint32_t i = 0; i < batch.descriptorSetIndices.size();i++) {
			this->m_pCommandBuffer->bindDescriptorSet(VK_PIPELINE_BIND_POINT_GRAPHICS,
				                                      batch.graphicsPipeline->getPipelineLayout(), 
				                                      0, 
				                                      1,
				                                      &(batch.graphicsPipeline->getDescriptorSet(batch.descriptorSetIndices[i])));
			batch.graphicsPipeline->pushConstantMatrix("modelMatrix", VK_SHADER_STAGE_VERTEX_BIT,*(batch.modelMatrix[i]));
			if (batch.mesh->getIndexBuffer()) {
				this->m_pCommandBuffer->drawIndexed(batch.mesh->getIndexBuffer()->getIndexCount());
			} else {
				this->m_pCommandBuffer->draw(batch.mesh->getVertexBuffer()->getVertexCount());
			}
		}
		this->m_pCommandBuffer->endRenderPass();
	}

	// draw grid
	this->m_gridPipeline->buildPipeline();
	this->m_pCommandBuffer->beginRenderPass();
	this->m_pCommandBuffer->bindGraphicsPipeline(this->m_gridPipeline);
	this->m_pCommandBuffer->bindDescriptorSet(VK_PIPELINE_BIND_POINT_GRAPHICS,
		                                      this->m_gridPipeline->getPipelineLayout(),
		                                      0,
		                                      1,
		                                      &(this->m_gridPipeline->getDescriptorSet(0)));
	this->m_pCommandBuffer->draw(6);
	this->m_pCommandBuffer->endRenderPass();
	if (this->m_pRenderTarget->getRenderTargetType() == LE_RENDER_TARGET_WINDOW) {
		this->m_pCommandBuffer->submit(1, &(static_cast<LERenderTargetWindow*>(this->m_pRenderTarget)->getImageAvailableSemaphore()));
	}
	else {
		this->m_pCommandBuffer->submit();
	}
	this->m_pRenderTarget->setResized(false);
	this->ms_pRenderingCamera = nullptr;
}

void LECamera::startRecord() {
	this->m_pCommandBuffer->startRecord();
}

void LECamera::submit() {
	this->m_pCommandBuffer->submit();
}

LEPrimitiveRenderer* LECamera::pickUpRendererOnMousePos() {
	return nullptr;
}

void LECamera::drawOutline(VkCommandBuffer commandBuffer) {
	auto pRenderer = pickUpRendererOnMousePos();
	// static LEOutlineRenderer* pOutlineRenderer = new LEOutlineRenderer;
	// pOutlineRenderer->setOutlineThickness(5.0f);
	// pOutlineRenderer->setOutlineColor({ 1.0f,165.0f / 255.0f,0.0f,1.0f });
	if (pRenderer) {
		std::cout << "Picked up object' name = " << pRenderer->getAttachedEntity()->getName() << std::endl;
		// pOutlineRenderer->render(commandBuffer, this, pRenderer);
	}
	else {
		std::cout << "you didn't pick up any object" << std::endl;
	}
}

#if defined(LE_EDITOR)
#include "Editor/WrappedQt/le_editor_gui_layout.h"
void LECamera::onPropertyEditorGUI() {
	uint32_t hash = std::hash<std::string>().operator()("Camera");
	if (LEEditorGUILayout::componentSection(le_hash_value(hash, __LINE__), "Camera", false)) {
		this->m_fieldOfView = LEEditorGUILayout::floatField(le_hash_value(hash, __LINE__), "FOV", this->m_fieldOfView);
		this->m_near = LEEditorGUILayout::floatField(le_hash_value(hash, __LINE__), "Near", this->m_near);
		this->m_far = LEEditorGUILayout::floatField(le_hash_value(hash, __LINE__), "Far", this->m_far);

		// viewport
		this->m_viewport.x = LEEditorGUILayout::floatField(le_hash_value(hash, __LINE__), "Offset X", this->m_viewport.x);
		this->m_viewport.y = LEEditorGUILayout::floatField(le_hash_value(hash, __LINE__), "Offset Y", this->m_viewport.y);
		this->m_viewport.z = LEEditorGUILayout::floatField(le_hash_value(hash, __LINE__), "Width", this->m_viewport.z);
		this->m_viewport.a = LEEditorGUILayout::floatField(le_hash_value(hash, __LINE__), "Height", this->m_viewport.a);
	}
	if (this->m_far >= 1000) {
		LEEditorGUILayout::tipBox(le_hash_value(hash, __LINE__), "Far Value is too large!!", LEEditorWrappedTipBox::TipIcon::Warning);
	}
	else if (this->m_far < this->m_near) {
		LEEditorGUILayout::tipBox(le_hash_value(hash, __LINE__), "Far value must be greater than near value", LEEditorWrappedTipBox::TipIcon::Error);
	}
}
#endif // !LE_EDITOR

#if defined(NEAR_FAR_MACRO_CANCELED)
#pragma pop_macro("near")
#pragma pop_macro("far")
#endif