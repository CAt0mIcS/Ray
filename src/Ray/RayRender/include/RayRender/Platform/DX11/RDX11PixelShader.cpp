#include "../../RRpch.h"
#include "RDX11PixelShader.h"

#include <RayUtil/RTypeConvert.h>
#include <RayUtil/RException.h>


namespace At0::Ray
{
	DX11PixelShader::DX11PixelShader(std::string_view filepath, bool compiled)
	{
		if (compiled)
		{
			RAY_GFX_THROW_FAILED(D3DReadFileToBlob(Util::MultiByteToWideChar(filepath).c_str(), &m_pBlob));
		}
		else
		{
			uint32_t flags1 =
#ifndef NDEBUG
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
				0;
#endif

			Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
			RAY_GFX_THROW_FAILED(D3DCompileFromFile(
				Util::MultiByteToWideChar(filepath).c_str(),
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"main",
				"ps_5_0",
				flags1,
				0,
				&m_pBlob,
				&errorBlob
			));

			if (errorBlob)
				RAY_THROW_RUNTIME("[DX11VertexShader] Failed to compile Vertex Shader from file '{0}' with error: '{1}'", filepath, errorBlob->GetBufferPointer());
		}

		RAY_GFX_THROW_FAILED(GetDevice()->CreatePixelShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pPixelShader));
	}

	void DX11PixelShader::Bind()
	{
		GetContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	}
}



