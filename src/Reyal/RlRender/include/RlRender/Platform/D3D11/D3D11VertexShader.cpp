#include "D3D11VertexShader.h"

#ifdef _WIN32

#include <RlUtil/TypeConvert.h>
#include <RlUtil/Exception.h>

#include <RlDebug/RlAssert.h>
#include <RlDebug/Instrumentor.h>
#include <RlDebug/ReyalLogger.h>

namespace WRL = Microsoft::WRL;


namespace At0::Reyal
{
	D3D11VertexShader::D3D11VertexShader(const std::string_view filepath)
		: m_Name("")
	{
		RL_PROFILE_FUNCTION();

		RL_GFX_THROW_FAILED(D3DReadFileToBlob(Util::MultiByteToWideChar(filepath).c_str(), &m_pBlob));
		RL_GFX_THROW_FAILED(s_Device->CreateVertexShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pShader));

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}
	
	D3D11VertexShader::D3D11VertexShader(const std::string_view name, const std::string_view vertexSrc)
		: m_Name(name)
	{
		RL_PROFILE_FUNCTION();
		
		RL_GFX_THROW_FAILED(D3DCompile(
			vertexSrc.data(),
			vertexSrc.size(),
			NULL,
			nullptr,
			nullptr,
			"main", "ps_4_0",
			D3DCOMPILE_ENABLE_STRICTNESS,
			0,
			&m_pBlob,
			nullptr
		));

		RL_GFX_THROW_FAILED(s_Device->CreateVertexShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pShader));
	}

	void D3D11VertexShader::Bind() const
	{
		RL_PROFILE_FUNCTION();
		s_Context->VSSetShader(m_pShader.Get(), nullptr, 0);
	}

	void D3D11VertexShader::Unbind() const
	{
		RL_PROFILE_FUNCTION();
		s_Context->VSSetShader(nullptr, nullptr, 0);
	}
}


#endif // _WIN32