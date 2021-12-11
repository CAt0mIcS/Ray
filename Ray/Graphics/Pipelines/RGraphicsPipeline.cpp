#include "Rpch.h"
#include "RGraphicsPipeline.h"

#include "Shader/RShader.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/RenderPass/RRenderPass.h"
#include "Graphics/RCodex.h"


namespace At0::Ray
{
	GraphicsPipeline::GraphicsPipeline(const RenderPass& renderPass, Ref<Shader> shader,
		VkPipelineCache pipelineCache, VkCullModeFlags cullMode, VkPrimitiveTopology topology,
		VkPolygonMode polygonMode, float lineWidth, bool depthTestEnabled,
		std::optional<std::vector<VkVertexInputBindingDescription>> bindingDescriptions,
		std::optional<std::vector<VkVertexInputAttributeDescription>> attributeDescriptions)
		: Pipeline(std::move(shader))
	{
		CreateDescriptorSetLayouts();
		CreateDescriptorPool();
		CreatePipelineLayout();
		CreatePipeline(renderPass, pipelineCache, cullMode, topology, polygonMode, lineWidth,
			depthTestEnabled, std::move(bindingDescriptions), std::move(attributeDescriptions));
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
		VkPipelineCache pipelineCache, VkCullModeFlags cullMode, VkPrimitiveTopology topology,
		VkPolygonMode polygonMode, float lineWidth, bool depthTestEnabled,
		std::optional<std::vector<VkVertexInputBindingDescription>> bindingDescriptions,
		std::optional<std::vector<VkVertexInputAttributeDescription>> attributeDescriptions)
	{
		std::ostringstream oss;
		oss << "GraphicsPipeline#"
			<< "#" << (uint32_t)cullMode << "#" << (uint32_t)topology << "#"
			<< (uint32_t)polygonMode << "#" << lineWidth << "#" << depthTestEnabled << "#";
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
		VkPipelineCache pipelineCache, VkCullModeFlags cullMode, VkPrimitiveTopology topology,
		VkPolygonMode polygonMode, float lineWidth, bool depthTestEnabled,
		std::optional<std::vector<VkVertexInputBindingDescription>> bindingDescriptions,
		std::optional<std::vector<VkVertexInputAttributeDescription>> attributeDescriptions)
	{
		std::vector<VkVertexInputBindingDescription> bindingDescs{};
		std::vector<VkVertexInputAttributeDescription> attribDescs{};

		if (bindingDescriptions)
			bindingDescs = *bindingDescriptions;
		else
			bindingDescs = m_Shader->GetVertexInputBindings();
		if (attributeDescriptions)
			attribDescs = *attributeDescriptions;
		else
			attribDescs = m_Shader->GetVertexInputAttributes();

		VkPipelineVertexInputStateCreateInfo vertexInput{};
		vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInput.vertexBindingDescriptionCount = (uint32_t)bindingDescs.size();
		vertexInput.pVertexBindingDescriptions = bindingDescs.data();
		vertexInput.vertexAttributeDescriptionCount = (uint32_t)attribDescs.size();
		vertexInput.pVertexAttributeDescriptions = attribDescs.data();

		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
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
		// Input Assembler
		VkPipelineInputAssemblyStateCreateInfo inputAssembler{};
		inputAssembler.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembler.topology = topology;
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
		rasterizer.polygonMode = polygonMode;
		rasterizer.lineWidth = lineWidth;
		rasterizer.cullMode = cullMode;
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
		// Depth stencil
		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = depthTestEnabled;
		depthStencil.depthWriteEnable = depthTestEnabled;
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.minDepthBounds = 0.0f;	 // Optional
		depthStencil.maxDepthBounds = 1.0f;	 // Optional
		depthStencil.stencilTestEnable = VK_FALSE;
		depthStencil.front = {};  // Optional
		depthStencil.back = {};	  // Optional


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
		createInfo.stageCount = (uint32_t)shaderStages.size();
		createInfo.pStages = shaderStages.data();
		createInfo.pVertexInputState = &vertexInput;
		createInfo.pInputAssemblyState = &inputAssembler;
		createInfo.pTessellationState = nullptr;
		createInfo.pViewportState = &viewportState;
		createInfo.pRasterizationState = &rasterizer;
		createInfo.pMultisampleState = &multisampling;
		createInfo.pDepthStencilState = &depthStencil;
		createInfo.pColorBlendState = &colorBlending;
		createInfo.pDynamicState = &dynamicStateInfo;
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
		  m_PipelineCache(Graphics::Get().GetPipelineCache()), m_CullMode(VK_CULL_MODE_BACK_BIT),
		  m_Topology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST), m_PolygonMode(VK_POLYGON_MODE_FILL),
		  m_LineWidth(1.0f), m_DepthTestEnabled(true)
	{
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
		m_CullMode = cullMode;
		return *this;
	}

	GraphicsPipeline::Builder& GraphicsPipeline::Builder::SetPrimitiveTopology(
		VkPrimitiveTopology topology)
	{
		m_Topology = topology;
		return *this;
	}

	GraphicsPipeline::Builder& GraphicsPipeline::Builder::SetPolygonMode(VkPolygonMode polygonMode)
	{
		m_PolygonMode = polygonMode;
		return *this;
	}

	GraphicsPipeline::Builder& GraphicsPipeline::Builder::SetLineWidth(float lineWidth)
	{
		m_LineWidth = lineWidth;
		return *this;
	}

	GraphicsPipeline::Builder& GraphicsPipeline::Builder::SetDepthTestEnabled(bool enabled)
	{
		m_DepthTestEnabled = enabled;
		return *this;
	}

	GraphicsPipeline::Builder& GraphicsPipeline::Builder::SetVertexInputBindingDescriptions(
		std::vector<VkVertexInputBindingDescription> descs)
	{
		m_BindingDescriptions = std::move(descs);
		return *this;
	}

	GraphicsPipeline::Builder& GraphicsPipeline::Builder::SetVertexInputAttributeDescriptions(
		std::vector<VkVertexInputAttributeDescription> descs)
	{
		m_AttributeDescriptions = std::move(descs);
		return *this;
	}

	Ref<GraphicsPipeline> GraphicsPipeline::Builder::Build() const
	{
		ThrowIfInvalidArguments();
		return MakeRef<GraphicsPipeline>(*m_RenderPass, m_Shader, m_PipelineCache, m_CullMode,
			m_Topology, m_PolygonMode, m_LineWidth, m_DepthTestEnabled, m_BindingDescriptions,
			m_AttributeDescriptions);
	}

	Scope<GraphicsPipeline> GraphicsPipeline::Builder::BuildScoped() const
	{
		ThrowIfInvalidArguments();
		return MakeScope<GraphicsPipeline>(*m_RenderPass, m_Shader, m_PipelineCache, m_CullMode,
			m_Topology, m_PolygonMode, m_LineWidth, m_DepthTestEnabled, m_BindingDescriptions,
			m_AttributeDescriptions);
	}

	Ref<GraphicsPipeline> GraphicsPipeline::Builder::Acquire() const
	{
		ThrowIfInvalidArguments();
		return Codex::Resolve<GraphicsPipeline>(*m_RenderPass, m_Shader, m_PipelineCache,
			m_CullMode, m_Topology, m_PolygonMode, m_LineWidth, m_DepthTestEnabled,
			m_BindingDescriptions, m_AttributeDescriptions);
	}

	bool GraphicsPipeline::Builder::ArgumentsValid() const { return m_Shader != nullptr; }

	void GraphicsPipeline::Builder::ThrowIfInvalidArguments() const
	{
		RAY_MEXPECTS(m_Shader != nullptr, "[GraphicsPipeline::Builder] Shader not set");
	}
}  // namespace At0::Ray
