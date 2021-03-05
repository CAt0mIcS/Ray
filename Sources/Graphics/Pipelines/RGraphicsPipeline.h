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

		const VkDescriptorPool& GetDescriptorPool() const { return m_DescriptorPool; }
		const auto& GetDescriptorSetLayouts() const { return m_DescriptorSetLayouts; }

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
		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts;
	};
}  // namespace At0::Ray
