#include "rlrpch.h"
#include "RlRender/RendererAPI.h"

#include "RlRender/Platform/D3D11/D3D11RendererAPI.h"
#include "RlRender/Platform/OpenGL/OpenGLRendererAPI.h"
#include "RlRender/Platform/X11/X11RendererAPI.h"

#include <RlDebug/RlAssert.h>
#include <RlDebug/ReyalLogger.h>


namespace At0::Reyal
{
	// Set default RendererAPI for every platform
#ifdef _WIN32
	RendererAPI::API RendererAPI::s_Api = RendererAPI::API::D3D11;
#elif defined(__linux__)
	RendererAPI::API RendererAPI::s_Api = RendererAPI::API::OpenGL;
#endif

	std::unique_ptr<RendererAPI> RendererAPI::Create()
	{
		RL_EXPECTS(s_Api >= API::FIRST && s_Api <= API::LAST);

		switch (GetAPI())
		{
		case API::None:				RL_LOG_WARN("[RendererAPI] Returning invalid RendererAPI. Calls to the API will fail!"); return nullptr;
#ifdef _WIN32
		case API::D3D11:			return std::make_unique<D3D11RendererAPI>();
#endif
		case API::OpenGL:			return std::make_unique<OpenGLRendererAPI>();
#ifdef __linux__
		case API::X11:				return std::make_unique<X11RendererAPI>();
#endif
		}

		RL_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}

