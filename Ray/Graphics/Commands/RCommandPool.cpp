#include "Rpch.h"
#include "RCommandPool.h"

#include "Graphics/RGraphics.h"
#include "Graphics/Core/RLogicalDevice.h"
#include "Core/RRendererLoader.h"

#include "RayBase/RException.h"


namespace At0::Ray
{
	CommandPool::CommandPool(RrCommandPoolCreateFlags flags)
	{
		RrCommandPoolCreateInfo createInfo{};
		createInfo.flags = flags;
		createInfo.queueFamilyIndex = Graphics::Get().GetDevice().GetGraphicsFamily();

		ThrowRenderError(RendererAPI::CreateCommandPool(
							 Graphics::Get().GetDevice(), &createInfo, &m_CommandPool),
			"[CommandPool] Failed to create");
	}

	CommandPool::~CommandPool()
	{
		RendererAPI::DestroyCommandPool(Graphics::Get().GetDevice(), m_CommandPool);
	}
}  // namespace At0::Ray
