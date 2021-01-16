#include "RRpch.h"
#include "RPixelShader.h"

#include "RRendererAPI.h"
#include "Platform/DX11/RDX11PixelShader.h"
#include "Platform/OpenGL/ROpenGLPixelShader.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>



namespace At0::Ray
{
	Ref<PixelShader> PixelShader::CreateFromCompiled(std::string_view filepath)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[PixelShader::Create] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeRef<DX11PixelShader>(filepath, true);
#endif
		case RendererAPI::OpenGL:	return MakeRef<OpenGLPixelShader>(filepath, true);
			break;
		}

		return nullptr;
	}

	Ref<PixelShader> PixelShader::CreateFromSource(std::string_view filepath)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[PixelShader::Create] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeRef<DX11PixelShader>(filepath, false);
#endif
		case RendererAPI::OpenGL:	return MakeRef<OpenGLPixelShader>(filepath, false);
		}

		return nullptr;
	}
}
