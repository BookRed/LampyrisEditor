/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Primitive
 * \File:    le_light.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// 
#include "le_light.h"
#include "../../Base/le_class_info.h"
#include "../../Base/le_entity.h"
#include "../../Base/le_hash.h"
#include "../../LevelManage/le_world.h"
#include "../../LevelManage/le_level.h"

LE_INITIAL_IMPL_NO_CREATE_BEGIN(LELight)
	{
	LE_PROPERTY_REGISTER_METHOD(&LELight::initProperty);
	LE_PROPERTY_DESTORY_METHOD(&LELight::terminateProperty);
}
LE_INITIAL_IMPL_NO_CREATE_END(LELight)

LE_ADD_PROPERTY_BEGIN(LELight)
{
	LE_ADD_PROPERTY_OBJECT(LELight, m_color);
	LE_ADD_PROPERTY_RANGE_NOUSE(LELight, m_illuminance);
}
LE_ADD_PROPERTY_END

glm::vec4 LELight::ms_ambient = glm::vec4(0.8437f,0.8614,1.0f,1.0f);

LELight::LELight() {
	LEWorld::getInstance()->getActiveLevel()->addLight(this);
}

LELight::~LELight() {
	LEWorld::getInstance()->getActiveLevel()->removeLight(this);
}


#if defined(LE_EDITOR)
#include "Editor/WrappedQt/le_editor_gui_layout.h"
void LELight::onPropertyEditorGUI() {
	uint32_t hash = std::hash<std::string>().operator()("Light");
	LEEditorGUILayout::componentSection(le_hash_value(hash, __LINE__), "Light", true);
	this->m_lightType = static_cast<LightType>(LEEditorGUILayout::comboBox(le_hash_value(hash, __LINE__), 
		                                      "Light Type", 
                                              { "Directional","Spot","Point" }));
	this->m_shadowMode = static_cast<ShadowMode>(LEEditorGUILayout::comboBox(le_hash_value(hash, __LINE__), 
		                                         "Shadow Type", 
		                                        { "Soft","Hard","None" },2));
	this->m_lightMode = static_cast<LightMode>(LEEditorGUILayout::comboBox(le_hash_value(hash, __LINE__), 
		                                       "Light Mode",
		                                       { "Real Time","Baked" },0));
	if(this->m_lightType == LightType::Point) {
		LEEditorGUILayout::tipBox(le_hash_value(hash, __LINE__), 
			                      "Point Light is not supported yet!!", 
			                      LEEditorWrappedTipBox::Information);
	}
	this->ms_ambient = LEEditorGUILayout::colorField(le_hash_value(hash, __LINE__), "Amibent");
	this->m_color = LEEditorGUILayout::colorField(le_hash_value(hash, __LINE__), "Color");
}
#endif // !LE_EDITOR