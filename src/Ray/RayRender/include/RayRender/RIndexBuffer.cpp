#include "RRpch.h"
#include "RIndexBuffer.h"

#include "RRendererAPI.h"

#include "Platform/DX11/RDX11IndexBuffer.h"

#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RAssert.h>


namespace At0::Ray
{
	Ref<IndexBuffer> IndexBuffer::Create(std::initializer_list<uint32_t> indices)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[PixelShader::Create] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::D3D11: return MakeRef<DX11IndexBuffer>(indices);
		case RendererAPI::OpenGL: break;
		}

		return nullptr;
	}
}


