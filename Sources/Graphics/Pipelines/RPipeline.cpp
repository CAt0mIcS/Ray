#include "Rpch.h"
#include "RPipeline.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Graphics/RCodex.h"


namespace At0::Ray
{
	Pipeline::~Pipeline()
	{
		vkDestroyPipelineLayout(Graphics::Get().GetDevice(), m_Layout, nullptr);
		vkDestroyPipeline(Graphics::Get().GetDevice(), m_Pipeline, nullptr);
	}

	void Pipeline::CmdBind(const CommandBuffer& cmdBuff) const
	{
		vkCmdBindPipeline(cmdBuff, (VkPipelineBindPoint)GetBindPoint(), m_Pipeline);
	}

	Pipeline::Pipeline(std::vector<std::string> shaders)
		: m_Shader(Codex::Resolve<Shader>(std::move(shaders)))
	{
	}
}  // namespace At0::Ray
