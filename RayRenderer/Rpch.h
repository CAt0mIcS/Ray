#pragma once

#include "RBase.h"

#if RR_RENDERER_API == Vulkan
	#include <vulkan/vulkan.h>
#elif RR_RENDERER_API == OpenGL
#endif
