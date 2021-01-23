#include "RRpch.h"
#include "RVertexBuffer.h"

#include "RRendererAPI.h"

#include <RayDebug/RAssert.h>

#include "Platform/DX11/RDX11VertexBuffer.h"
#include "Platform/OpenGL/ROpenGLVertexBuffer.h"


namespace At0::Ray
{
	Scope<VertexBuffer> VertexBuffer::Create(const VertexData& data, const Scope<Shader>& vShader)
	{
		RAY_MEXPECTS(RendererAPI::Valid(), "[VertexBuffer::Create] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeScope<DX11VertexBuffer>(data, vShader);
#endif
		case RendererAPI::OpenGL:	return MakeScope<OpenGLVertexBuffer>(data, vShader);
		}

		return nullptr;
	}
}
