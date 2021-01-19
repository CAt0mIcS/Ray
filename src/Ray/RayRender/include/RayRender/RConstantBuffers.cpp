#include "RRpch.h"
#include "RConstantBuffers.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>

#include "RRendererAPI.h"

#include "Platform/DX11/RDX11ConstantBuffers.h"
#include "Platform/OpenGL/ROpenGLConstantBuffers.h"


namespace At0::Ray
{
	template<typename T>
	Scope<ConstantBuffer> PixelConstantBuffer::Create(const T& data)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[ConstantBuffer::Create(Matrix)] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeScope<DX11PixelConstantBuffer<T>>(data);
#endif
		case RendererAPI::OpenGL:	return MakeScope<OpenGLPixelConstantBuffer<T>>(data);
		}

		return nullptr;
	}

	template<typename T>
	Scope<ConstantBuffer> VertexConstantBuffer::Create(const T& data)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[ConstantBuffer::Create(Matrix)] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeScope<DX11VertexConstantBuffer<T>>(data);
#endif
		case RendererAPI::OpenGL:	return MakeScope<OpenGLVertexConstantBuffer<T>>(data);
		}

		return nullptr;
	}
}

