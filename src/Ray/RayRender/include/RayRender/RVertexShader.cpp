#include "RRpch.h"
#include "RVertexShader.h"

#include "RRendererAPI.h"
#include "Platform/DX11/RDX11VertexShader.h"
#include "Platform/OpenGL/ROpenGLVertexShader.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>



namespace At0::Ray
{
	Scope<VertexShader> VertexShader::CreateFromCompiled(std::string_view filepath)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[VertexShader::Create] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeScope<DX11VertexShader>(filepath, true);
#endif
		case RendererAPI::OpenGL:	return MakeScope<OpenGLVertexShader>(filepath, true);
			break;
		}

		return nullptr;
	}

	Scope<VertexShader> VertexShader::CreateFromSource(std::string_view filepath)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[VertexShader::Create] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeScope<DX11VertexShader>(filepath, false);
#endif
		case RendererAPI::OpenGL:	return MakeScope<OpenGLVertexShader>(filepath, false);
			break;
		}

		return nullptr;
	}
}
