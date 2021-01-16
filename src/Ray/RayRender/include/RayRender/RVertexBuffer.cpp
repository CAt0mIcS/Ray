#include "RRpch.h"
#include "RVertexBuffer.h"

#include "RRendererAPI.h"

#include <RayDebug/RAssert.h>

#include "Platform/DX11/RDX11VertexBuffer.h"
#include "Platform/OpenGL/ROpenGLVertexBuffer.h"


namespace At0::Ray
{
	Ref<VertexBuffer> VertexBuffer::Create(std::initializer_list<Vertex> data)
	{
		RAY_MEXPECTS(RendererAPI::Valid(), "[VertexBuffer::Create] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeRef<DX11VertexBuffer>(data);
#endif
		case RendererAPI::OpenGL:	return MakeRef<OpenGLVertexBuffer>(data);
		}

		return nullptr;
	}
}
