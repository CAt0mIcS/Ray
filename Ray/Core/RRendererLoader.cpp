#include "Rpch.h"
#include "RRendererLoader.h"

#include <RayBase/RException.h>
#include <Windows.h>


namespace At0::Ray
{
	namespace RendererAPI
	{
		Type API;
		RrPFNInitialize Initialize = nullptr;
		RrPFNDestroyInstance DestroyInstance = nullptr;
	}  // namespace RendererAPI


	void LoadRenderer(RendererAPI::Type type)
	{
		// RAY_TODO: Parse automatically
		// RAY_TODO: Make platform independent

		HMODULE lib = nullptr;
		switch (type)
		{
		case RendererAPI::OpenGL: lib = LoadLibraryA("RayRendererOpenGL.dll"); break;
		case RendererAPI::Vulkan: lib = LoadLibraryA("RayRendererVulkan.dll"); break;
		default: ThrowRuntime("Invalid renderer type {0}", (uint32_t)type);
		}

		if (!lib)
		{
			std::string rendererStr;
			switch (type)
			{
			case RendererAPI::OpenGL: rendererStr = "OpenGL"; break;
			case RendererAPI::Vulkan: rendererStr = "Vulkan"; break;
			}

			ThrowRuntime("[Loader] Failed to load {0} renderer", rendererStr);
		}


		RendererAPI::Initialize = (RrPFNInitialize)GetProcAddress(lib, "RrInitialize");
		RendererAPI::DestroyInstance =
			(RrPFNDestroyInstance)GetProcAddress(lib, "RrDestroyInstance");

		RendererAPI::API = type;
	}
}  // namespace At0::Ray
