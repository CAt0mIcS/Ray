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
		GraphicsPipeline(
			const RenderPass& renderPass, const std::vector<std::string_view>& shaders);
		~GraphicsPipeline();

		Pipeline::BindPoint GetBindPoint() const override;
		const VertexLayout& GetVertexLayout() const { return m_Shader.GetVertexLayout(); }

		VkDescriptorPool GetDescriptorPool() const override { return m_DescriptorPool; }
		VkDescriptorSetLayout GetDescriptorSetLayout(uint32_t set) const override;

		static std::string GetUID(
			const RenderPass& renderPass, const std::vector<std::string_view>& shaders);

	private:
		void CreateShaderProgram(const std::vector<std::string_view>& shaders);
		void CreateDescriptorSetLayouts();
		void CreateDescriptorPool();
		void CreatePipelineLayout();
		void CreatePipeline(const RenderPass& renderPass);

	private:
		std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;

		VkDescriptorPool m_DescriptorPool;
		std::vector<std::pair<uint32_t, VkDescriptorSetLayout>> m_DescriptorSetLayouts;
	};
}  // namespace At0::Ray
