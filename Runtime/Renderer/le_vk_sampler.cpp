/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_vk_sampler.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#include "le_vk_device.h"
#include "le_vk_sampler.h"

std::unordered_map<uint64_t, LEVkSampler*> LEVkSampler::ms_samplerMap;

LEVkSampler* LEVkSampler::getSampler(VkSamplerCreateInfo samplerCreateInfo) {
	uint64_t hashValue = le_hash_value(
		samplerCreateInfo.sType,
		samplerCreateInfo.magFilter,
		samplerCreateInfo.minFilter,
		samplerCreateInfo.mipmapMode,
		samplerCreateInfo.addressModeU,
		samplerCreateInfo.addressModeV,
		samplerCreateInfo.addressModeW,
		samplerCreateInfo.mipLodBias,
		samplerCreateInfo.anisotropyEnable,
		samplerCreateInfo.maxAnisotropy,
		samplerCreateInfo.compareOp,
		samplerCreateInfo.minLod,
		samplerCreateInfo.maxLod,
		samplerCreateInfo.compareEnable,
		samplerCreateInfo.borderColor);
	if (ms_samplerMap.count(hashValue) > 0) {
		return ms_samplerMap[hashValue];
	}
	VkSampler sampler = LEVkDevice::createSampler(samplerCreateInfo);
	LEVkSampler* pSampler = new LEVkSampler;
	pSampler->m_handle = sampler;
	ms_samplerMap[hashValue] = pSampler;
	return pSampler;
}

LEVkSampler::~LEVkSampler() {
	vkDestroySampler(vkVariableManager->vkDevice, this->m_handle, nullptr);
}
