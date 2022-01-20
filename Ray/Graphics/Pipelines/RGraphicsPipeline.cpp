#include "RGraphicsPipeline.h"

#include "Shader/RShader.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/RenderPass/RRenderPass.h"
#include "Graphics/RCodex.h"


namespace At0::Ray
{
	GraphicsPipeline::GraphicsPipeline(const RenderPass& renderPass, Ref<Shader> shader,
		VkPipelineCache pipelineCache, VkPipelineVertexInputStateCreateInfo* vertexInput,
		VkPipelineInputAssemblyStateCreateInfo* inputAssembler,
		VkPipelineViewportStateCreateInfo* viewportState,
		VkPipelineRasterizationStateCreateInfo* rasterizer,
		VkPipelineMultisampleStateCreateInfo* multisampling,
		VkPipelineDepthStencilStateCreateInfo* depthStencil,
		VkPipelineColorBlendAttachmentState* colorBlendAttachment,
		VkPipelineColorBlendStateCreateInfo* colorBlending,
		VkPipelineDynamicStateCreateInfo* dynamicStateInfo)
		: Pipeline(std::move(shader))
	{
		CreateDescriptorSetLayouts();
		CreateDescriptorPool();
		CreatePipelineLayout();
		CreatePipeline(renderPass, pipelineCache, vertexInput, inputAssembler, viewportState,
			rasterizer, multisampling, depthStencil, colorBlendAttachment, colorBlending,
			dynamicStateInfo);
	}

	GraphicsPipeline::~GraphicsPipeline()
	{
		vkDestroyDescriptorPool(Graphics::Get().GetDevice(), m_DescriptorPool, nullptr);
		for (auto [set, descSetLayout] : m_DescriptorSetLayouts)
			vkDestroyDescriptorSetLayout(Graphics::Get().GetDevice(), descSetLayout, nullptr);
	}

	Pipeline::BindPoint GraphicsPipeline::GetBindPoint() const
	{
		return Pipeline::BindPoint::Graphics;
	}

	VkDescriptorSetLayout GraphicsPipeline::GetDescriptorSetLayout(uint32_t set) const
	{
		for (auto [vecSet, descSetLayout] : m_DescriptorSetLayouts)
			if (vecSet == set)
				return descSetLayout;

		RAY_ASSERT(
			false, "[GraphicsPipeline] Descriptor set layout at set {0} does not exist", set);
		return VK_NULL_HANDLE;
	}

	std::string GraphicsPipeline::GetUID(const RenderPass& renderPass, Ref<Shader> shader,
		VkPipelineCache pipelineCache, VkPipelineVertexInputStateCreateInfo* vertexInput,
		VkPipelineInputAssemblyStateCreateInfo* inputAssembler,
		VkPipelineViewportStateCreateInfo* viewportState,
		VkPipelineRasterizationStateCreateInfo* rasterizer,
		VkPipelineMultisampleStateCreateInfo* multisampling,
		VkPipelineDepthStencilStateCreateInfo* depthStencil,
		VkPipelineColorBlendAttachmentState* colorBlendAttachment,
		VkPipelineColorBlendStateCreateInfo* colorBlending,
		VkPipelineDynamicStateCreateInfo* dynamicStateInfo)
	{
		std::ostringstream oss;
		oss << "GraphicsPipeline#"
			<< "#" << (uint32_t)rasterizer->cullMode << "#" << (uint32_t)inputAssembler->topology
			<< "#" << (uint32_t)rasterizer->polygonMode << "#" << rasterizer->lineWidth << "#"
			<< depthStencil->depthWriteEnable << "#";
		for (std::string_view shader : shader->GetFilepaths())
			oss << shader << "#";

		return oss.str();
	}

	void GraphicsPipeline::CreateDescriptorSetLayouts()
	{
		const auto& descriptorSetLayoutBindings = m_Shader->GetDescriptorSetLayoutBindings();

		m_DescriptorSetLayouts.resize(descriptorSetLayoutBindings.size());
		uint32_t i = 0;
		for (auto& [set, layoutBindings] : descriptorSetLayoutBindings)
		{
			VkDescriptorSetLayoutCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			createInfo.flags =
				/*m_PushDescriptors ? VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR : */
				0;
			createInfo.bindingCount = layoutBindings.size();
			createInfo.pBindings = layoutBindings.data();

			ThrowVulkanError(vkCreateDescriptorSetLayout(Graphics::Get().GetDevice(), &createInfo,
								 nullptr, &m_DescriptorSetLayouts[i].second),
				"[GraphicsPipeline] Failed to create descriptor set layout");

			m_DescriptorSetLayouts[i].first = set;
			++i;
		}

		// Sort descriptor set layouts by set
		std::sort(m_DescriptorSetLayouts.begin(), m_DescriptorSetLayouts.end(),
			[](const auto& l, const auto& r) { return l.first < r.first; });
	}

	void GraphicsPipeline::CreateDescriptorPool()
	{
		const std::vector<VkDescriptorPoolSize>& descriptorPoolSizes =
			m_Shader->GetDescriptorPoolSizes();

		VkDescriptorPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.flags = 0;
		// createInfo.maxSets = 8192;
		createInfo.maxSets = 500000;
		createInfo.poolSizeCount = (uint32_t)descriptorPoolSizes.size();
		createInfo.pPoolSizes = descriptorPoolSizes.data();

		ThrowVulkanError(vkCreateDescriptorPool(
							 Graphics::Get().GetDevice(), &createInfo, nullptr, &m_DescriptorPool),
			"[GraphicsPipeline] Failed to create descriptor pool");
	}

	void GraphicsPipeline::CreatePipelineLayout()
	{
		const std::vector<VkPushConstantRange>& pushConstantRanges =
			m_Shader->GetPushConstantRanges();

		// Move descriptor set layouts from std::vector<std::pair<>> to std::vector<>
		std::vector<VkDescriptorSetLayout> descSetLayouts;
		descSetLayouts.reserve(m_DescriptorSetLayouts.size());
		for (const auto& [set, descSetLayout] : m_DescriptorSetLayouts)
			descSetLayouts.emplace_back(descSetLayout);

		VkPipelineLayoutCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		createInfo.flags = 0;
		createInfo.setLayoutCount = (uint32_t)m_DescriptorSetLayouts.size();
		createInfo.pSetLayouts = descSetLayouts.data();
		createInfo.pushConstantRangeCount = (uint32_t)pushConstantRanges.size();
		createInfo.pPushConstantRanges = pushConstantRanges.data();

		ThrowVulkanError(
			vkCreatePipelineLayout(Graphics::Get().GetDevice(), &createInfo, nullptr, &m_Layout),
			"[GraphicsPipeline] Failed to create layout");

		Log::Info("[GraphicsPipeline] Created pipeline layout with {0} descriptor set layout(s) "
				  "and {1} push constant range(s)",
			createInfo.setLayoutCount, createInfo.pushConstantRangeCount);
	}

	void GraphicsPipeline::CreatePipeline(const RenderPass& renderPass,
		VkPipelineCache pipelineCache, VkPipelineVertexInputStateCreateInfo* vertexInput,
		VkPipelineInputAssemblyStateCreateInfo* inputAssembler,
		VkPipelineViewportStateCreateInfo* viewportState,
		VkPipelineRasterizationStateCreateInfo* rasterizer,
		VkPipelineMultisampleStateCreateInfo* multisampling,
		VkPipelineDepthStencilStateCreateInfo* depthStencil,
		VkPipelineColorBlendAttachmentState* colorBlendAttachment,
		VkPipelineColorBlendStateCreateInfo* colorBlending,
		VkPipelineDynamicStateCreateInfo* dynamicStateInfo)
	{
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		shaderStages.reserve(m_Shader->GetShaderModules().size());
		// Create shader module structs
		for (const auto& [stage, shaderModule] : m_Shader->GetShaderModules())
		{
			// std::ostringstream defineBlock;
			// for (const auto& [defineName, defineValue] : m_Defines)
			//	defineBlock << "#define " << defineName << " " << defineValue << '\n';

			VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo{};
			pipelineShaderStageCreateInfo.sType =
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			pipelineShaderStageCreateInfo.stage = (VkShaderStageFlagBits)stage;
			pipelineShaderStageCreateInfo.module = shaderModule;
			pipelineShaderStageCreateInfo.pName = "main";
			shaderStages.emplace_back(pipelineShaderStageCreateInfo);
		}


		// ---------------------------------------------------------------------------------------
		// Pipeline creation
		VkGraphicsPipelineCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfo.flags = 0;
		createInfo.stageCount = (uint32_t)shaderStages.size();
		createInfo.pStages = shaderStages.data();
		createInfo.pVertexInputState = vertexInput;
		createInfo.pInputAssemblyState = inputAssembler;
		createInfo.pTessellationState = nullptr;
		createInfo.pViewportState = viewportState;
		createInfo.pRasterizationState = rasterizer;
		createInfo.pMultisampleState = multisampling;
		createInfo.pDepthStencilState = depthStencil;
		createInfo.pColorBlendState = colorBlending;
		createInfo.pDynamicState = dynamicStateInfo;
		createInfo.layout = m_Layout;
		createInfo.renderPass = renderPass;
		createInfo.subpass = 0;	 // Index of subpass used with pipeline;
		createInfo.basePipelineHandle = VK_NULL_HANDLE;
		createInfo.basePipelineIndex = -1;

		ThrowVulkanError(vkCreateGraphicsPipelines(Graphics::Get().GetDevice(), pipelineCache, 1,
							 &createInfo, nullptr, &m_Pipeline),
			"[GraphicsPipeline] Failed to create");
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////// BUILDER //////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	GraphicsPipeline::Builder::Builder()
		: m_RenderPass(&Graphics::Get().GetRenderPass()),
		  m_PipelineCache(Graphics::Get().GetPipelineCache())
	{
		m_VertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		// ---------------------------------------------------------------------------------------
		// Input Assembler
		m_InputAssembler.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		m_InputAssembler.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		m_InputAssembler.primitiveRestartEnable = VK_FALSE;


		// ---------------------------------------------------------------------------------------
		// Viewport State
		m_ViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		m_ViewportState.viewportCount = 1;
		m_ViewportState.pViewports = nullptr;
		m_ViewportState.scissorCount = 1;
		m_ViewportState.pScissors = nullptr;


		// ---------------------------------------------------------------------------------------
		// Rasterizer
		m_Rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		m_Rasterizer.depthClampEnable = VK_FALSE;
		m_Rasterizer.rasterizerDiscardEnable = VK_FALSE;
		m_Rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		m_Rasterizer.lineWidth = 1.f;
		m_Rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		m_Rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		m_Rasterizer.depthBiasEnable = VK_FALSE;
		m_Rasterizer.depthBiasConstantFactor = 0.0f;
		m_Rasterizer.depthBiasClamp = 0.0f;
		m_Rasterizer.depthBiasSlopeFactor = 0.0f;


		// ---------------------------------------------------------------------------------------
		// Multisampling
		m_Multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		m_Multisampling.sampleShadingEnable = VK_FALSE;
		m_Multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		m_Multisampling.minSampleShading = 1.0f;
		m_Multisampling.pSampleMask = nullptr;
		m_Multisampling.alphaToCoverageEnable = VK_FALSE;
		m_Multisampling.alphaToOneEnable = VK_FALSE;


		// ---------------------------------------------------------------------------------------
		// Depth stencil
		m_DepthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		m_DepthStencil.depthTestEnable = true;
		m_DepthStencil.depthWriteEnable = true;
		m_DepthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		m_DepthStencil.depthBoundsTestEnable = VK_FALSE;
		m_DepthStencil.minDepthBounds = 0.0f;  // Optional
		m_DepthStencil.maxDepthBounds = 1.0f;  // Optional
		m_DepthStencil.stencilTestEnable = VK_FALSE;
		m_DepthStencil.front = {};	// Optional
		m_DepthStencil.back = {};	// Optional


		// ---------------------------------------------------------------------------------------
		// Color Blending
		m_ColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
												VK_COLOR_COMPONENT_G_BIT |
												VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		m_ColorBlendAttachment.blendEnable = VK_TRUE;
		m_ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		m_ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		m_ColorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		m_ColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		m_ColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		m_ColorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		m_ColorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		m_ColorBlending.logicOpEnable = VK_FALSE;
		m_ColorBlending.logicOp = VK_LOGIC_OP_COPY;
		m_ColorBlending.attachmentCount = 1;
		m_ColorBlending.pAttachments = &m_ColorBlendAttachment;
		m_ColorBlending.blendConstants[0] = 0.0f;
		m_ColorBlending.blendConstants[1] = 0.0f;
		m_ColorBlending.blendConstants[2] = 0.0f;
		m_ColorBlending.blendConstants[3] = 0.0f;


		// ---------------------------------------------------------------------------------------
		// Dynamic state
		m_DynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		m_DynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		m_DynamicStateInfo.dynamicStateCount = m_DynamicStates.size();
		m_DynamicStateInfo.pDynamicStates = m_DynamicStates.data();
	}

	GraphicsPipeline::Builder& GraphicsPipeline::Builder::SetRenderPass(
		const RenderPass& renderPass)
	{
		m_RenderPass = &renderPass;
		return *this;
	}

	GraphicsPipeline::Builder& GraphicsPipeline::Builder::SetShader(Ref<Shader> shader)
	{
		m_Shader = std::move(shader);
		return *this;
	}

	GraphicsPipeline::Builder& GraphicsPipeline::Builder::SetPipelineCache(
		VkPipelineCache pipelineCache)
	{
		m_PipelineCache = pipelineCache;
		return *this;
	}

	GraphicsPipeline::Builder& GraphicsPipeline::Builder::SetCullMode(VkCullModeFlags cullMode)
	{
		m_Rasterizer.cullMode = cullMode;
		return *this;
	}

	GraphicsPipeline::Builder& GraphicsPipeline::Builder::SetPrimitiveTopology(
		VkPrimitiveTopology topology)
	{
		m_InputAssembler.topology = topology;
		return *this;
	}

	GraphicsPipeline::Builder& GraphicsPipeline::Builder::SetPolygonMode(VkPolygonMode polygonMode)
	{
		m_Rasterizer.polygonMode = polygonMode;
		return *this;
	}

	GraphicsPipeline::Builder& GraphicsPipeline::Builder::SetLineWidth(float lineWidth)
	{
		m_Rasterizer.lineWidth = lineWidth;
		return *this;
	}

	GraphicsPipeline::Builder& GraphicsPipeline::Builder::SetDepthTestEnabled(bool enabled)
	{
		m_DepthStencil.depthTestEnable = enabled;
		m_DepthStencil.depthWriteEnable = enabled;
		return *this;
	}

	GraphicsPipeline::Builder& GraphicsPipeline::Builder::SetVertexInputBindingDescriptions(
		std::vector<VkVertexInputBindingDescription> descs)
	{
		m_BindingDescs = std::move(descs);
		m_CustomSetBinding = true;
		return *this;
	}

	GraphicsPipeline::Builder& GraphicsPipeline::Builder::SetVertexInputAttributeDescriptions(
		std::vector<VkVertexInputAttributeDescription> descs)
	{
		m_AttribDescs = std::move(descs);
		m_CustomSetAttrib = true;
		return *this;
	}

	Ref<GraphicsPipeline> GraphicsPipeline::Builder::Build()
	{
		ThrowIfInvalidArguments();
		SetFinalData();
		return MakeRef<GraphicsPipeline>(*m_RenderPass, m_Shader, m_PipelineCache, &m_VertexInput,
			&m_InputAssembler, &m_ViewportState, &m_Rasterizer, &m_Multisampling, &m_DepthStencil,
			&m_ColorBlendAttachment, &m_ColorBlending, &m_DynamicStateInfo);
	}

	Scope<GraphicsPipeline> GraphicsPipeline::Builder::BuildScoped()
	{
		ThrowIfInvalidArguments();
		SetFinalData();
		return MakeScope<GraphicsPipeline>(*m_RenderPass, m_Shader, m_PipelineCache, &m_VertexInput,
			&m_InputAssembler, &m_ViewportState, &m_Rasterizer, &m_Multisampling, &m_DepthStencil,
			&m_ColorBlendAttachment, &m_ColorBlending, &m_DynamicStateInfo);
	}

	Ref<GraphicsPipeline> GraphicsPipeline::Builder::Acquire()
	{
		ThrowIfInvalidArguments();
		SetFinalData();
		return Codex::Resolve<GraphicsPipeline>(*m_RenderPass, m_Shader, m_PipelineCache,
			&m_VertexInput, &m_InputAssembler, &m_ViewportState, &m_Rasterizer, &m_Multisampling,
			&m_DepthStencil, &m_ColorBlendAttachment, &m_ColorBlending, &m_DynamicStateInfo);
	}

	bool GraphicsPipeline::Builder::ArgumentsValid() const { return m_Shader != nullptr; }

	void GraphicsPipeline::Builder::ThrowIfInvalidArguments() const
	{
		RAY_MEXPECTS(m_Shader != nullptr, "[GraphicsPipeline::Builder] Shader not set");
	}

	void GraphicsPipeline::Builder::SetFinalData()
	{
		// ---------------------------------------------------------------------------------------
		// Vertex Input
		if (!m_CustomSetBinding)
			m_BindingDescs = m_Shader->GetVertexInputBindings();
		if (!m_CustomSetAttrib)
			m_AttribDescs = m_Shader->GetVertexInputAttributes();
		m_VertexInput.vertexBindingDescriptionCount = (uint32_t)m_BindingDescs.size();
		m_VertexInput.pVertexBindingDescriptions = m_BindingDescs.data();
		m_VertexInput.vertexAttributeDescriptionCount = (uint32_t)m_AttribDescs.size();
		m_VertexInput.pVertexAttributeDescriptions = m_AttribDescs.data();
	}

}  // namespace At0::Ray
