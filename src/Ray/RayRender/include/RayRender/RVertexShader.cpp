#include "RRpch.h"
#include "RVertexShader.h"

#include "RRendererAPI.h"
#include "Platform/DX11/RDX11VertexShader.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>



namespace At0::Ray
{
	Ref<VertexShader> VertexShader::CreateFromCompiled(std::string_view filepath)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[VertexShader::Create] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::D3D11:
			return MakeRef<DX11VertexShader>(filepath, true);
		case RendererAPI::OpenGL:
			break;
		}

		return nullptr;
	}

	Ref<VertexShader> VertexShader::CreateFromSource(std::string_view filepath)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[VertexShader::Create] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::D3D11:
			return MakeRef<DX11VertexShader>(filepath, false);
		case RendererAPI::OpenGL:
			break;
		}

		return nullptr;
	}
}