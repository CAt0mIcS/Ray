#include "Rpch.h"
#include "RPipeline.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"


namespace At0::Ray
{
	Pipeline::~Pipeline()
	{
		vkDestroyPipelineLayout(Graphics::Get().GetDevice(), m_Layout, nullptr);
		vkDestroyPipeline(Graphics::Get().GetDevice(), m_Pipeline, nullptr);
	}
}  // namespace At0::Ray