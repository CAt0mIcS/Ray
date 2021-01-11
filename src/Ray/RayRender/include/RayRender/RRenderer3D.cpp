#include "RRpch.h"
#include "RRenderer3D.h"

#include "RRendererAPI.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>

#include "Platform/DX11/RDX11Renderer3D.h"
//#include "Platform/OpenGL/ROpenGLRenderer3D.h"


namespace At0::Ray
{
	Ref<Renderer3D> Renderer3D::Create()
	{
		RAY_PROFILE_FUNCTION();

		RAY_MEXPECTS(RendererAPI::GetAPI() >= RendererAPI::API::FIRST && RendererAPI::GetAPI() <= RendererAPI::API::LAST, "[Renderer3D::Create] Invalid RendererAPI selected");
#ifndef _WIN32
		RAY_MEXPECTS(RendererAPI::GetAPI() != RendererAPI::API::D3D11, "[Renderer3D::Create] Cannot use Direct3D if platform is not Windows.");
#endif

		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::D3D11:
			return MakeRef<DX11Renderer3D>();
		case RendererAPI::OpenGL:
			break;
		}

		return nullptr;
	}
}


