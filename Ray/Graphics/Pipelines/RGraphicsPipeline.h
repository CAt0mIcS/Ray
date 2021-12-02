#pragma once

#include "RPipeline.h"

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
			VkPipelineCache pipelineCache = nullptr,
			VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT,
			VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL, float lineWidth = 1.0f,
			bool depthTestEnabled = true,
			std::optional<std::vector<VkVertexInputBindingDescription>> bindingDescriptions =
				std::nullopt,
			std::optional<std::vector<VkVertexInputAttributeDescription>> attributeDescriptions =
				std::nullopt);
		~GraphicsPipeline();

		Pipeline::BindPoint GetBindPoint() const override;

		VkDescriptorPool GetDescriptorPool() const override { return m_DescriptorPool; }
		VkDescriptorSetLayout GetDescriptorSetLayout(uint32_t set) const override;

		static std::string GetUID(const RenderPass& renderPass, Ref<Shader> shader,
			VkPipelineCache pipelineCache = nullptr,
			VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT,
			VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL, float lineWidth = 1.0f,
			bool depthTestEnabled = true,
			std::optional<std::vector<VkVertexInputBindingDescription>> bindingDescriptions =
				std::nullopt,
			std::optional<std::vector<VkVertexInputAttributeDescription>> attributeDescriptions =
				std::nullopt);

	private:
		void CreateDescriptorSetLayouts();
		void CreateDescriptorPool();
		void CreatePipelineLayout();
		void CreatePipeline(const RenderPass& renderPass, VkPipelineCache pipelineCache,
			VkCullModeFlags cullMode, VkPrimitiveTopology topology, VkPolygonMode polygonMode,
			float lineWidth, bool depthTestEnabled,
			std::optional<std::vector<VkVertexInputBindingDescription>> bindingDescriptions,
			std::optional<std::vector<VkVertexInputAttributeDescription>> attributeDescriptions);

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
			Builder& SetVertexInputBindingDescriptions(
				std::vector<VkVertexInputBindingDescription> descs);
			Builder& SetVertexInputAttributeDescriptions(
				std::vector<VkVertexInputAttributeDescription> descs);

			/**
			 * Builds a new pipeline withouth checking the codex
			 */
			Ref<GraphicsPipeline> Build() const;

			/**
			 * Builds a new pipeline withouth checking the codex
			 */
			Scope<GraphicsPipeline> BuildScoped() const;

			/**
			 * Tries to find a already created pipeline in the codex. If it's not found, it'll be
			 * created.
			 */
			Ref<GraphicsPipeline> Acquire() const;

		private:
			const RenderPass* m_RenderPass;
			Ref<Shader> m_Shader{};
			VkPipelineCache m_PipelineCache;
			VkCullModeFlags m_CullMode;
			VkPrimitiveTopology m_Topology;
			VkPolygonMode m_PolygonMode;
			float m_LineWidth;
			bool m_DepthTestEnabled;
			std::optional<std::vector<VkVertexInputBindingDescription>> m_BindingDescriptions;
			std::optional<std::vector<VkVertexInputAttributeDescription>> m_AttributeDescriptions;
		};
	};
}  // namespace At0::Ray
