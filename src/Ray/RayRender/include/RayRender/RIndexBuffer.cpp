#include "RRpch.h"
#include "RIndexBuffer.h"

#include "RRendererAPI.h"

#include "Platform/DX11/RDX11IndexBuffer.h"
#include "Platform/OpenGL/ROpenGLIndexBuffer.h"

#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RAssert.h>


namespace At0::Ray
{
	Scope<IndexBuffer> IndexBuffer::Create(std::initializer_list<uint32_t> indices)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[PixelShader::Create] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeScope<DX11IndexBuffer>(indices);
#endif
		case RendererAPI::OpenGL:	return MakeScope<OpenGLIndexBuffer>(indices);
		}

		return nullptr;
	}
}


