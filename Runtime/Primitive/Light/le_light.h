#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Primitive
 * \File:    le_light.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "../../Base/le_component.h"

// GLM Includes
#include <glm/vec4.hpp>

class LEVkGraphicsPipeline;
class LETexture2D;

class LELight:public LEComponent {
	LE_COMPONENT_NAME(LELight);
private:
	LE_INITIAL_DECLARE_NO_CREATE
public:
	enum LightType {
		Directional,
		Spot,
		Point
	};
	// not-implemented
	enum LightMode {
		LM_RealTime,
		LM_Baked,
		LM_MAX_ENUM = 0x7ffffff
	};

	enum ShadowMode {
		SM_Hard, 
		SM_Soft, // not supported yet
		SM_None,
		SM_MAX_ENUM = 0x7ffffff
	};

private:
	static glm::vec4 ms_ambient;

	// the color of light
	glm::vec4        m_color{1.0f,1.0f,0.0f,1.0f};

	// the power of light
	float            m_illuminance{1.0f};
				     
	float            m_spanAngle{10.f};
				     
	// The textur e will be displayed on the surface illuminated by the light (not-implemented)
	LETexture2D*     m_cookieTexture;
				     
	LightType        m_lightType;
				     
	// the method of light rendering
	LightMode        m_lightMode {LM_RealTime};
				     
	// the method of shadow casting
	ShadowMode       m_shadowMode {SM_None};
public:
	                 LELight();

	virtual         ~LELight();

	static void      setAmbient(glm::vec4 rgba) { ms_ambient = rgba; }

	static glm::vec4 getAmbient() { return ms_ambient; }

	float            getSpanAngle() const { return this->m_spanAngle; }

	void             setSpanAngle(float angle) {this->m_spanAngle = angle;}

	glm::vec4        getColor() const { return this->m_color; }

	void             setColor(const glm::vec4& m_color) { this->m_color = m_color; }
		             
	float            getIlluminance() const { return this->m_illuminance;  }
		             
	void             setIlluminance(float illuminance) { this->m_illuminance = illuminance; }
				     
	LETexture2D*     getCookieTexture() const { return m_cookieTexture; }
				     
	void             setCookieTexture(LETexture2D* cookieTexture) { this->m_cookieTexture = cookieTexture; }
				     
	LightType        getLightType() const { return this->m_lightType; }
				     
	void             setLightType(LightType lightType) { this->m_lightType = lightType; }
				     
	LightMode        getLightMode() const { return this->m_lightMode;}
				     
	void             setLightMode(LightMode lightMode) { m_lightMode = lightMode; }
				     
	ShadowMode       getShadowMode() const { return this->m_shadowMode; }
				     
	void             setShadowMode(ShadowMode shadowMode) { this->m_shadowMode = m_shadowMode; }

#if defined(LE_EDITOR)
	void onPropertyEditorGUI() override;
#endif // !LE_EDITOR
};