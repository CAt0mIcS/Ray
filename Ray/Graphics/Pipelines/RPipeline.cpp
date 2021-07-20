#include "Rpch.h"
#include "RPipeline.h"

#include "Ray/Core/RRendererLoader.h"
#include "Ray/Graphics/RGraphics.h"
#include "Ray/Graphics/Core/RLogicalDevice.h"
#include "Ray/Graphics/Commands/RCommandBuffer.h"
#include "Ray/Graphics/RCodex.h"
#include "Shader/RShader.h"


namespace At0::Ray
{
	Pipeline::~Pipeline()
	{
		RendererAPI::DestroyPipelineLayout(Graphics::Get().GetDevice(), m_Layout);
		RendererAPI::DestroyPipeline(Graphics::Get().GetDevice(), m_Pipeline);
	}

	RrDescriptorSetLayout Pipeline::GetDescriptorSetLayout(uint32_t set) const { return nullptr; }

	void Pipeline::CmdBind(const CommandBuffer& cmdBuff) const
	{
		RendererAPI::CmdBindPipeline(cmdBuff, (RrPipelineBindPoint)GetBindPoint(), *this);
	}

	Pipeline::Pipeline(Ref<Shader> shader) : m_Shader(std::move(shader)) {}
}  // namespace At0::Ray
