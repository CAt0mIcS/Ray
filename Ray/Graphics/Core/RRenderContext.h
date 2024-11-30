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

	struct RenderContext
	{
		RenderContext(
			Window& window, const VulkanInstance& instance, const PhysicalDevice& physicalDevice);
		~RenderContext();

		const VulkanInstance& instance;
		const PhysicalDevice& physicalDevice;
		Surface surface;
		LogicalDevice device;
		Graphics graphics;
	};
}  // namespace At0::Ray
