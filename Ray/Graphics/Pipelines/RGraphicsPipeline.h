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
		struct Layout
		{
			const RenderPass* renderPass = nullptr;	 // Set to Graphics::Get().GetRenderPass()
			Ref<Shader> shader{};
			std::optional<VkPipelineCache>
				pipelineCache;	// set to Graphics::Get().GetPipelineCache()
			VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT;
			VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
			float lineWidth = 1.0f;
			bool depthTestEnabled = true;
			std::optional<std::vector<VkVertexInputBindingDescription>> bindingDescriptions;
			std::optional<std::vector<VkVertexInputAttributeDescription>> attributeDescriptions;

			virtual ~Layout() = default;
		};

	public:
		GraphicsPipeline(Layout layout);
		~GraphicsPipeline();

		Pipeline::BindPoint GetBindPoint() const override;

		VkDescriptorPool GetDescriptorPool() const override { return m_DescriptorPool; }
		VkDescriptorSetLayout GetDescriptorSetLayout(uint32_t set) const override;

		static std::string GetUID(const Layout& layout);

	private:
		void CreateShaderProgram(std::string_view filepath);
		void CreateDescriptorSetLayouts();
		void CreateDescriptorPool();
		void CreatePipelineLayout();
		void CreatePipeline(const Layout& layout);

	private:
		VkDescriptorPool m_DescriptorPool;
		std::vector<std::pair<uint32_t, VkDescriptorSetLayout>> m_DescriptorSetLayouts;
	};
}  // namespace At0::Ray
