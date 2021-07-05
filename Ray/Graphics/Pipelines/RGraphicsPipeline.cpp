#include "Rpch.h"
#include "RGraphicsPipeline.h"

#include "Shader/RShader.h"
#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/RenderPass/RRenderPass.h"

#include "Ray/Utils/RAssert.h"
#include "Ray/Utils/RException.h"
#include "Ray/Utils/RLogger.h"

#include <RayRenderer/Pipeline/RPipeline.h>
#include <RayRenderer/Pipeline/RDescriptor.h>
#include "Core/RRendererLoader.h"

#include <imgui/imgui.h>


namespace At0::Ray
{
	GraphicsPipeline::GraphicsPipeline(Layout layout) : Pipeline(layout.shader)
	{
		if (!layout.renderPass)
			layout.renderPass = &Graphics::Get().GetRenderPass();
		if (!layout.pipelineCache)
			layout.pipelineCache = Graphics::Get().GetPipelineCache();

		CreateDescriptorSetLayouts();
		CreateDescriptorPool();
		CreatePipelineLayout();
		CreatePipeline(layout);
	}

	GraphicsPipeline::~GraphicsPipeline()
	{
		RendererAPI::DestroyDescriptorPool(Graphics::Get().GetDevice(), m_DescriptorPool);
		for (auto [set, descSetLayout] : m_DescriptorSetLayouts)
			RendererAPI::DestroyDescriptorSetLayout(Graphics::Get().GetDevice(), descSetLayout);
	}

	Pipeline::BindPoint GraphicsPipeline::GetBindPoint() const
	{
		return Pipeline::BindPoint::Graphics;
	}

	RrDescriptorSetLayout GraphicsPipeline::GetDescriptorSetLayout(uint32_t set) const
	{
		for (auto [vecSet, descSetLayout] : m_DescriptorSetLayouts)
			if (vecSet == set)
				return descSetLayout;

		RAY_ASSERT(
			false, "[GraphicsPipeline] Descriptor set layout at set {0} does not exist", set);
		return nullptr;
	}

	std::string GraphicsPipeline::GetUID(const Layout& layout)
	{
		std::ostringstream oss;
		oss << "GraphicsPipeline#"
			<< "#" << (uint32_t)layout.cullMode << "#" << (uint32_t)layout.topology << "#"
			<< (uint32_t)layout.polygonMode << "#" << layout.lineWidth << "#"
			<< layout.depthTestEnabled << "#";
		for (std::string_view shader : layout.shader->GetFilepaths())
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
			RrDescriptorSetLayoutCreateInfo createInfo{};
			createInfo.flags =
				/*m_PushDescriptors ? RrDescriptorSetLayoutCreatePushDescriptorKHR : */
				0;
			createInfo.bindingCount = layoutBindings.size();
			createInfo.pBindings = layoutBindings.data();

			ThrowRenderError(RendererAPI::CreateDescriptorSetLayout(Graphics::Get().GetDevice(),
								 &createInfo, &m_DescriptorSetLayouts[i].second),
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
		const std::vector<RrDescriptorPoolSize>& descriptorPoolSizes =
			m_Shader->GetDescriptorPoolSizes();

		RrDescriptorPoolCreateInfo createInfo{};
		createInfo.flags = 0;
		// createInfo.maxSets = 8192;
		createInfo.maxSets = 500000;
		createInfo.poolSizeCount = (uint32_t)descriptorPoolSizes.size();
		createInfo.pPoolSizes = descriptorPoolSizes.data();

		ThrowRenderError(RendererAPI::CreateDescriptorPool(
							 Graphics::Get().GetDevice(), &createInfo, &m_DescriptorPool),
			"[GraphicsPipeline] Failed to create descriptor pool");
	}

	void GraphicsPipeline::CreatePipelineLayout()
	{
		const std::vector<RrPushConstantRange>& pushConstantRanges =
			m_Shader->GetPushConstantRanges();

		// Move descriptor set layouts from std::vector<std::pair<>> to std::vector<>
		std::vector<RrDescriptorSetLayout> descSetLayouts;
		descSetLayouts.reserve(m_DescriptorSetLayouts.size());
		for (const auto& [set, descSetLayout] : m_DescriptorSetLayouts)
			descSetLayouts.emplace_back(descSetLayout);

		RrPipelineLayoutCreateInfo createInfo{};
		createInfo.setLayoutCount = (uint32_t)m_DescriptorSetLayouts.size();
		createInfo.pSetLayouts = descSetLayouts.data();
		createInfo.pushConstantRangeCount = (uint32_t)pushConstantRanges.size();
		createInfo.pPushConstantRanges = pushConstantRanges.data();

		ThrowRenderError(
			RendererAPI::CreatePipelineLayout(Graphics::Get().GetDevice(), &createInfo, &m_Layout),
			"[GraphicsPipeline] Failed to create layout");

		Log::Info("[GraphicsPipeline] Created pipeline layout with {0} descriptor set layout(s) "
				  "and {1} push constant range(s)",
			createInfo.setLayoutCount, createInfo.pushConstantRangeCount);
	}

	void GraphicsPipeline::CreatePipeline(const Layout& layout)
	{
		std::vector<RrVertexInputBindingDescription> bindingDescs{};
		std::vector<RrVertexInputAttributeDescription> attribDescs{};

		if (layout.bindingDescriptions)
			bindingDescs = *layout.bindingDescriptions;
		else
			bindingDescs = m_Shader->GetVertexInputBindings();
		if (layout.attributeDescriptions)
			attribDescs = *layout.attributeDescriptions;
		else
			attribDescs = m_Shader->GetVertexInputAttributes();

		RrPipelineVertexInputStateCreateInfo vertexInput{};
		vertexInput.vertexBindingDescriptionCount = (uint32_t)bindingDescs.size();
		vertexInput.pVertexBindingDescriptions = bindingDescs.data();
		vertexInput.vertexAttributeDescriptionCount = (uint32_t)attribDescs.size();
		vertexInput.pVertexAttributeDescriptions = attribDescs.data();

		std::vector<RrPipelineShaderStageCreateInfo> shaderStages;
		// Create shader module structs
		for (const auto& [stage, shaderModule] : m_Shader->GetShaderModules())
		{
			// std::ostringstream defineBlock;
			// for (const auto& [defineName, defineValue] : m_Defines)
			//	defineBlock << "#define " << defineName << " " << defineValue << '\n';

			RrPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo{};
			pipelineShaderStageCreateInfo.stage = (RrShaderStageFlagBits)stage;
			pipelineShaderStageCreateInfo.module = shaderModule;
			pipelineShaderStageCreateInfo.pName = "main";
			shaderStages.emplace_back(pipelineShaderStageCreateInfo);
		}


		// ---------------------------------------------------------------------------------------
		// Input Assembler
		RrPipelineInputAssemblyStateCreateInfo inputAssembler{};
		inputAssembler.topology = layout.topology;
		inputAssembler.primitiveRestartEnable = false;


		// ---------------------------------------------------------------------------------------
		// Viewport State
		RrPipelineViewportStateCreateInfo viewportState{};
		viewportState.viewportCount = 1;
		viewportState.pViewports = nullptr;
		viewportState.scissorCount = 1;
		viewportState.pScissors = nullptr;


		// ---------------------------------------------------------------------------------------
		// Rasterizer
		RrPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.depthClampEnable = false;
		rasterizer.rasterizerDiscardEnable = false;
		rasterizer.polygonMode = layout.polygonMode;
		rasterizer.lineWidth = layout.lineWidth;
		rasterizer.cullMode = layout.cullMode;
		rasterizer.frontFace = RrFrontFaceCounterClockwise;
		rasterizer.depthBiasEnable = false;
		rasterizer.depthBiasConstantFactor = 0.0f;
		rasterizer.depthBiasClamp = 0.0f;
		rasterizer.depthBiasSlopeFactor = 0.0f;


		// ---------------------------------------------------------------------------------------
		// Multisampling
		RrPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sampleShadingEnable = false;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f;
		multisampling.pSampleMask = nullptr;
		multisampling.alphaToCoverageEnable = false;
		multisampling.alphaToOneEnable = false;


		// ---------------------------------------------------------------------------------------
		// Depth stencil
		RrPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.depthTestEnable = layout.depthTestEnabled;
		depthStencil.depthWriteEnable = layout.depthTestEnabled;
		depthStencil.depthCompareOp = RrCompareOpLess;
		depthStencil.depthBoundsTestEnable = false;
		depthStencil.minDepthBounds = 0.0f;	 // Optional
		depthStencil.maxDepthBounds = 1.0f;	 // Optional
		depthStencil.stencilTestEnable = false;
		depthStencil.front = {};  // Optional
		depthStencil.back = {};	  // Optional


		// ---------------------------------------------------------------------------------------
		// Color Blending
		RrPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask =
			RrColorComponentR | RrColorComponentG | RrColorComponentB | RrColorComponentA;
		colorBlendAttachment.blendEnable = true;
		colorBlendAttachment.srcColorBlendFactor = RrBlendFactorSrcAlpha;
		colorBlendAttachment.dstColorBlendFactor = RrBlendFactorOneMinusSrcAlpha;
		colorBlendAttachment.colorBlendOp = RrBlendOpAdd;
		colorBlendAttachment.srcAlphaBlendFactor = RrBlendFactorOne;
		colorBlendAttachment.dstAlphaBlendFactor = RrBlendFactorZero;
		colorBlendAttachment.alphaBlendOp = RrBlendOpAdd;

		RrPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.logicOpEnable = false;
		colorBlending.logicOp = RrLogicOpCopy;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;


		// ---------------------------------------------------------------------------------------
		// Dynamic state
		RrDynamicState dynamicStates[] = { RrDynamicStateViewport, RrDynamicStateScissor };
		RrPipelineDynamicStateCreateInfo dynamicStateInfo{};
		dynamicStateInfo.dynamicStateCount = std::size(dynamicStates);
		dynamicStateInfo.pDynamicStates = dynamicStates;


		// ---------------------------------------------------------------------------------------
		// Pipeline creation
		RrGraphicsPipelineCreateInfo createInfo{};
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
		createInfo.renderPass = *layout.renderPass;
		createInfo.subpass = 0;	 // Index of subpass used with pipeline;
		createInfo.basePipelineHandle = nullptr;
		createInfo.basePipelineIndex = -1;

		ThrowRenderError(RendererAPI::CreateGraphicsPipeline(Graphics::Get().GetDevice(),
							 *layout.pipelineCache, &createInfo, &m_Pipeline),
			"[GraphicsPipeline] Failed to create");
	}
}  // namespace At0::Ray
