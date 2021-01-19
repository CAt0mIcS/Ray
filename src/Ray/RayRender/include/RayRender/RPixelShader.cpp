#include "RRpch.h"
#include "RPixelShader.h"

#include "RRendererAPI.h"
#include "Platform/DX11/RDX11PixelShader.h"
#include "Platform/OpenGL/ROpenGLPixelShader.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>



namespace At0::Ray
{
	Scope<PixelShader> PixelShader::CreateFromCompiled(std::string_view filepath)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[PixelShader::Create] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeScope<DX11PixelShader>(filepath, true);
#endif
		case RendererAPI::OpenGL:	return MakeScope<OpenGLPixelShader>(filepath, true);
			break;
		}

		return nullptr;
	}

	Scope<PixelShader> PixelShader::CreateFromSource(std::string_view filepath)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[PixelShader::Create] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeScope<DX11PixelShader>(filepath, false);
#endif
		case RendererAPI::OpenGL:	return MakeScope<OpenGLPixelShader>(filepath, false);
		}

		return nullptr;
	}
}
