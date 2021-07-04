#include "Rpch.h"
#include <../Pipeline/RPipeline.h>
#include <../Core/RUtils.h>


RrError RrCreatePipelineLayout(RrLogicalDevice device,
	const RrPipelineLayoutCreateInfo* pCreateInfo, RrPipelineLayout* pLayout)
{
	VkPipelineLayoutCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	createInfo.flags = 0;
	createInfo.pNext = NULL;
	createInfo.setLayoutCount = pCreateInfo->setLayoutCount;
	createInfo.pushConstantRangeCount = pCreateInfo->pushConstantRangeCount;

	VkPushConstantRange* pushConstants =
		malloc(sizeof(VkPushConstantRange) * pCreateInfo->pushConstantRangeCount);
	for (uint32_t i = 0; i < pCreateInfo->pushConstantRangeCount; ++i)
	{
		pushConstants[i].size = pCreateInfo->pPushConstantRanges[i].size;
		pushConstants[i].offset = pCreateInfo->pPushConstantRanges[i].offset;
		pushConstants[i].stageFlags =
			(VkShaderStageFlags)pCreateInfo->pPushConstantRanges[i].stageFlags;
	}

	createInfo.pSetLayouts = (VkDescriptorSetLayout*)pCreateInfo->pSetLayouts;
	createInfo.pPushConstantRanges = pushConstants;

	RrError error = GetError(
		vkCreatePipelineLayout((VkDevice)device, &createInfo, NULL, (VkPipelineLayout*)pLayout));

	free(pushConstants);
	return error;
}

RrError RrCreateGraphicsPipeline(RrLogicalDevice device, RrPipelineCache pipelineCache,
	const RrGraphicsPipelineCreateInfo* pCreateInfo, RrPipeline* pPipeline)
{
	VkGraphicsPipelineCreateInfo createInfo;
	memset(&createInfo, 0, sizeof(createInfo));
	VkPipelineShaderStageCreateInfo* shaderStages =
		malloc(sizeof(VkPipelineShaderStageCreateInfo) * pCreateInfo->stageCount);
	for (uint32_t i = 0; i < pCreateInfo->stageCount; ++i)
	{
		shaderStages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[i].pNext = NULL;
		shaderStages[i].flags = 0;
		shaderStages[i].stage = pCreateInfo->pStages[i].stage;
		shaderStages[i].module = (VkShaderModule)pCreateInfo->pStages[i].module;
		shaderStages[i].pName = pCreateInfo->pStages[i].pName;
		shaderStages[i].pSpecializationInfo = NULL;
	}


	VkPipelineVertexInputStateCreateInfo vertexInputState;
	vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputState.pNext = NULL;
	vertexInputState.flags = 0;
	vertexInputState.vertexBindingDescriptionCount =
		pCreateInfo->pVertexInputState->vertexBindingDescriptionCount;
	vertexInputState.vertexAttributeDescriptionCount =
		pCreateInfo->pVertexInputState->vertexAttributeDescriptionCount;

	VkVertexInputAttributeDescription* attributeDescriptions =
		malloc(sizeof(VkVertexInputAttributeDescription) *
			   pCreateInfo->pVertexInputState->vertexAttributeDescriptionCount);
	VkVertexInputBindingDescription* bindingDescriptions =
		malloc(sizeof(VkVertexInputBindingDescription) *
			   pCreateInfo->pVertexInputState->vertexBindingDescriptionCount);
	for (uint32_t i = 0; i < pCreateInfo->pVertexInputState->vertexAttributeDescriptionCount; ++i)
	{
		attributeDescriptions[i].binding =
			pCreateInfo->pVertexInputState->pVertexAttributeDescriptions[i].binding;
		attributeDescriptions[i].format =
			pCreateInfo->pVertexInputState->pVertexAttributeDescriptions[i].format;
		attributeDescriptions[i].location =
			pCreateInfo->pVertexInputState->pVertexAttributeDescriptions[i].location;
		attributeDescriptions[i].offset =
			pCreateInfo->pVertexInputState->pVertexAttributeDescriptions[i].offset;
	}
	for (uint32_t i = 0; i < pCreateInfo->pVertexInputState->vertexBindingDescriptionCount; ++i)
	{
		bindingDescriptions[i].binding =
			pCreateInfo->pVertexInputState->pVertexBindingDescriptions[i].binding;
		bindingDescriptions[i].inputRate =
			pCreateInfo->pVertexInputState->pVertexBindingDescriptions[i].inputRate;
		bindingDescriptions[i].stride =
			pCreateInfo->pVertexInputState->pVertexBindingDescriptions[i].stride;
	}
	vertexInputState.pVertexAttributeDescriptions = attributeDescriptions;
	vertexInputState.pVertexBindingDescriptions = bindingDescriptions;


	VkPipelineInputAssemblyStateCreateInfo inputAssembly;
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.pNext = NULL;
	inputAssembly.flags = 0;
	inputAssembly.topology = pCreateInfo->pInputAssemblyState->topology;
	inputAssembly.primitiveRestartEnable = pCreateInfo->pInputAssemblyState->primitiveRestartEnable;


	VkPipelineTessellationStateCreateInfo tesselationState;
	if (pCreateInfo->pTessellationState)
	{
		tesselationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		tesselationState.pNext = NULL;
		tesselationState.flags = 0;
		tesselationState.patchControlPoints = pCreateInfo->pTessellationState->patchControlPoints;
		createInfo.pTessellationState = &tesselationState;
	}


	VkPipelineViewportStateCreateInfo viewportState;
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.pNext = NULL;
	viewportState.flags = 0;
	viewportState.viewportCount = pCreateInfo->pViewportState->viewportCount;
	viewportState.pViewports = (VkViewport*)pCreateInfo->pViewportState->pViewports;
	viewportState.scissorCount = pCreateInfo->pViewportState->scissorCount;
	viewportState.pScissors = (VkRect2D*)pCreateInfo->pViewportState->pScissors;


	VkPipelineRasterizationStateCreateInfo rasterizationState;
	rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationState.pNext = NULL;
	rasterizationState.flags = 0;
	rasterizationState.depthClampEnable = pCreateInfo->pRasterizationState->depthClampEnable;
	rasterizationState.rasterizerDiscardEnable =
		pCreateInfo->pRasterizationState->rasterizerDiscardEnable;
	rasterizationState.polygonMode = pCreateInfo->pRasterizationState->polygonMode;
	rasterizationState.cullMode = pCreateInfo->pRasterizationState->cullMode;
	rasterizationState.frontFace = pCreateInfo->pRasterizationState->frontFace;
	rasterizationState.depthBiasEnable = pCreateInfo->pRasterizationState->depthBiasEnable;
	rasterizationState.depthBiasConstantFactor =
		pCreateInfo->pRasterizationState->depthBiasConstantFactor;
	rasterizationState.depthBiasClamp = pCreateInfo->pRasterizationState->depthBiasClamp;
	rasterizationState.depthBiasSlopeFactor =
		pCreateInfo->pRasterizationState->depthBiasSlopeFactor;
	rasterizationState.lineWidth = pCreateInfo->pRasterizationState->lineWidth;


	VkPipelineMultisampleStateCreateInfo multisampleState;
	multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleState.pNext = NULL;
	multisampleState.flags = 0;
	multisampleState.rasterizationSamples = pCreateInfo->pMultisampleState->rasterizationSamples;
	multisampleState.sampleShadingEnable = pCreateInfo->pMultisampleState->sampleShadingEnable;
	multisampleState.minSampleShading = pCreateInfo->pMultisampleState->minSampleShading;
	multisampleState.pSampleMask = pCreateInfo->pMultisampleState->pSampleMask;
	multisampleState.alphaToCoverageEnable = pCreateInfo->pMultisampleState->alphaToCoverageEnable;
	multisampleState.alphaToOneEnable = pCreateInfo->pMultisampleState->alphaToOneEnable;


	VkStencilOpState front;
	VkStencilOpState back;
	VkPipelineDepthStencilStateCreateInfo depthStencilState;
	if (pCreateInfo->pDepthStencilState)
	{
		front.failOp = pCreateInfo->pDepthStencilState->front.failOp;
		front.passOp = pCreateInfo->pDepthStencilState->front.passOp;
		front.depthFailOp = pCreateInfo->pDepthStencilState->front.depthFailOp;
		front.compareOp = pCreateInfo->pDepthStencilState->front.compareOp;
		front.compareMask = pCreateInfo->pDepthStencilState->front.compareMask;
		front.writeMask = pCreateInfo->pDepthStencilState->front.writeMask;
		front.reference = pCreateInfo->pDepthStencilState->front.reference;

		back.failOp = pCreateInfo->pDepthStencilState->back.failOp;
		back.passOp = pCreateInfo->pDepthStencilState->back.passOp;
		back.depthFailOp = pCreateInfo->pDepthStencilState->back.depthFailOp;
		back.compareOp = pCreateInfo->pDepthStencilState->back.compareOp;
		back.compareMask = pCreateInfo->pDepthStencilState->back.compareMask;
		back.writeMask = pCreateInfo->pDepthStencilState->back.writeMask;
		back.reference = pCreateInfo->pDepthStencilState->back.reference;

		depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilState.pNext = NULL;
		depthStencilState.flags = 0;
		depthStencilState.depthTestEnable = pCreateInfo->pDepthStencilState->depthTestEnable;
		depthStencilState.depthWriteEnable = pCreateInfo->pDepthStencilState->depthWriteEnable;
		depthStencilState.depthCompareOp = pCreateInfo->pDepthStencilState->depthCompareOp;
		depthStencilState.depthBoundsTestEnable =
			pCreateInfo->pDepthStencilState->depthBoundsTestEnable;
		depthStencilState.stencilTestEnable = pCreateInfo->pDepthStencilState->stencilTestEnable;
		depthStencilState.front = front;
		depthStencilState.back = back;
		depthStencilState.minDepthBounds = pCreateInfo->pDepthStencilState->minDepthBounds;
		depthStencilState.maxDepthBounds = pCreateInfo->pDepthStencilState->maxDepthBounds;

		createInfo.pDepthStencilState = &depthStencilState;
	}


	VkPipelineColorBlendAttachmentState* colorBlendAttachments =
		malloc(sizeof(VkPipelineColorBlendAttachmentState) *
			   pCreateInfo->pColorBlendState->attachmentCount);
	for (uint32_t i = 0; i < pCreateInfo->pColorBlendState->attachmentCount; ++i)
	{
		colorBlendAttachments[i].blendEnable =
			pCreateInfo->pColorBlendState->pAttachments[i].blendEnable;
		colorBlendAttachments[i].srcColorBlendFactor =
			pCreateInfo->pColorBlendState->pAttachments[i].srcColorBlendFactor;
		colorBlendAttachments[i].dstColorBlendFactor =
			pCreateInfo->pColorBlendState->pAttachments[i].dstColorBlendFactor;
		colorBlendAttachments[i].colorBlendOp =
			pCreateInfo->pColorBlendState->pAttachments[i].colorBlendOp;
		colorBlendAttachments[i].srcAlphaBlendFactor =
			pCreateInfo->pColorBlendState->pAttachments[i].srcAlphaBlendFactor;
		colorBlendAttachments[i].dstAlphaBlendFactor =
			pCreateInfo->pColorBlendState->pAttachments[i].dstAlphaBlendFactor;
		colorBlendAttachments[i].alphaBlendOp =
			pCreateInfo->pColorBlendState->pAttachments[i].alphaBlendOp;
		colorBlendAttachments[i].colorWriteMask =
			pCreateInfo->pColorBlendState->pAttachments[i].colorWriteMask;
	}

	VkPipelineColorBlendStateCreateInfo colorBlendState;
	colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendState.pNext = NULL;
	colorBlendState.flags = 0;
	colorBlendState.logicOpEnable = pCreateInfo->pColorBlendState->logicOpEnable;
	colorBlendState.logicOp = pCreateInfo->pColorBlendState->logicOp;
	colorBlendState.attachmentCount = pCreateInfo->pColorBlendState->attachmentCount;
	colorBlendState.pAttachments = colorBlendAttachments;
	colorBlendState.blendConstants[0] = pCreateInfo->pColorBlendState->blendConstants[0];
	colorBlendState.blendConstants[1] = pCreateInfo->pColorBlendState->blendConstants[1];
	colorBlendState.blendConstants[2] = pCreateInfo->pColorBlendState->blendConstants[2];
	colorBlendState.blendConstants[3] = pCreateInfo->pColorBlendState->blendConstants[3];


	VkPipelineDynamicStateCreateInfo dynamicState;
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.pNext = NULL;
	dynamicState.flags = 0;
	dynamicState.dynamicStateCount = pCreateInfo->pDynamicState->dynamicStateCount;
	dynamicState.pDynamicStates = (const VkDynamicState*)pCreateInfo->pDynamicState->pDynamicStates;


	createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	createInfo.pNext = NULL;
	createInfo.flags = pCreateInfo->flags;
	createInfo.stageCount = pCreateInfo->stageCount;
	createInfo.pStages = shaderStages;
	createInfo.pVertexInputState = &vertexInputState;
	createInfo.pInputAssemblyState = &inputAssembly;
	createInfo.pViewportState = &viewportState;
	createInfo.pRasterizationState = &rasterizationState;
	createInfo.pMultisampleState = &multisampleState;
	createInfo.pColorBlendState = &colorBlendState;
	createInfo.pDynamicState = &dynamicState;
	createInfo.layout = (VkPipelineLayout)pCreateInfo->layout;
	createInfo.renderPass = (VkRenderPass)pCreateInfo->renderPass;
	createInfo.subpass = pCreateInfo->subpass;
	createInfo.basePipelineHandle = (VkPipeline)pCreateInfo->basePipelineHandle;
	createInfo.basePipelineIndex = pCreateInfo->basePipelineIndex;

	RrError error = vkCreateGraphicsPipelines((VkDevice)device, (VkPipelineCache)pipelineCache, 1,
		&createInfo, NULL, (VkPipeline*)pPipeline);

	free(bindingDescriptions);
	free(attributeDescriptions);
	free(shaderStages);
	free(colorBlendAttachments);
	return error;
}
