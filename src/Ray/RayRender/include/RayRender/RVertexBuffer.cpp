#include "RRpch.h"
#include "RVertexBuffer.h"

#include "RRendererAPI.h"

#include <RayDebug/RAssert.h>

#include "Platform/DX11/RDX11VertexBuffer.h"


namespace At0::Ray
{
	Ref<VertexBuffer> VertexBuffer::Create(const std::vector<Vertex>& data)
	{
		RAY_MEXPECTS(RendererAPI::Valid(), "[VertexBuffer::Create] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::D3D11: return MakeRef<DX11VertexBuffer>(data);
		case RendererAPI::OpenGL: return nullptr;
		}

		return nullptr;
	}
}