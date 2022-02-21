#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_vk_buffer.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_VK_BUFFER_H
#define LE_VK_BUFFER_H

// Vulkan Includes
#include <vulkan/vulkan.h>

// LE Includes
#include "le_vk_device.h"
#include "../Renderer/le_vk_variable_manager.h"
#include "../Renderer/utility/le_memory_type_from_properties.h"
#include "Runtime/Base/le_type_traits.h"

// GLM Includes
#include <glm/glm.hpp>

#include "le_vk_object.h"

namespace Lampyris::Detail{
template<typename  T>
concept LEBufferAssignable = BasicType<T> ||
							 std::is_same_v<glm::vec2, T> ||
							 std::is_same_v<glm::vec3, T> ||
							 std::is_same_v<glm::vec4, T> ||
							 std::is_same_v<glm::mat4, T>;
}
class LEVkBuffer:public LEVulkanObject<VkBuffer> {
private:
	// must be specified when constructed
	uint32_t           m_byteCount;
	uint32_t           m_byteAllocatedCount;
	VkDeviceMemory     m_vkBufferMemory;
	int                m_usage;
	unsigned char*     m_mappedMemory;
public:
		LEVkBuffer(unsigned int byteCount,int usage);

		~LEVkBuffer();

	void map();

	void unmap();

	void flush(VkDeviceSize offset = 0, VkDeviceSize size = 0) const;

	template<typename T> requires Lampyris::Detail::LEBufferAssignable<T>
	T read(uint32_t offset) {
		if (!this->m_mappedMemory || offset >= this->m_byteAllocatedCount) {
			return T();
		}
		return *reinterpret_cast<T*>(this->m_mappedMemory + offset);
	}

	template<typename T> requires Lampyris::Detail::LEBufferAssignable<T>
	void write(T              data,
		       uint32_t       offset) {
		uint32_t size = sizeof(T);
		if (!this->m_mappedMemory || (offset+size) < this->m_byteAllocatedCount) {
			std::memcpy(this->m_mappedMemory+offset, &data, size);
		}
	}

	void writeByte(const unsigned char* pData,
		           uint32_t             size,
		           uint32_t             offset) const;

	uint32_t getSize() const;

	bool isMapped() const{
		return this->m_mappedMemory != nullptr;
	}
};
#endif // !LE_VK_BUFFER_H