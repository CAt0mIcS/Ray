#include "RRpch.h"
#include "RRendererAPI.h"

namespace At0::Ray
{
#ifdef _WIN32
	RendererAPI::API RendererAPI::s_API = RendererAPI::D3D11;
#elif defined(__linux__)
	RendererAPI::API RendererAPI::s_API = RendererAPI::OpenGL;
#endif

	RendererAPI::API RendererAPI::GetAPI()
	{
		return s_API;
	}

	void RendererAPI::SetAPI(API newAPI)
	{
		s_API = newAPI;
	}

	bool RendererAPI::Valid()
	{
		return RendererAPI::GetAPI() >= RendererAPI::API::FIRST && RendererAPI::GetAPI() <= RendererAPI::API::LAST
#ifndef _WIN32
			&& RendererAPI::GetAPI() != API::D3D11
#endif
			;
	}
}