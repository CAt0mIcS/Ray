#include "RRenderContext.h"

#include "Devices/RWindow.h"


namespace At0::Ray
{
	RenderContext::RenderContext(
		Window& window, const VulkanInstance& instance, const PhysicalDevice& physicalDevice)
		: instance(instance), physicalDevice(physicalDevice), surface{ instance, window },
		  device{ physicalDevice, surface }, graphics{ window, *this }
	{
	}
	RenderContext::~RenderContext() {}
}  // namespace At0::Ray