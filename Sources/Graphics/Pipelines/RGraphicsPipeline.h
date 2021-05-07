#pragma once

#include "RPipeline.h"
#include "RShader.h"

#include <vector>
#include <string>


namespace At0::Ray
{
	class RenderPass;
	class VertexLayout;

	class RAY_EXPORT GraphicsPipeline : public Pipeline
	{
	public:
		GraphicsPipeline(const RenderPass& renderPass, const std::vector<std::string>& shaders,
			const VertexLayout* pLayout = nullptr, VkPipelineCache pipelineCache = VK_NULL_HANDLE,
			VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT,
			VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL, float lineWidth = 1.0f,
			bool enableDepthTest = true,
			std::vector<VkVertexInputBindingDescription> bindingDescs = {},
			std::vector<VkVertexInputAttributeDescription> attribDescs = {});
		~GraphicsPipeline();

		Pipeline::BindPoint GetBindPoint() const override;
		const VertexLayout& GetVertexLayout() const { return m_Shader.GetVertexLayout(); }

		VkDescriptorPool GetDescriptorPool() const override { return m_DescriptorPool; }
		VkDescriptorSetLayout GetDescriptorSetLayout(uint32_t set) const override;

		static std::string GetUID(const RenderPass& renderPass,
			const std::vector<std::string>& shaders, const VertexLayout* pLayout = nullptr,
			VkPipelineCache pipelineCache = VK_NULL_HANDLE,
			VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT,
			VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL, float lineWidth = 1.0f,
			bool enableDepthTest = true,
			std::vector<VkVertexInputBindingDescription> bindingDescs = {},
			std::vector<VkVertexInputAttributeDescription> attribDescs = {});

	private:
		void CreateShaderProgram(const std::vector<std::string>& shaders);
		void CreateDescriptorSetLayouts();
		void CreateDescriptorPool();
		void CreatePipelineLayout();
		void CreatePipeline(const RenderPass& renderPass, const VertexLayout* pLayout,
			VkPipelineCache pipelineCache, VkCullModeFlags cullMode, VkPrimitiveTopology topology,
			VkPolygonMode polygonMode, float lineWidth, bool enableDepthTest,
			std::vector<VkVertexInputBindingDescription> bindingDescs,
			std::vector<VkVertexInputAttributeDescription> attribDescs);

	private:
		std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;

		VkDescriptorPool m_DescriptorPool;
		std::vector<std::pair<uint32_t, VkDescriptorSetLayout>> m_DescriptorSetLayouts;
	};
}  // namespace At0::Ray
