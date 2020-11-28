#include "rlrpch.h"
#include "RendererAPI.h"


namespace At0::Reyal
{
#ifdef _WIN32
	RendererAPI::API RendererAPI::s_API = RendererAPI::D3D11;
#elif defined(__linux__)
	RendererAPI::API RendererAPI::s_API = RendererAPI::OpenGL;
#else
	RendererAPI::API RendererAPI::s_API = RendererAPI::None;
#endif
}