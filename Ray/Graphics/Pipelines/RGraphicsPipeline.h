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
			std::optional<RrPipelineCache>
				pipelineCache;	// set to Graphics::Get().GetPipelineCache()
			RrCullModeFlags cullMode = RrCullModeBack;
			RrPrimitiveTopology topology = RrPrimitiveTopologyTriangleList;
			RrPolygonMode polygonMode = RrPolygonModeFill;
			float lineWidth = 1.0f;
			bool depthTestEnabled = true;
			std::optional<std::vector<RrVertexInputBindingDescription>> bindingDescriptions;
			std::optional<std::vector<RrVertexInputAttributeDescription>> attributeDescriptions;

			virtual ~Layout() = default;
		};

	public:
		GraphicsPipeline(Layout layout);
		~GraphicsPipeline();

		Pipeline::BindPoint GetBindPoint() const override;

		VkDescriptorPool GetDescriptorPool() const override
		{
			return (VkDescriptorPool)m_DescriptorPool;
		}
		VkDescriptorSetLayout GetDescriptorSetLayout(uint32_t set) const override;

		static std::string GetUID(const Layout& layout);

	private:
		void CreateDescriptorSetLayouts();
		void CreateDescriptorPool();
		void CreatePipelineLayout();
		void CreatePipeline(const Layout& layout);

	private:
		RrDescriptorPool m_DescriptorPool;
		std::vector<std::pair<uint32_t, RrDescriptorSetLayout>> m_DescriptorSetLayouts;
	};
}  // namespace At0::Ray
