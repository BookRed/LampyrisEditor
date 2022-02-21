/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_vk_buffer.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_vk_buffer.h"

LEVkBuffer::LEVkBuffer(unsigned byteCount, int usage):m_byteCount(byteCount),                             
                                                      m_mappedMemory(nullptr),m_usage(usage) {
	auto [buffer, bufferMemory, byteAllocatedCount] = LEVkDevice::createBuffer(usage,this->m_byteCount);
	this->m_handle = buffer;
	this->m_vkBufferMemory = bufferMemory;
	this->m_byteAllocatedCount = byteAllocatedCount;
}

void LEVkBuffer::map() {
	this->m_mappedMemory = static_cast<unsigned char*>(LEVkDevice::mapMemory(this->m_vkBufferMemory, this->m_byteCount));
}

void LEVkBuffer::unmap() {
	if (this->m_mappedMemory) {
		LEVkDevice::unmapMemory(this->m_vkBufferMemory);
		this->m_mappedMemory = nullptr;
	}
}

void LEVkBuffer::flush(VkDeviceSize offset, VkDeviceSize size) const {
	if(size == 0 && this->m_byteAllocatedCount!=0) {
		size = this->m_byteAllocatedCount;
	}
	LEVkDevice::flushMappedMemoryRanges(this->m_vkBufferMemory,offset,size);
}

void LEVkBuffer::writeByte(const unsigned char* pData, uint32_t size, uint32_t offset) const {
	if (!this->m_mappedMemory || (offset + size) <= this->m_byteAllocatedCount) {
		std::memcpy(this->m_mappedMemory + offset, pData, size);
	}
}

LEVkBuffer::~LEVkBuffer() {
	LEVkDevice::destroyBuffer(this->m_handle, this->m_vkBufferMemory);
}

uint32_t LEVkBuffer::getSize() const {
	return this->m_byteCount;
}
