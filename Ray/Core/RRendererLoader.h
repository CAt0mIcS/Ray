
#pragma once

#include "Ray/RBase.h"
#include <RayRenderer/Core/RCore.h>


namespace At0::Ray
{
	namespace RendererAPI
	{
		enum Type
		{
			OpenGL,
			Vulkan
		};


		extern Type API;
		extern RrPFNInitialize Initialize;
		extern RrPFNDestroyInstance DestroyInstance;

	}  // namespace RendererAPI

	RAY_EXPORT void LoadRenderer(RendererAPI::Type type);
}  // namespace At0::Ray

