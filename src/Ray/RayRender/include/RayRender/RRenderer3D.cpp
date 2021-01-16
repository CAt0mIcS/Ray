#include "RRpch.h"
#include "RRenderer3D.h"

#include "RRendererAPI.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>

#include "Platform/DX11/RDX11Renderer3D.h"
#include "Platform/OpenGL/ROpenGLRenderer3D.h"


namespace At0::Ray
{
	Ref<Renderer3D> Renderer3D::Create(void* window)
	{
		RAY_PROFILE_FUNCTION();

		RAY_MEXPECTS(RendererAPI::Valid(), "[Renderer3D::Create] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeRef<DX11Renderer3D>((HWND)window);
#endif
		case RendererAPI::OpenGL:	return MakeRef<OpenGLRenderer3D>((GLFWwindow*)window);
			break;
		}

		return nullptr;
	}
}


