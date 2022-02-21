#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Common
 * \File:    le_memory_Type_from_properties.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// Vulkan Includes
#include <vulkan/vulkan.h>

static bool le_memory_type_from_properties(VkPhysicalDeviceMemoryProperties& props,
	                                       uint32_t                          typeBit, 
	                                       VkFlags                           reqMask, 
	                                       uint32_t*                         typeIndex) {
	for (int i = 0; i < 32; i++) {
		if ((typeBit & 1) == 1) {
			if ((props.memoryTypes[i].propertyFlags & reqMask) == reqMask) {
				*typeIndex = i;
				return true;
			}
		}
		typeBit >>= 1;
	}
	return false;
}