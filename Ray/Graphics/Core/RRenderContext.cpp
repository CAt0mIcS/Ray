#include "RRenderContext.h"

#include "Devices/RWindow.h"


namespace At0::Ray
{
	RenderContext::RenderContext(Window& window, const EngineRenderContext& engineContext)
		: instance(engineContext.instance), physicalDevice(engineContext.physicalDevice),
		  surface{ instance, window }, device{ physicalDevice, surface },
		  graphics{ instance, physicalDevice, surface, device }
	{
	}
}  // namespace At0::Ray