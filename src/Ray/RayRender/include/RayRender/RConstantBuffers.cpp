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


	template<typename T>
	Scope<ConstantBuffer> PixelConstantBuffer::Create()
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[ConstantBuffer::Create(Matrix)] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeScope<DX11PixelConstantBuffer<T>>();
#endif
		case RendererAPI::OpenGL:	return MakeScope<OpenGLPixelConstantBuffer<T>>();
		}

		return nullptr;
	}

	template<typename T>
	Scope<ConstantBuffer> VertexConstantBuffer::Create()
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[ConstantBuffer::Create(Matrix)] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeScope<DX11VertexConstantBuffer<T>>();
#endif
		case RendererAPI::OpenGL:	return MakeScope<OpenGLVertexConstantBuffer<T>>();
		}

		return nullptr;
	}
	
	
#ifdef __GNUC__
	// RAY_TODO: Add more data
	// -----------------------------------------------------------------
	// explicit template instantiation

	// PixelConstantBuffer
	template Scope<ConstantBuffer> PixelConstantBuffer::Create<Matrix>(const Matrix& data);
	template Scope<ConstantBuffer> PixelConstantBuffer::Create<Float3>(const Float3& data);
	template Scope<ConstantBuffer> PixelConstantBuffer::Create<Matrix>();
	template Scope<ConstantBuffer> PixelConstantBuffer::Create<Float3>();

	// VertexConstantBuffer
	template Scope<ConstantBuffer> VertexConstantBuffer::Create<Matrix>(const Matrix& data);
	template Scope<ConstantBuffer> VertexConstantBuffer::Create<Float3>(const Float3& data);
	template Scope<ConstantBuffer> VertexConstantBuffer::Create<Matrix>();
	template Scope<ConstantBuffer> VertexConstantBuffer::Create<Float3>();
	
#endif
	
}

