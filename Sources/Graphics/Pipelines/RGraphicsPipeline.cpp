#include "Rpch.h"
#include "RGraphicsPipeline.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/RenderPass/RRenderPass.h"

#include "Utils/RAssert.h"
#include "Utils/RException.h"
#include "Utils/RLogger.h"


namespace At0::Ray
{
	GraphicsPipeline::GraphicsPipeline(
		const RenderPass& renderPass, const std::vector<std::string>& shaders)
	{
		CreateShaderProgram(shaders);
		CreatePipelineLayout();
		CreatePipeline(renderPass);
	}

	VkPipelineBindPoint GraphicsPipeline::GetBindPoint() const
	{
		return VK_PIPELINE_BIND_POINT_GRAPHICS;
	}

	void GraphicsPipeline::CreateShaderProgram(const std::vector<std::string>& shaders)
	{
		// std::ostringstream defineBlock;
		// for (const auto& [defineName, defineValue] : m_Defines)
		//	defineBlock << "#define " << defineName << " " << defineValue << '\n';

		for (const std::string& shader : shaders)
		{
			std::optional<std::string> shaderCode = ReadFile(shader);

			VkShaderStageFlagBits stageFlag = Shader::GetShaderStage(shader);

			VkShaderModule shaderModule = m_Shader.CreateShaderModule(
				shader, *shaderCode, /*defineBlock.str()*/ "", stageFlag);

			VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo{};
			pipelineShaderStageCreateInfo.sType =
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			pipelineShaderStageCreateInfo.stage = stageFlag;
			pipelineShaderStageCreateInfo.module = shaderModule;
			pipelineShaderStageCreateInfo.pName = "main";
			m_ShaderStages.emplace_back(pipelineShaderStageCreateInfo);
		}

		m_Shader.CreateReflection();
	}

	void GraphicsPipeline::CreatePipelineLayout()
	{
		VkPipelineLayoutCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		createInfo.flags = 0;
		createInfo.setLayoutCount = 0;
		createInfo.pSetLayouts = nullptr;
		createInfo.pushConstantRangeCount = 0;
		createInfo.pPushConstantRanges = nullptr;

		RAY_VK_THROW_FAILED(
			vkCreatePipelineLayout(Graphics::Get().GetDevice(), &createInfo, nullptr, &m_Layout),
			"[GraphicsPipeline] Failed to create layout.");
	}

	void GraphicsPipeline::CreatePipeline(const RenderPass& renderPass)
	{
		VkVertexInputBindingDescription bindingDesc{};
		bindingDesc.binding = 0;
		bindingDesc.stride = sizeof(Float3);
		bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		VkVertexInputAttributeDescription attribDesc{};
		attribDesc.location = 0;
		attribDesc.binding = 0;
		attribDesc.format = VK_FORMAT_R32G32B32_SFLOAT;
		attribDesc.offset = 0;

		VkPipelineVertexInputStateCreateInfo vertexInput{};
		vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInput.vertexBindingDescriptionCount = 1;
		vertexInput.pVertexBindingDescriptions = &bindingDesc;
		vertexInput.vertexAttributeDescriptionCount = 1;
		vertexInput.pVertexAttributeDescriptions = &attribDesc;


		// ---------------------------------------------------------------------------------------
		// Input Assembler
		VkPipelineInputAssemblyStateCreateInfo inputAssembler{};
		inputAssembler.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembler.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembler.primitiveRestartEnable = VK_FALSE;

		// ---------------------------------------------------------------------------------------
		// Viewport State
		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = nullptr;
		viewportState.scissorCount = 1;
		viewportState.pScissors = nullptr;


		// ---------------------------------------------------------------------------------------
		// Rasterizer
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = /*VK_CULL_MODE_BACK_BIT*/ VK_CULL_MODE_NONE;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f;
		rasterizer.depthBiasClamp = 0.0f;
		rasterizer.depthBiasSlopeFactor = 0.0f;


		// ---------------------------------------------------------------------------------------
		// Multisampling
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f;
		multisampling.pSampleMask = nullptr;
		multisampling.alphaToCoverageEnable = VK_FALSE;
		multisampling.alphaToOneEnable = VK_FALSE;


		// ---------------------------------------------------------------------------------------
		// Color Blending
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
											  VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;


		// ---------------------------------------------------------------------------------------
		// Dynamic state
		VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
		dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateInfo.dynamicStateCount = std::size(dynamicStates);
		dynamicStateInfo.pDynamicStates = dynamicStates;


		// ---------------------------------------------------------------------------------------
		// Pipeline creation
		VkGraphicsPipelineCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfo.flags = 0;
		createInfo.stageCount = (uint32_t)m_ShaderStages.size();
		createInfo.pStages = m_ShaderStages.data();
		createInfo.pVertexInputState = &vertexInput;
		createInfo.pInputAssemblyState = &inputAssembler;
		createInfo.pTessellationState = nullptr;
		createInfo.pViewportState = &viewportState;
		createInfo.pRasterizationState = &rasterizer;
		createInfo.pMultisampleState = &multisampling;
		createInfo.pDepthStencilState = nullptr;
		createInfo.pColorBlendState = &colorBlending;
		createInfo.pDynamicState = &dynamicStateInfo;
		createInfo.layout = m_Layout;
		createInfo.renderPass = renderPass;
		createInfo.subpass = 0;	 // Index of subpass used with pipeline;
		createInfo.basePipelineHandle = VK_NULL_HANDLE;
		createInfo.basePipelineIndex = -1;

		RAY_VK_THROW_FAILED(vkCreateGraphicsPipelines(Graphics::Get().GetDevice(), nullptr, 1,
								&createInfo, nullptr, &m_Pipeline),
			"[GraphicsPipeline] Failed to create.");

		// Destroy shader modules as they aren't needed anymore
		for (const VkPipelineShaderStageCreateInfo& shaderStage : m_ShaderStages)
		{
			vkDestroyShaderModule(Graphics::Get().GetDevice(), shaderStage.module, nullptr);
		}

		// These vectors aren't needed anymore and can be freed
		m_ShaderStages.resize(0);
	}
}  // namespace At0::Ray
