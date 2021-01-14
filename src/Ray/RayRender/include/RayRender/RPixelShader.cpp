#include "RRpch.h"
#include "RPixelShader.h"

#include "RRendererAPI.h"
#include "Platform/DX11/RDX11PixelShader.h"

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
		case RendererAPI::D3D11:
			return MakeRef<DX11PixelShader>(filepath, true);
		case RendererAPI::OpenGL:
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
		case RendererAPI::D3D11:
			return MakeRef<DX11PixelShader>(filepath, false);
		case RendererAPI::OpenGL:
			break;
		}

		return nullptr;
	}
}