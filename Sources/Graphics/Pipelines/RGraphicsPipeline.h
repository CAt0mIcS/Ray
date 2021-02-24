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
		GraphicsPipeline(const RenderPass& renderPass, const std::vector<std::string>& shaders);
		~GraphicsPipeline();

		VkPipelineBindPoint GetBindPoint() const override;
		const VertexLayout& GetVertexLayout() const { return m_Shader.GetVertexLayout(); }

		const VkDescriptorPool& GetDescriptorPool() const { return m_DescriptorPool; }
		const VkDescriptorSetLayout& GetDescriptorSetLayout() const
		{
			return m_DescriptorSetLayout;
		}

		static std::string GetUID(
			const RenderPass& renderPass, const std::vector<std::string>& shaders);

	private:
		void CreateShaderProgram(const std::vector<std::string>& shaders);
		void CreateDescriptorSetLayout();
		void CreateDescriptorPool();
		void CreatePipelineLayout();
		void CreatePipeline(const RenderPass& renderPass);

	private:
		std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;

		VkDescriptorPool m_DescriptorPool;
		VkDescriptorSetLayout m_DescriptorSetLayout;
	};
}  // namespace At0::Ray
