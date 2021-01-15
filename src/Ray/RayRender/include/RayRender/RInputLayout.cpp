#include "RRpch.h"
#include "RInputLayout.h"

#include "RRendererAPI.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>

#include "Platform/DX11/RDX11InputLayout.h"


namespace At0::Ray
{
	Ref<InputLayout> InputLayout::Create(std::initializer_list<InputElement> inputElements, const Ref<VertexShader>& vShader)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[InputLayout::Create] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::D3D11: return MakeRef<DX11InputLayout>(inputElements, vShader.get());
		case RendererAPI::OpenGL: break;
		}

		return nullptr;
	}
}