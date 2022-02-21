#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_vk_sampler.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// STL Includes
#include <unordered_map>
// LE Includes
#include "le_vk_object.h"
#include "../Base/le_hash.h"

class LEVkSampler :public LEVulkanObject<VkSampler> {
	static std::unordered_map<uint64_t, LEVkSampler*> ms_samplerMap;
public:
	using LEVulkanObject::LEVulkanObject;

	static LEVkSampler* getSampler(VkSamplerCreateInfo samplerCreateInfo);

	virtual ~LEVkSampler();
};
