/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Renderer
 * \File:    le_vk_graphics_pipeline.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_vk_buffer.h"
#include "le_vk_render_pass.h"
#include "le_inherent_ubo.h"
#include "le_render_target.h"
#include "le_vk_graphics_pipeline.h"
#include "le_vk_debugger.h"
#include "le_vk_loaded_api.h"
#include "le_vk_command_buffer.h"
void LEVkGraphicsPipeline::setLayoutBindingForBuffer(const std::vector<LEShaderBufferInfo>& uboInfo, 
	                                                 VkDescriptorSetLayoutBinding* pBindings, 
	                                                 int arrayOffset, 
	                                                 VkShaderStageFlagBits stage, 
	                                                 VkDescriptorType bufferType) {
	for (uint32_t i = 0; i < uboInfo.size(); i++) {
		pBindings[i+arrayOffset].binding            = uboInfo[i].binding;
		pBindings[i+arrayOffset].descriptorType     = bufferType;
		pBindings[i+arrayOffset].descriptorCount    = 1;
		pBindings[i+arrayOffset].stageFlags         = stage;
		pBindings[i+arrayOffset].pImmutableSamplers = nullptr;
		this->m_writeDescSetIndexMap[uboInfo[i].name] = i + arrayOffset;
	}
}

void LEVkGraphicsPipeline::setLayoutBindingForTex(const std::vector<LEShaderTextureInfo>& texInfo, 
	                                              VkDescriptorSetLayoutBinding* pBindings, 
	                                              int arrayOffset, 
	                                              VkShaderStageFlagBits stage) {
	for (uint32_t i = 0; i < texInfo.size(); i++) {
		pBindings[i+arrayOffset].binding            = texInfo[i].binding;
		pBindings[i+arrayOffset].descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		pBindings[i+arrayOffset].descriptorCount    = 1;
		pBindings[i+arrayOffset].stageFlags         = stage;
		pBindings[i+arrayOffset].pImmutableSamplers = nullptr;
		this->m_writeDescSetIndexMap[texInfo[i].name] = i + arrayOffset;
	}
}

LEVkGraphicsPipeline::LEVkGraphicsPipeline(LEPipelineState pipelineState) :
	m_pVShader(nullptr),m_pFShader(nullptr),
	m_pGShader(nullptr),m_pipelineState(pipelineState) {}

LEVkGraphicsPipeline::~LEVkGraphicsPipeline() {}

void LEVkGraphicsPipeline::createDescriptorSetInfo(const std::vector<LEShaderBufferInfo>& uboInfo, VkDescriptorSetLayoutBinding* pBindings, VkDescriptorPoolSize* pPoolSize, int arrayOffset) {
	for (uint32_t i = arrayOffset; i < uboInfo.size(); i++) {
		// pool size
		pPoolSize[i].descriptorCount = 1; // important
		pPoolSize[i].type = pBindings[i].descriptorType;

		// write descriptor set
		// update method:
		// [1]: for data likes scalar, vector, matrix, use 'vkMapMemory'
		// [2]: for data likes sampler, just modify 'pImageInfo'
		this->m_writeDescriptorSet[i]                 = {};
		this->m_writeDescriptorSet[i].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		this->m_writeDescriptorSet[i].pNext           = nullptr;
		this->m_writeDescriptorSet[i].descriptorCount = 1;       // important
		this->m_writeDescriptorSet[i].descriptorType  = pBindings[i].descriptorType; // important
		this->m_writeDescriptorSet[i].pBufferInfo     = nullptr;     // vkUpdateDescriptorSet
		this->m_writeDescriptorSet[i].pImageInfo      = nullptr;      // vkUpdateDescriptorSet
		this->m_writeDescriptorSet[i].dstArrayElement = 0;       // not supported yet
		this->m_writeDescriptorSet[i].dstBinding      = pBindings[i].binding;
	}
}

void LEVkGraphicsPipeline::createPipelineLayout() {
	auto& vsUBOInfo        = this->m_pVShader->getMetaData()->uboInfo;
	auto& fsUBOInfo        = this->m_pFShader->getMetaData()->uboInfo;
	auto& vsSSBOInfo       = this->m_pVShader->getMetaData()->ssboInfo;
	auto& fsSSBOInfo       = this->m_pFShader->getMetaData()->ssboInfo;
	auto& vsTexInfo        = this->m_pVShader->getMetaData()->textureInfo;
	auto& fsTexInfo        = this->m_pFShader->getMetaData()->textureInfo;

	const int vsUBOSize    = vsUBOInfo.size() , fsUBOSize = fsUBOInfo.size();
	const int vsSSBOSize   = vsSSBOInfo.size(),fsSSBOSize = fsSSBOInfo.size();
	const int vsTexSize    = vsTexInfo.size() ,fsTexSize  = fsTexInfo.size();
	const int uboSize      = vsUBOSize + fsUBOSize;
	const int ssboSize     = vsSSBOSize + fsSSBOSize;
	const int texSize      = vsTexSize  + fsTexSize;
	const int bindingCount = uboSize  + ssboSize + texSize;

	// uniform binding
	std::vector<VkDescriptorSetLayoutBinding> bindings(bindingCount);
	this->setLayoutBindingForBuffer(vsUBOInfo, bindings.data(), 0, VK_SHADER_STAGE_VERTEX_BIT, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	this->setLayoutBindingForBuffer(fsUBOInfo, bindings.data(), vsUBOSize, VK_SHADER_STAGE_FRAGMENT_BIT, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	this->setLayoutBindingForBuffer(vsSSBOInfo, bindings.data(), uboSize, VK_SHADER_STAGE_VERTEX_BIT, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
	this->setLayoutBindingForBuffer(fsSSBOInfo, bindings.data(), uboSize + vsSSBOSize, VK_SHADER_STAGE_FRAGMENT_BIT, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
	this->setLayoutBindingForTex(vsTexInfo, bindings.data(), uboSize + ssboSize, VK_SHADER_STAGE_VERTEX_BIT);
	this->setLayoutBindingForTex(fsTexInfo, bindings.data(), uboSize + ssboSize, VK_SHADER_STAGE_FRAGMENT_BIT);

	this->m_descriptorSetLayout = LEVkDevice::createDescriptorSetLayout(bindings);

	// push constant 
	auto& vsPushConstantInfo = this->m_pVShader->getMetaData()->pushConstantInfo;
	const int pushConstantRangesSize = vsPushConstantInfo.blockSize > 0 ? 1 : 0;

	int pushConstantOffset = 0;
	int index = 0;
	std::vector<VkPushConstantRange> pushConstantRanges(pushConstantRangesSize);
	if (vsPushConstantInfo.blockSize > 0) {
		pushConstantRanges[index].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		pushConstantRanges[index].offset     = 0;
		pushConstantRanges[index].size       = this->m_pVShader->getMetaData()->pushConstantInfo.blockSize;
		pushConstantOffset += pushConstantRanges[index].size;
		index++;
	}
	// pipeline layout
	this->m_pipelineLayout = LEVkDevice::createPipelineLayout(pushConstantRanges, this->m_descriptorSetLayout);


	// ensure vkWriteDescriptorSets
	std::vector<VkDescriptorPoolSize> poolSize(uboSize + ssboSize + texSize);

	this->m_writeDescriptorSet.resize(uboSize + ssboSize+ texSize);
	for (uint32_t i = 0; i < uboSize + texSize; i++) {
		this->m_writeDescriptorSet[i]                 = {};
		this->m_writeDescriptorSet[i].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		this->m_writeDescriptorSet[i].pNext           = nullptr;
		this->m_writeDescriptorSet[i].descriptorCount = 1;                          // important
		this->m_writeDescriptorSet[i].descriptorType  = bindings[i].descriptorType; // important
		this->m_writeDescriptorSet[i].pBufferInfo     = nullptr;                    // vkUpdateDescriptorSet
		this->m_writeDescriptorSet[i].pImageInfo      = nullptr;                    // vkUpdateDescriptorSet
		this->m_writeDescriptorSet[i].pTexelBufferView= nullptr;                    // vkUpdateDescriptorSet
		this->m_writeDescriptorSet[i].dstArrayElement = 0;                          // not supported yet
		this->m_writeDescriptorSet[i].dstBinding      = bindings[i].binding;
		poolSize[i].type                              = bindings[i].descriptorType;
		poolSize[i].descriptorCount                   = DESCRIPTOR_COUNT;
	}

	// ensure descriptor set
	this->m_descriptorPool = LEVkDevice::createDescriptorPool(poolSize, DESCRIPTOR_COUNT);
	this->m_descriptorSets = LEVkDevice::allocateDescriptorSet(this->m_descriptorPool, this->m_descriptorSetLayout, DESCRIPTOR_COUNT);

	// inherent UBO
	for(uint32_t i = 0;i<this->m_descriptorSets.size();i++) {
		LEInherentUBO::getInstance()->updateDescriptorSet(this->m_descriptorSets[i]);
		// default textures update
		for (uint32_t j = 0; j < texSize; j++) {
			this->m_writeDescriptorSet[uboSize + j].dstSet     = this->m_descriptorSets[i];
			this->m_writeDescriptorSet[uboSize + j].pImageInfo = &(LETexture2D::getDefaultResource()->getDescriptorImageInfo());
			LEVkDevice::updateDescriptorSets(1, &this->m_writeDescriptorSet[uboSize + j], 0, nullptr);
		}
	}


	// storage buffer
	const int ssboBufferByteCount = this->calculateBufferByteCount(vsSSBOInfo) + this->calculateBufferByteCount(fsSSBOInfo);
	if (ssboBufferByteCount == 0) {
		this->m_pStorageBuffer = nullptr;
	}
	else {
		this->m_pStorageBuffer = new LEVkBuffer(ssboBufferByteCount,VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
	}

}

void LEVkGraphicsPipeline::createShader(VkShaderStageFlagBits shaderStage, VkPipelineShaderStageCreateInfo* pShaderInfo, LEShader* pShader) {
	pShaderInfo->sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pShaderInfo->pNext               = NULL;
	pShaderInfo->flags               = 0;
	pShaderInfo->stage               = shaderStage;
	pShaderInfo->pName               = "main";
	pShaderInfo->module              = pShader->getShaderModule();
	pShaderInfo->pSpecializationInfo = nullptr;
	// specialization constants
	auto& specializationConstantInfo = pShader->getMetaData()->specializationConstantInfo;
	int specializationConstantSize = specializationConstantInfo.size();
	VkSpecializationInfo* pSpecializationInfo = nullptr;
	VkSpecializationMapEntry* pMapEntry = nullptr;
	if (specializationConstantSize > 0) {
		pSpecializationInfo = new VkSpecializationInfo[specializationConstantSize];
		pMapEntry = new VkSpecializationMapEntry[specializationConstantSize];
		int dataSizeSum = 0;
		for (int i = 0; i < specializationConstantSize; i++) {
			int byteCount = s_glslTypeByteCountMap[specializationConstantInfo[i].type];
			pMapEntry[i].constantID = specializationConstantInfo[i].id;
			pMapEntry[i].size = byteCount;
			pMapEntry[i].offset = dataSizeSum;
			dataSizeSum += pMapEntry[i].size;
		}
		VkSpecializationInfo specializationInfo = {};
		specializationInfo.dataSize = dataSizeSum;
		specializationInfo.mapEntryCount = specializationConstantSize;
		specializationInfo.pMapEntries = pMapEntry;
		specializationInfo.pData = new char[dataSizeSum] {0};
	}
}

void LEVkGraphicsPipeline::createRenderPass() {
	if(this->m_pFShader) {
		this->m_pRenderPass = new LEVkRenderPass;
		this->m_pRenderPass->createForwardPass(this->m_pFShader->getMetaData()->outputInfo, VK_SAMPLE_COUNT_1_BIT);
	}
}

void LEVkGraphicsPipeline::createShader() {
	// must contains vertex shader and fragment shader
	this->m_shaderStateCount = 2 + (this->m_pGShader == nullptr ? 0 : 1);
	this->m_pShaderState = new VkPipelineShaderStageCreateInfo[this->m_shaderStateCount];
	this->createShader(VK_SHADER_STAGE_VERTEX_BIT, &this->m_pShaderState[0], this->m_pVShader);
	this->createShader(VK_SHADER_STAGE_FRAGMENT_BIT, &this->m_pShaderState[1], this->m_pFShader);
}

void LEVkGraphicsPipeline::createPipeline() {
	// vertex input of vertex shader
	int totalByteCount = 0;
	auto& inputInfo = this->m_pVShader->getMetaData()->inputInfo;
	VkVertexInputBindingDescription inputBinding[16];
	VkVertexInputAttributeDescription inputAttribute[16];
	for (uint32_t i = 0; i < inputInfo.size(); i++) {
		inputAttribute[i].binding  = inputInfo[i].location;
		inputAttribute[i].location = inputInfo[i].location;
		inputAttribute[i].format   = s_glslTypeFormatMap[inputInfo[i].type];
		inputAttribute[i].offset   = 0;
		// totalByteCount += s_glslTypeByteCountMap[inputInfo[i].type];
		inputBinding[i].binding = inputInfo[i].location;
		inputBinding[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		inputBinding[i].stride = s_glslTypeByteCountMap[inputInfo[i].type];
	}

	VkPipelineVertexInputStateCreateInfo inputState;
	inputState.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	inputState.pNext                           = NULL;
	inputState.flags                           = 0;
	inputState.vertexBindingDescriptionCount   = inputInfo.size();
	inputState.pVertexBindingDescriptions      = inputBinding;
	inputState.vertexAttributeDescriptionCount = inputInfo.size();
	inputState.pVertexAttributeDescriptions    = inputAttribute;

	// dynamic states
	VkDynamicState dynamicStates[2] = { VK_DYNAMIC_STATE_VIEWPORT,VK_DYNAMIC_STATE_SCISSOR };
	VkPipelineDynamicStateCreateInfo dynamicState = {};
	dynamicState.sType                            = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.pNext                            = NULL;
	dynamicState.pDynamicStates                   = dynamicStates;
	dynamicState.dynamicStateCount                = 2;

	VkPipelineInputAssemblyStateCreateInfo assmblyState;
	assmblyState.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	assmblyState.pNext                  = NULL;
	assmblyState.flags                  = 0;
	assmblyState.primitiveRestartEnable = VK_FALSE;
	assmblyState.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; // important

	VkPipelineRasterizationStateCreateInfo rasterizationState;
	rasterizationState.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationState.pNext                   = NULL;
	rasterizationState.flags                   = 0;
	rasterizationState.polygonMode             = VK_POLYGON_MODE_FILL;
	rasterizationState.cullMode                = this->m_pipelineState.cullMode;
	rasterizationState.frontFace               = this->m_pipelineState.frontFace; // important
	rasterizationState.depthClampEnable        = VK_FALSE;
	rasterizationState.rasterizerDiscardEnable = VK_FALSE;
	rasterizationState.depthBiasEnable         = this->m_pipelineState.ZBiasEnabled;
	rasterizationState.depthBiasConstantFactor = 0; // important
	rasterizationState.depthBiasClamp          = 0;
	rasterizationState.depthBiasSlopeFactor    = 0;
	rasterizationState.lineWidth               = 1.0f; // important in line renderer

	VkPipelineColorBlendAttachmentState blendAttachmentState;
	blendAttachmentState.colorWriteMask      =  this->m_pipelineState.makeColorWriteMask();
	blendAttachmentState.blendEnable         =  this->m_pipelineState.blendEnabled;
	blendAttachmentState.alphaBlendOp        =  this->m_pipelineState.alphaBlendOp;
	blendAttachmentState.colorBlendOp        =  this->m_pipelineState.colorBlendOp;
	blendAttachmentState.srcColorBlendFactor =  this->m_pipelineState.srcColorBlendFactor;
	blendAttachmentState.dstColorBlendFactor =  this->m_pipelineState.dstColorBlendFactor;
	blendAttachmentState.srcAlphaBlendFactor =  this->m_pipelineState.srcAlphaBlendFactor;
	blendAttachmentState.dstAlphaBlendFactor =  this->m_pipelineState.dstAlphaBlendFactor;

	VkPipelineColorBlendStateCreateInfo blendState;
	blendState.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	blendState.pNext           = NULL;
	blendState.flags           = 0;
	blendState.attachmentCount = 1;
	blendState.pAttachments    = &blendAttachmentState;
	blendState.logicOpEnable   = VK_FALSE;
	blendState.logicOp         = VK_LOGIC_OP_NO_OP;
	// important
	blendState.blendConstants[0] = 1.0f;
	blendState.blendConstants[1] = 1.0f;
	blendState.blendConstants[2] = 1.0f;
	blendState.blendConstants[3] = 1.0f;

	// important
	VkViewport viewports;
	viewports.minDepth = 0.0f;
	viewports.maxDepth = 1.0f;
	viewports.x        = 0;
	viewports.y        = 0;
	viewports.width    = 0;
	viewports.height   = 0;

	VkRect2D scissor;
	scissor.extent.width  = 0;
	scissor.extent.height = 0;
	scissor.offset.x      = 0;
	scissor.offset.y      = 0;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.pNext         = NULL;
	viewportState.flags         = 0;
	viewportState.viewportCount = 1;
	viewportState.scissorCount  = 1;
	viewportState.pScissors     = &scissor;
	viewportState.pViewports    = &viewports;

	VkPipelineDepthStencilStateCreateInfo depthStencilState;
	depthStencilState.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilState.pNext                 = NULL;
	depthStencilState.flags                 = 0;
	depthStencilState.depthTestEnable       = this->m_pipelineState.ZTestEnabled;   // important
	depthStencilState.depthWriteEnable      = this->m_pipelineState.ZWriteEnabled;  // important
	depthStencilState.depthCompareOp        = VK_COMPARE_OP_LESS_OR_EQUAL; // important
	depthStencilState.depthBoundsTestEnable = VK_FALSE;
	depthStencilState.minDepthBounds        = 0;
	depthStencilState.maxDepthBounds        = 0;
	depthStencilState.stencilTestEnable     = this->m_pipelineState.stencilTestEnabled; // important
	depthStencilState.back.failOp           = this->m_pipelineState.stencilFailOp;
	depthStencilState.back.passOp           = this->m_pipelineState.stencilPassOp;
	depthStencilState.back.compareOp        = this->m_pipelineState.stencilCompareOp;
	depthStencilState.back.compareMask      = 0;
	depthStencilState.back.reference        = 0;
	depthStencilState.back.depthFailOp      = VK_STENCIL_OP_KEEP;
	depthStencilState.back.writeMask        = 0;
	depthStencilState.front                 = depthStencilState.back;

	VkPipelineMultisampleStateCreateInfo multiSampleState;
	multiSampleState.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multiSampleState.pNext                 = NULL;
	multiSampleState.flags                 = 0;
	multiSampleState.pSampleMask           = NULL;
	multiSampleState.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
	multiSampleState.sampleShadingEnable   = VK_FALSE;
	multiSampleState.alphaToCoverageEnable = VK_FALSE;
	multiSampleState.alphaToOneEnable      = VK_FALSE;
	multiSampleState.minSampleShading      = 0.0;

	std::vector<VkFormat> colorAttachmentFormats;
	auto& outputInfo = this->m_pFShader->getMetaData()->outputInfo;
	colorAttachmentFormats.resize(outputInfo.size());
	for (uint32_t i = 0; i < outputInfo.size(); i++) {
		colorAttachmentFormats[i] = VK_FORMAT_B8G8R8A8_UNORM;
	}

	VkPipelineRenderingCreateInfoKHR renderCreateInfo;
	renderCreateInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
	renderCreateInfo.pNext                   = nullptr;
	renderCreateInfo.viewMask                = 0;
	renderCreateInfo.colorAttachmentCount    = colorAttachmentFormats.size();
	renderCreateInfo.pColorAttachmentFormats = colorAttachmentFormats.data();
	renderCreateInfo.depthAttachmentFormat   = VK_FORMAT_D24_UNORM_S8_UINT;
	renderCreateInfo.stencilAttachmentFormat = VK_FORMAT_D24_UNORM_S8_UINT;

	VkGraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.pNext               = &renderCreateInfo;
	pipelineInfo.layout              = this->m_pipelineLayout;
	pipelineInfo.basePipelineHandle  = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex   = 0;
	pipelineInfo.flags               = 0;
	pipelineInfo.pVertexInputState   = &inputState;
	pipelineInfo.pInputAssemblyState = &assmblyState;
	pipelineInfo.pRasterizationState = &rasterizationState;
	pipelineInfo.pColorBlendState    = &blendState;
	pipelineInfo.pTessellationState  = nullptr;
	pipelineInfo.pMultisampleState   = &multiSampleState;
	pipelineInfo.pDynamicState       = &dynamicState;
	pipelineInfo.pViewportState      = &viewportState;
	pipelineInfo.pDepthStencilState  = &depthStencilState;
	pipelineInfo.pStages             = this->m_pShaderState;
	pipelineInfo.stageCount          = this->m_shaderStateCount;
	pipelineInfo.renderPass          = nullptr;
	pipelineInfo.subpass             = 0;

	this->m_pipelineCache = LEVkDevice::createGraphicsPipelineCache();
	this->m_handle        = LEVkDevice::createGraphicsPipeline(this->m_pipelineCache,pipelineInfo);

}

void LEVkGraphicsPipeline::destroyShader() {}

void LEVkGraphicsPipeline::destroyPipeline() {}

void LEVkGraphicsPipeline::destroyUniformBuffer() {}

void LEVkGraphicsPipeline::destroyPipelineLayout() {}

void LEVkGraphicsPipeline::setVertexShader(LEShader* pShader) {
	this->m_pVShader = pShader;
}

void LEVkGraphicsPipeline::setFragmentShader(LEShader* pShader) {
	this->m_pFShader = pShader;
}

void LEVkGraphicsPipeline::buildPipeline() {
	LERenderTarget* pRenderTarget = LECamera::getRenderingCamera()->getRenderTarget();
	if(!this->m_pRecordedRenderTargets.contains(pRenderTarget)) {
		// this->createRenderPass();
		this->createShader();
		this->createPipelineLayout();
		this->createPipeline();
		this->m_pRecordedRenderTargets.insert(pRenderTarget);
		return;
	}
	if (pRenderTarget->isResized()) {
		// only recreate pipeline
		this->createPipeline();
	}
}

void LEVkGraphicsPipeline::pushConstantInternal(const std::string& name, const char* typeName, VkShaderStageFlagBits stage, void* pData) {
	int offset = -1;
	int byteCount = -1;
	const std::vector<LEShaderMemberInfo>* pMemberInfo = nullptr;
	pMemberInfo = &(this->m_pVShader->getMetaData()->pushConstantInfo.members);
	if (pMemberInfo == nullptr) {
		return;
	}
	this->locateMemberByteCountAndOffset(name, typeName, *pMemberInfo, byteCount, offset);
	if (offset != -1 && byteCount != -1) {
		LECamera::getRenderingCamera()->getCommandBuffer()->pushConstants(this->m_pipelineLayout, stage, offset, byteCount, pData);
	}
}

bool LEVkGraphicsPipeline::locateMemberByteCountAndOffset(const std::string& name, const char* typeName, const std::vector<LEShaderMemberInfo>& memberInfo, int& byteCount, int& offset) {
	for (auto& member : memberInfo) {
		if (member.name == name) {
			if (member.type == typeName) {
				offset = member.offset;
				byteCount = s_glslTypeByteCountMap[member.type];
				return true;
			}
		}
	}
	return false;
}

void LEVkGraphicsPipeline::pushConstantVector4(const std::string& name, VkShaderStageFlagBits stage, glm::vec4 vec) {
	if (this->m_vkCommandBuffer == VK_NULL_HANDLE) {
		return;
	}
	this->pushConstantInternal(name, "vec4", stage, &vec);
}

void LEVkGraphicsPipeline::pushConstantMatrix(const std::string& name, VkShaderStageFlagBits stage, glm::mat4 mat) {
	if (this->m_vkCommandBuffer == VK_NULL_HANDLE) {
		return;
	}
	this->pushConstantInternal(name, "mat4", stage, &mat);
}

void LEVkGraphicsPipeline::pushConstantUInt(const std::string& name, VkShaderStageFlagBits stage,unsigned int value) {
	if (this->m_vkCommandBuffer == VK_NULL_HANDLE) {
		return;
	}
	this->pushConstantInternal(name, "uint", stage, &value);
}

void LEVkGraphicsPipeline::setUniformInternal(const std::string& name, const char* typeName, int descriptorID, VkShaderStageFlagBits stage, void* pData) {
	int offset = -1;
	int byteCount = -1;
	const std::vector<LEShaderMemberInfo>* pMemberInfo = nullptr;
	if (stage == VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT) {
		for (int i = 0; i < this->m_pVShader->getMetaData()->uboInfo.size(); i++) {
			pMemberInfo = &(this->m_pVShader->getMetaData()->uboInfo[i].members);
			if (this->locateMemberByteCountAndOffset(name, typeName, *pMemberInfo, byteCount, offset)) {
				break;
			}
		}
	}
	else if (stage == VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT) {
		for (int i = 0; i < this->m_pFShader->getMetaData()->uboInfo.size(); i++) {
			pMemberInfo = &(this->m_pFShader->getMetaData()->uboInfo[i].members);
			if (this->locateMemberByteCountAndOffset(name, typeName, *pMemberInfo, byteCount, offset)) {
				break;
			}
		}
	}
	if (offset != -1 && byteCount != -1) {
		int writeDescindex = this->m_writeDescSetIndexMap[name];
		int uboSize = this->m_writeDescSetIndexMap.size();
		auto& writeDescSet = this->m_writeDescriptorSet[writeDescindex];
		// writeDescSet.dstSet = this->m_descriptorSets[descriptorID];
		writeDescSet.dstSet = nullptr;
		writeDescSet.pBufferInfo = &(this->m_pBufferDesc[descriptorID * uboSize + writeDescindex]);

		this->m_pUniformBuffer->map();
		this->m_pUniformBuffer->writeByte((unsigned char*)pData, byteCount, uboSize * this->m_pUniformBuffer->getSize() / 1 + offset);
		this->m_pUniformBuffer->unmap();
		::vkUpdateDescriptorSets(vkVariableManager->vkDevice, 1, &writeDescSet, 0, nullptr);
	}
}

void LEVkGraphicsPipeline::setUniformVector2(const std::string& name, uint32_t descriptorID, VkShaderStageFlagBits stage, glm::vec2 vec) {
	if (this->m_vkCommandBuffer == VK_NULL_HANDLE) {
		return;
	}
	this->setUniformInternal(name, "vec2", descriptorID, stage, &vec);
}

void LEVkGraphicsPipeline::setUniformVector3(const std::string& name, uint32_t descriptorID, VkShaderStageFlagBits stage, glm::vec3 vec) {
	if (this->m_vkCommandBuffer == VK_NULL_HANDLE) {
		return;
	}
	this->setUniformInternal(name, "vec3", descriptorID, stage, &vec);
}

void LEVkGraphicsPipeline::setUniformVector4(const std::string& name, uint32_t descriptorID, VkShaderStageFlagBits stage, glm::vec4 vec) {
	if (this->m_vkCommandBuffer == VK_NULL_HANDLE) {
		return;
	}
	this->setUniformInternal(name, "vec4", descriptorID, stage, &vec);
}

void LEVkGraphicsPipeline::setUniformMatrix(const std::string& name, uint32_t descriptorID, VkShaderStageFlagBits stage, glm::mat4 mat) {
	if (this->m_vkCommandBuffer == VK_NULL_HANDLE) {
		return;
	}
	this->setUniformInternal(name, "mat4", descriptorID, stage, &mat);
}

void LEVkGraphicsPipeline::setTexture2D(const std::string& name, uint32_t descriptorID,VkShaderStageFlagBits stage, LETexture2D* pTexture2D) {
	if (!this->m_writeDescSetIndexMap.contains(name)) {
		return;
	}
	// texture ubo
	uint32_t writeDescIndex = this->m_writeDescSetIndexMap[name];
	this->m_writeDescriptorSet[writeDescIndex].dstSet          = this->m_descriptorSets[descriptorID];
	this->m_writeDescriptorSet[writeDescIndex].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	this->m_writeDescriptorSet[writeDescIndex].pNext           = nullptr;
	this->m_writeDescriptorSet[writeDescIndex].dstBinding      = 3;
	this->m_writeDescriptorSet[writeDescIndex].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	this->m_writeDescriptorSet[writeDescIndex].pImageInfo      = &pTexture2D->getDescriptorImageInfo();
	this->m_writeDescriptorSet[writeDescIndex].descriptorCount = 1;
	vkUpdateDescriptorSets(vkVariableManager->vkDevice, 1, &this->m_writeDescriptorSet[writeDescIndex], 0, nullptr);
}

void LEVkGraphicsPipeline::setTextureCube(const std::string& name, uint32_t descriptorID,VkShaderStageFlagBits stage, LECubemap* pTexture2D) {
	if (!this->m_writeDescSetIndexMap.contains(name)) {
		return;
	}
	// texture ubo
	int writeDescIndex = this->m_writeDescSetIndexMap[name];
	this->m_writeDescriptorSet[writeDescIndex].dstSet          = this->m_descriptorSets[descriptorID];
	this->m_writeDescriptorSet[writeDescIndex].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	this->m_writeDescriptorSet[writeDescIndex].pNext           = nullptr;
	this->m_writeDescriptorSet[writeDescIndex].dstBinding      = 1;
	this->m_writeDescriptorSet[writeDescIndex].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	this->m_writeDescriptorSet[writeDescIndex].pImageInfo      = &pTexture2D->getDescriptorImageInfo();
	this->m_writeDescriptorSet[writeDescIndex].descriptorCount = 1;
	vkUpdateDescriptorSets(vkVariableManager->vkDevice, 1, &this->m_writeDescriptorSet[writeDescIndex], 0, nullptr);
}

uint32_t LEVkGraphicsPipeline::getRequiredVertexFormatMask() const {
	if (!this->m_requiredVertexFormatMask) {
		LE_ASSERT_MSG(this->m_pVShader, "vertex shader can not be nullptr");
		auto& inputInfo = this->m_pVShader->getMetaData()->inputInfo;
		for (const auto& input : inputInfo) {
			const uint32_t location = input.location;
			this->m_requiredVertexFormatMask |= (1 << location);
		}
	}
	return this->m_requiredVertexFormatMask;
}

LEVkGraphicsPipeline* LEVkGraphicsPipeline::createFromShader(LEShader* pVSShader, LEShader* pFSShader) {
	if (!pVSShader || !pFSShader) {
		return nullptr;
	}
	static std::unordered_map<int, LEVkGraphicsPipeline*> pipelineMap;
	int hashValue = le_hash_value(pVSShader, pVSShader);
	if (pipelineMap.contains(hashValue)) {
		return pipelineMap[hashValue];
	}
	// read pipeline stage from specialization constants
	auto &constants = pVSShader->getMetaData()->specializationConstantInfo;
	LEPipelineState pipelineState;

	for(auto& constant:constants) {
		if (constant.id == 0) {
			pipelineState.lod = constant.defaultValue.uintValue;
		}
		else if (constant.id == 1) {
			const uint32_t value = constant.defaultValue.uintValue;
			if (value == 0) {
				pipelineState.cullMode = VK_CULL_MODE_FRONT_BIT;
			}
			else {
				pipelineState.cullMode = VK_CULL_MODE_BACK_BIT;
			}
		}
		else if (constant.id == 2) {
			pipelineState.ZWriteEnabled = constant.defaultValue.boolValue;
		}
		else if (constant.id == 3) {
			pipelineState.ZTestEnabled = constant.defaultValue.boolValue;
		}
		else if (constant.id == 4) {
			pipelineState.ZCompareOp = static_cast<VkCompareOp>(constant.defaultValue.uintValue);
		}
		else if (constant.id == 5) {
			pipelineState.stencilWriteEnabled = constant.defaultValue.boolValue;
		}
		else if (constant.id == 6) {
			pipelineState.stencilTestEnabled = constant.defaultValue.boolValue;
		}
		else if (constant.id == 7) {
			pipelineState.stencilCompareOp = static_cast<VkCompareOp>(constant.defaultValue.uintValue);
		}
		else if (constant.id == 8) {
			pipelineState.stencilFailOp = static_cast<VkStencilOp>(constant.defaultValue.uintValue);
		}
		else if (constant.id == 9) {
			pipelineState.stencilPassOp = static_cast<VkStencilOp>(constant.defaultValue.uintValue);
		}
		else if (constant.id == 10) {
			pipelineState.blendEnabled = constant.defaultValue.boolValue;
		}
		else if (constant.id == 11) {
			uint32_t blendMask = constant.defaultValue.uintValue;
			pipelineState.srcColorBlendFactor = static_cast<VkBlendFactor>(constant.defaultValue.uintValue / 100);
			pipelineState.dstColorBlendFactor = static_cast<VkBlendFactor>(constant.defaultValue.uintValue % 100);
		}
		else if (constant.id == 12) {
			pipelineState.colorBlendOp = static_cast<VkBlendOp>(constant.defaultValue.uintValue);
		}
		else if (constant.id == 13) {
			pipelineState.ColorMask.red = static_cast<bool>((constant.defaultValue.uintValue >> 3) & 1);
			pipelineState.ColorMask.green = static_cast<bool>((constant.defaultValue.uintValue >> 2) & 1);
			pipelineState.ColorMask.blue= static_cast<bool>((constant.defaultValue.uintValue >> 1) & 1);
			pipelineState.ColorMask.alpha = static_cast<bool>(constant.defaultValue.uintValue & 1);
			pipelineState.makeColorWriteMask();
		}
	}
	LEVkGraphicsPipeline* pPipeline = new LEVkGraphicsPipeline(pipelineState);
	pPipeline->setVertexShader(pVSShader);
	pPipeline->setFragmentShader(pFSShader);
	pipelineMap[hashValue] = pPipeline;
	return pPipeline;
}

int LEVkGraphicsPipeline::requestDescriptorIDForMaterial() {
	return this->m_descriptorSetUseCount++;
}

int LEVkGraphicsPipeline::calculateBufferByteCount(const std::vector<LEShaderBufferInfo>& uboInfo) {
	int bufferByteCount = 0;
	// ignore inherent ubo 
	for (int i = 0; i < uboInfo.size(); i++) {
		if (uboInfo[i].name == "SpatialUBO" || uboInfo[i].name == "LightUBO") {
			continue;
		}
		bufferByteCount += s_glslTypeByteCountMap[uboInfo[i].members[i].type];
	}
	return bufferByteCount;
}

void LEVkGraphicsPipeline::updateInherentUBODescriptor() {
	// inherent ubo
	this->m_writeDescriptorSet[0].pBufferInfo = &LEInherentUBO::getInstance()->getCameraUBOInfo();
	vkUpdateDescriptorSets(vkVariableManager->vkDevice, 1, &(this->m_writeDescriptorSet[0]), 0, nullptr);
}

uint32_t LEVkGraphicsPipeline::getInputCount() const {
	auto& inputInfo = this->m_pVShader->getMetaData()->inputInfo;
	return inputInfo.size();
}


