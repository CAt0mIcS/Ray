#include "../../RRpch.h"

#ifdef _WIN32
#include "RDX11Shader.h"

#include <RayUtil/RException.h>
#include <RayUtil/RTypeConvert.h>

namespace At0::Ray
{
	DX11Shader::DX11Shader(std::string_view vertexFilepath, std::string_view pixelFilepath)
	{
		uint32_t flags1 =
#ifndef NDEBUG
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
			0;
#endif
		// ------------------------------------------------------------
		// Pixel shader compilation
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
		RAY_GFX_THROW_FAILED(D3DCompileFromFile(
			Util::MultiByteToWideChar(pixelFilepath).c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			"ps_5_0",
			flags1,
			0,
			&m_VertexShaderBlob,
			&errorBlob
		));

		if (errorBlob)
			RAY_THROW_RUNTIME("[DX11Shader] Failed to compile Vertex Shader from file '{0}' with error: '{1}'", vertexFilepath, errorBlob->GetBufferPointer());

		RAY_GFX_THROW_FAILED(GetDevice()->CreatePixelShader(m_VertexShaderBlob->GetBufferPointer(), m_VertexShaderBlob->GetBufferSize(), nullptr, &m_PixelShader));

		// ------------------------------------------------------------
		// Vertex shader compilation
		RAY_GFX_THROW_FAILED(D3DCompileFromFile(
			Util::MultiByteToWideChar(vertexFilepath).c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			"vs_5_0",
			flags1,
			0,
			&m_VertexShaderBlob,
			&errorBlob
		));

		if (errorBlob)
			RAY_THROW_RUNTIME("[DX11Shader] Failed to compile Vertex Shader from file '{0}' with error: '{1}'", vertexFilepath, errorBlob->GetBufferPointer());

		RAY_GFX_THROW_FAILED(GetDevice()->CreateVertexShader(m_VertexShaderBlob->GetBufferPointer(), m_VertexShaderBlob->GetBufferSize(), nullptr, &m_VertexShader));
	}

	void DX11Shader::Bind()
	{
		GetContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0);
		GetContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0);
	}

	void* DX11Shader::GetBufferPointer() const
	{
		return m_VertexShaderBlob->GetBufferPointer();
	}

	size_t DX11Shader::GetBufferSize() const
	{
		return m_VertexShaderBlob->GetBufferSize();
	}
}

#endif // _WIN32