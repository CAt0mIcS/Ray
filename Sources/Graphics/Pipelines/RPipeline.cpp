#include "Rpch.h"
#include "RPipeline.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Commands/RCommandBuffer.h"


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
}  // namespace At0::Ray
