/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_vk_loaded.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_vk_loaded_api.h"
#include "le_vk_variable_manager.h"

LE_IMPL_LOADED_API(vkCmdPushDescriptorSetKHR);

void LELoadedAPI::loadAPIFromDLL() {
	LE_LOAD_API(vkCmdPushDescriptorSetKHR);
}
