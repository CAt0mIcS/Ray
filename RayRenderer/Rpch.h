#pragma once

#include "RBase.h"

#if RR_RENDERER_API == RR_RENDERER_API_VULKAN
	#include <vulkan/vulkan.h>
#elif RR_RENDERER_API == RR_RENDERER_API_OPENGL
	#include <Glad/include/glad/glad.h>
#endif
