#pragma once

#include "RPipeline.h"
#include "Shader/RShaderTypes.h"

#include <vector>
#include <string>
#include <optional>


namespace At0::Ray
{
	class RenderPass;

	class RAY_EXPORT GraphicsPipeline : public Pipeline
	{
	public:
		GraphicsPipeline(const RenderPass& renderPass, Ref<Shader> shader,
			VkPipelineCache pipelineCache, VkPipelineVertexInputStateCreateInfo* vertexInput,
			VkPipelineInputAssemblyStateCreateInfo* inputAssembler,
			VkPipelineShaderStageCreateInfo* shaderStages,
			VkPipelineViewportStateCreateInfo* viewportState,
			VkPipelineRasterizationStateCreateInfo* rasterizer,
			VkPipelineMultisampleStateCreateInfo* multisampling,
			VkPipelineDepthStencilStateCreateInfo* depthStencil,
			VkPipelineColorBlendAttachmentState* colorBlendAttachment,
			VkPipelineColorBlendStateCreateInfo* colorBlending,
			VkPipelineDynamicStateCreateInfo* dynamicStateInfo);
		~GraphicsPipeline();

		Pipeline::BindPoint GetBindPoint() const override;

		VkDescriptorPool GetDescriptorPool() const override { return m_DescriptorPool; }
		VkDescriptorSetLayout GetDescriptorSetLayout(uint32_t set) const override;

		static std::string GetUID(const RenderPass& renderPass, Ref<Shader> shader,
			VkPipelineCache pipelineCache, VkPipelineVertexInputStateCreateInfo* vertexInput,
			VkPipelineInputAssemblyStateCreateInfo* inputAssembler,
			VkPipelineShaderStageCreateInfo* shaderStages,
			VkPipelineViewportStateCreateInfo* viewportState,
			VkPipelineRasterizationStateCreateInfo* rasterizer,
			VkPipelineMultisampleStateCreateInfo* multisampling,
			VkPipelineDepthStencilStateCreateInfo* depthStencil,
			VkPipelineColorBlendAttachmentState* colorBlendAttachment,
			VkPipelineColorBlendStateCreateInfo* colorBlending,
			VkPipelineDynamicStateCreateInfo* dynamicStateInfo);

	private:
		void CreateDescriptorSetLayouts();
		void CreateDescriptorPool();
		void CreatePipelineLayout();
		void CreatePipeline(const RenderPass& renderPass, VkPipelineCache pipelineCache,
			VkPipelineVertexInputStateCreateInfo* vertexInput,
			VkPipelineInputAssemblyStateCreateInfo* inputAssembler,
			VkPipelineShaderStageCreateInfo* shaderStages,
			VkPipelineViewportStateCreateInfo* viewportState,
			VkPipelineRasterizationStateCreateInfo* rasterizer,
			VkPipelineMultisampleStateCreateInfo* multisampling,
			VkPipelineDepthStencilStateCreateInfo* depthStencil,
			VkPipelineColorBlendAttachmentState* colorBlendAttachment,
			VkPipelineColorBlendStateCreateInfo* colorBlending,
			VkPipelineDynamicStateCreateInfo* dynamicStateInfo);

	private:
		VkDescriptorPool m_DescriptorPool;
		std::vector<std::pair<uint32_t, VkDescriptorSetLayout>> m_DescriptorSetLayouts;

	public:
		class RAY_EXPORT Builder
		{
		public:
			/**
			 * Sets some defaults for most arguments
			 * The shader must still be set.
			 */
			Builder();

			Builder& SetRenderPass(const RenderPass& renderPass);
			Builder& SetShader(Ref<Shader> shader);
			Builder& SetPipelineCache(VkPipelineCache pipelineCache);
			Builder& SetCullMode(VkCullModeFlags cullMode);
			Builder& SetPrimitiveTopology(VkPrimitiveTopology topology);
			Builder& SetPolygonMode(VkPolygonMode polygonMode);
			Builder& SetLineWidth(float lineWidth);
			Builder& SetDepthTestEnabled(bool enabled);
			Builder& SetBlendingEnabled(bool enabled);

			// Not supported by glslang
			// Builder& SetShaderEntryPoint(ShaderStage stage, const char* pName);

			Builder& SetVertexInputBindingDescriptions(
				std::vector<VkVertexInputBindingDescription> descs);
			Builder& SetVertexInputAttributeDescriptions(
				std::vector<VkVertexInputAttributeDescription> descs);

			/**
			 * Builds a new pipeline withouth checking the codex
			 */
			Ref<GraphicsPipeline> Build();

			/**
			 * Builds a new pipeline withouth checking the codex
			 */
			Scope<GraphicsPipeline> BuildScoped();

			/**
			 * Tries to find a already created pipeline in the codex. If it's not found, it'll be
			 * created.
			 */
			Ref<GraphicsPipeline> Acquire();

			/**
			 * Checks if arguments are valid
			 */
			bool ArgumentsValid() const;

		private:
			void ThrowIfInvalidArguments() const;
			void SetFinalData();

		private:
			const RenderPass* m_RenderPass;
			Ref<Shader> m_Shader{};
			VkPipelineCache m_PipelineCache;

			VkPipelineVertexInputStateCreateInfo m_VertexInput{};
			VkPipelineInputAssemblyStateCreateInfo m_InputAssembler{};
			VkPipelineViewportStateCreateInfo m_ViewportState{};
			VkPipelineRasterizationStateCreateInfo m_Rasterizer{};
			VkPipelineMultisampleStateCreateInfo m_Multisampling{};
			VkPipelineDepthStencilStateCreateInfo m_DepthStencil{};
			VkPipelineColorBlendAttachmentState m_ColorBlendAttachment{};
			VkPipelineColorBlendStateCreateInfo m_ColorBlending{};
			std::vector<VkDynamicState> m_DynamicStates;
			VkPipelineDynamicStateCreateInfo m_DynamicStateInfo{};
			std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;

			bool m_CustomSetBinding = false;
			bool m_CustomSetAttrib = false;
			std::vector<VkVertexInputBindingDescription> m_BindingDescs{};
			std::vector<VkVertexInputAttributeDescription> m_AttribDescs{};
			std::unordered_map<ShaderStage, std::string> m_ShaderEntryPoints;
		};
	};
}  // namespace At0::Ray
