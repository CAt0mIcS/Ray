#include "D3D11VertexShader.h"

#ifdef _WIN32

#include <RlUtil/TypeConvert.h>
#include <RlUtil/Exception.h>

#include <RlDebug/RlAssert.h>

namespace WRL = Microsoft::WRL;


namespace At0::Reyal
{
	D3D11VertexShader::D3D11VertexShader(const std::string_view filepath)
		: m_Name("")
	{
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
	{
		m_Name = name;
		RL_ASSERT(false, "Incomplete Implementation");
	}

	void D3D11VertexShader::Bind() const
	{
		s_Context->VSSetShader(m_pShader.Get(), nullptr, 0);
	}

	void D3D11VertexShader::Unbind() const
	{
		// TODO: Test
		s_Context->VSSetShader(nullptr, nullptr, 0);
	}
}


#endif // _WIN32