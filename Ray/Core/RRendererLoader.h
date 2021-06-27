
#pragma once

#include "Ray/RBase.h"
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RCommandPool.h>
#include <D:/dev/Cpp/Projects/Ray/RayRenderer/Core/RInstance.h>



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
		extern RrPFNCreateCommandPool CreateCommandPool;
		extern RrPFNDestroyCommandPool DestroyCommandPool;
		extern RrPFNInitialize Initialize;
		extern RrPFNDestroyInstance DestroyInstance;

	}  // namespace RendererAPI

	RAY_EXPORT void LoadRenderer(RendererAPI::Type type);
}  // namespace At0::Ray

