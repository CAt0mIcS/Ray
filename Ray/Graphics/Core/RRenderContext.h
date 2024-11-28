#pragma once

#include "../../RBase.h"

#include "RSurface.h"
#include "RLogicalDevice.h"
#include "../RGraphics.h"


namespace At0::Ray
{
	class VulkanInstance;
	class PhysicalDevice;
	class Window;

	struct EngineRenderContext
	{
		const VulkanInstance& instance;
		const PhysicalDevice& physicalDevice;
	};

	struct RenderContext
	{
		RenderContext(Window& window, const EngineRenderContext& engineContext);
		~RenderContext();

		const VulkanInstance& instance;
		const PhysicalDevice& physicalDevice;
		Surface surface;
		LogicalDevice device;
		Graphics graphics;
	};
}  // namespace At0::Ray
