#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_camera.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_CAMERA_H
#define LE_CAMERA_H

// GLM Includes
#include <glm/ext.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtc/matrix_transform.hpp>

// LE Includes
#include "..\Math\le_plane.h"
#include "..\Base\le_component.h"
#include "le_render_queue.h"

// Vulkan Includes
#include <vulkan/vulkan_core.h>

class LERenderTarget;
class LEMeshRenderer;
class LEVkGraphicsPipeline;
class LEVkRenderPass;
class LEVkCommandBuffer;

class LECamera:public LEComponent {
	LE_COMPONENT_NAME(LECamera);
public:
	enum ProjectionMode {
		PM_Perspective  = 1,
		PM_Orthographic = 2,
		PM_MAX_ENUM     = 0x7fffffff       
	};
protected:
	glm::vec3                           m_lookDirection { 0,0,1.0f };
	glm::vec3                           m_upDirection;
					                    
	float                               m_fieldOfView;
	float                               m_size;
					                    
	// viewport		                    
	float                               m_near;
	float                               m_far;
	float                               m_width;
	float                               m_height;
					                    
	// matrix		                    
	glm::mat4                           m_projectMatrix;
	glm::mat4                           m_viewMatrix;
	glm::vec4                           m_viewport;
					                    
	ProjectionMode                      m_projectionMode;
				 	                    
	LERenderTarget*                     m_pRenderTarget;
	LERenderQueue*                      m_pRenderQueue;

	// multi-sample level
	VkSampleCountFlagBits               m_msaaLevel { VK_SAMPLE_COUNT_1_BIT};

	std::vector<LEPlane>                m_frustumPlanes;
	// decides rendering order
	int                                 m_depth;

	// clear-relative variables
	glm::vec4                           m_clearColor {0,0,0,1.0f};
	float                               m_clearDepth {1.0f};
	float                               m_clearStencil {0.0f};
						               
	// static variables	               
	static LECamera*                    ms_pMainCamera;
	static LECamera*                    ms_pRenderingCamera;
						               
	LEVkCommandBuffer*                  m_pCommandBuffer;

	// clear Color+Z+Stencil
	void                                clearRenderTargetColorDepthStencil(LERenderTarget* pRenderTarget);

	// ZPrePass
	LEVkRenderPass*                     m_ZPreRenderPass;
	LEVkGraphicsPipeline*               m_ZPreRenderPipeline;

	// Grid
	LEVkGraphicsPipeline*               m_gridPipeline;

	// Shadow
	LEVkGraphicsPipeline*               m_shadowMapPipeline;
	LERenderTarget*                     m_shadowMap;

	void                                createZPrePass();

	void                                renderZPrePass();

	void                                createGridPass();

	void                                renderGridPass();

	void                                createShadowPass();
public:
	                                    LECamera();
				                        
	virtual                            ~LECamera();
				                        
	void                                setRenderTarget(LERenderTarget* pRenderTarget);
		                                
	void                                setProjectionMode(ProjectionMode mode);
				                        
	ProjectionMode                      getProjectionMode() const;
				                        
	void                                setFOV(float fov);
		                                
	float                               getFOV() const;
		                                
	void                                setNear(float near);
		                                
	float                               getNear() const;
		                                
	void                                setFar(float);
		                                
	float                               getFar() const;
		                                
	void                                setViewport(float x, float y, float w, float h);
				                        
	glm::vec4                           getViewPort();
				                        
	void                                setPerspective(float fov, float aspect, float near, float far);
				                        
	void                                setLookAt(float centerX,float centerY,float centerZ,
		                                          float upX, float upY, float upZ);
				                        
	void                                setOrthographic(float size, float near, float far);
				                        
	glm::mat4                           getVPMatrix();
				                        
	glm::mat4                           getProjectionMatrix() const;
				                        
	glm::mat4                           getViewMatrix();
				                        
	bool                                testVisiable(LEPrimitiveRenderer* pRenderer) const ;
					                    
	void                                resetRenderContext() {
		this->m_pRenderQueue->reset();
	}
					                    
	void                                addRenderContext(LEPrimitiveRenderer* pRenderer);
					                    
	int                                 getDepth() const;
					                    
	void                                setDepth(int depth);
					                    
	LERenderTarget*                     getRenderTarget() const {
		return this->m_pRenderTarget;
	}
					                    
	void                                render();
					                    
	void                                startRecord();
					                    
	void                                submit();
								       
	static LECamera*                    getMainCamera() { return LECamera::ms_pMainCamera;}
					                    
	static LECamera*                    getRenderingCamera() { return LECamera::ms_pRenderingCamera; }

	LEPrimitiveRenderer*                pickUpRendererOnMousePos();

	void                                drawOutline(VkCommandBuffer commandBuffer);

	const std::vector<LERenderContext>& getRenderContext() { return this->m_pRenderQueue->getRenderContext(); }

	glm::vec3                           getDirection() const { return this->m_lookDirection; }
			                            
	void                                setClearColor(const glm::vec4& color) { this->m_clearColor = color; }
			                            
	glm::vec4                           getClearColor() const { return this->m_clearColor; }
			                            
	void                                setClearDepthStencil(float depth, float stencil) { this->m_clearDepth = depth; this->m_clearStencil = stencil; }

	std::pair<float, float>             getClearDepthStencil() { return { this->m_clearDepth,this->m_clearStencil }; };
							           
	const LEVkCommandBuffer*            getCommandBuffer() const { return this->m_pCommandBuffer; }

	const VkSampleCountFlagBits         getMultiSampleLevel() const { return this->m_msaaLevel; }

#if defined(LE_EDITOR)
	void onPropertyEditorGUI() override;
#endif // !LE_EDITOR
};

#endif // !LE_CAMERA_H