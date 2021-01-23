#include "RRpch.h"
#include "RShader.h"

#include "RRendererAPI.h"
#include "Platform/DX11/RDX11Shader.h"
#include "Platform/OpenGL/ROpenGLShader.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>


namespace At0::Ray
{
	Scope<Shader> Shader::Create(std::string_view vertexFilepath, std::string_view pixelFilepath)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[Shader::Create] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeScope<DX11Shader>(vertexFilepath, pixelFilepath);
#endif
		case RendererAPI::OpenGL:	return MakeScope<OpenGLShader>(vertexFilepath, pixelFilepath);
			break;
		}

		return nullptr;
	}
}
