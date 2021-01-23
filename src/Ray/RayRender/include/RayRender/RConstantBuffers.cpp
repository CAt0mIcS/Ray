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
	Scope<ConstantBuffer> PixelConstantBuffer::Create(std::string_view name, const Shader* pShader, const T& data)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[ConstantBuffer::Create(Matrix)] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeScope<DX11PixelConstantBuffer<T>>(data);
#endif
		case RendererAPI::OpenGL:	return MakeScope<OpenGLPixelConstantBuffer<T>>(name, pShader, data);
		}

		return nullptr;
	}

	template<typename T>
	Scope<ConstantBuffer> VertexConstantBuffer::Create(std::string_view name, const Shader* pShader, const T& data)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[ConstantBuffer::Create(Matrix)] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeScope<DX11VertexConstantBuffer<T>>(data);
#endif
		case RendererAPI::OpenGL:	return MakeScope<OpenGLVertexConstantBuffer<T>>(name, pShader, data);
		}

		return nullptr;
	}


	template<typename T>
	Scope<ConstantBuffer> PixelConstantBuffer::Create(std::string_view name, const Shader* pShader)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[ConstantBuffer::Create(Matrix)] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeScope<DX11PixelConstantBuffer<T>>();
#endif
		case RendererAPI::OpenGL:	return MakeScope<OpenGLPixelConstantBuffer<T>>(name, pShader);
		}

		return nullptr;
	}

	template<typename T>
	Scope<ConstantBuffer> VertexConstantBuffer::Create(std::string_view name, const Shader* pShader)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(RendererAPI::Valid(), "[ConstantBuffer::Create(Matrix)] Invalid RendererAPI selected");

		switch (RendererAPI::GetAPI())
		{
#ifdef _WIN32
		case RendererAPI::D3D11:	return MakeScope<DX11VertexConstantBuffer<T>>();
#endif
		case RendererAPI::OpenGL:	return MakeScope<OpenGLVertexConstantBuffer<T>>(name, pShader);
		}

		return nullptr;
	}


#ifdef __GNUC__
	// RAY_TODO: Add more data
	// -----------------------------------------------------------------
	// explicit template instantiation

	// PixelConstantBuffer
	template Scope<ConstantBuffer> PixelConstantBuffer::Create<Matrix>(std::string_view name, const Shader* pShader, const Matrix& data);
	template Scope<ConstantBuffer> PixelConstantBuffer::Create<Float3>(std::string_view name, const Shader* pShader, const Float3& data);
	template Scope<ConstantBuffer> PixelConstantBuffer::Create<Matrix>(std::string_view name, const Shader* pShader);
	template Scope<ConstantBuffer> PixelConstantBuffer::Create<Float3>(std::string_view name, const Shader* pShader);

	// VertexConstantBuffer
	template Scope<ConstantBuffer> VertexConstantBuffer::Create<Matrix>(std::string_view name, const Shader* pShader, const Matrix& data);
	template Scope<ConstantBuffer> VertexConstantBuffer::Create<Float3>(std::string_view name, const Shader* pShader, const Float3& data);
	template Scope<ConstantBuffer> VertexConstantBuffer::Create<Matrix>(std::string_view name, const Shader* pShader);
	template Scope<ConstantBuffer> VertexConstantBuffer::Create<Float3>(std::string_view name, const Shader* pShader);

#endif

}

