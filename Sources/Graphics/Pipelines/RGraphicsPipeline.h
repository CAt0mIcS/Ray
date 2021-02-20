#pragma once

#include "RPipeline.h"
#include "RShader.h"

#include <vector>
#include <string>


namespace At0::Ray
{
	class RenderPass;

	class RAY_EXPORT GraphicsPipeline : public Pipeline
	{
	public:
		GraphicsPipeline(const RenderPass& renderPass, const std::vector<std::string>& shaders);
		~GraphicsPipeline() = default;

		VkPipelineBindPoint GetBindPoint() const override;

	private:
		void CreateShaderProgram(const std::vector<std::string>& shaders);
		void CreatePipelineLayout();
		void CreatePipeline(const RenderPass& renderPass);

	private:
		std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
		Shader m_Shader;
	};
}  // namespace At0::Ray
