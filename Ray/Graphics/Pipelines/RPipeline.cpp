#include "RPipeline.h"

#include "Graphics/Core/RRenderContext.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Graphics/Commands/RCommandBuffer.h"
#include "Shader/RShader.h"


namespace At0::Ray
{
	Pipeline::~Pipeline()
	{
		auto& device = m_Shader->GetRenderContext().device;
		vkDestroyPipelineLayout(device, m_Layout, nullptr);
		vkDestroyPipeline(device, m_Pipeline, nullptr);
	}

	VkDescriptorSetLayout Pipeline::GetDescriptorSetLayout(uint32_t set) const
	{
		return VK_NULL_HANDLE;
	}

	void Pipeline::CmdBind(const CommandBuffer& cmdBuff) const
	{
		vkCmdBindPipeline(cmdBuff, (VkPipelineBindPoint)GetBindPoint(), m_Pipeline);
	}

	Pipeline::Pipeline(Ref<Shader> shader) : m_Shader(std::move(shader)) {}
}  // namespace At0::Ray
