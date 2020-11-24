#include "D3D11PixelShader.h"

#ifdef _WIN32

#include <RlUtil/TypeConvert.h>
#include <RlUtil/Exception.h>

#include <RlDebug/RlAssert.h>
#include <RlDebug/Instrumentor.h>
#include <RlDebug/ReyalLogger.h>

namespace WRL = Microsoft::WRL;


namespace At0::Reyal
{
	D3D11PixelShader::D3D11PixelShader(const std::string_view filepath, FileState state)
		: m_Name("")
	{
		RL_PROFILE_FUNCTION();

		WRL::ComPtr<ID3DBlob> pBlob;
		switch (state)
		{
		case FileState::Compiled:
		{
			RL_GFX_THROW_FAILED(D3DReadFileToBlob(
				Util::MultiByteToWideChar(filepath).c_str(),
				&pBlob
			));

			break;
		}
		case FileState::Source:
		{
			UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifndef NDEBUG
			flags |= D3DCOMPILE_DEBUG;
#endif

			WRL::ComPtr<ID3DBlob> pErrorBlob;
			HRESULT hr = D3DCompileFromFile(
				Util::MultiByteToWideChar(filepath).c_str(),
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"main", 
				"ps_5_0",
				flags, 0,
				&pBlob,
				&pErrorBlob
			);
			
			if (FAILED(hr))
			{
				OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
				RL_LOG_ERROR("[D3D11PixelShader] Compilation failed with message: {0}", (char*)pErrorBlob->GetBufferPointer());
			}
			RL_GFX_THROW_FAILED(hr);

			break;
		}
		default:
		{
			RL_ASSERT(false, "[D3D11PixelShader] Unknown FileState option: {0}", (int)state);
		}
		}
		
		RL_GFX_THROW_FAILED(s_Device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pShader));


		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}
	
	D3D11PixelShader::D3D11PixelShader(const std::string_view name, const std::string_view pixelSrc)
		: m_Name(name)
	{
		RL_PROFILE_FUNCTION();
		
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifndef NDEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif

		WRL::ComPtr<ID3DBlob> pBlob;
		WRL::ComPtr<ID3DBlob> pErrorBlob;
		HRESULT hr = D3DCompile(
			pixelSrc.data(),
			pixelSrc.size(),
			NULL,
			nullptr,
			nullptr,
			"main", 
			"vs_5_0",
			flags, 0,
			&pBlob,
			&pErrorBlob
		);

		if(FAILED(hr))
			RL_LOG_ERROR("[D3D11PixelShader] Compilation failed with message: {0}", (char*)pErrorBlob->GetBufferPointer());
		RL_GFX_THROW_FAILED(hr);

		RL_GFX_THROW_FAILED(s_Device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pShader));
	}

	D3D11PixelShader::~D3D11PixelShader()
	{
		
	}

	void D3D11PixelShader::Bind() const
	{
		RL_PROFILE_FUNCTION();
		s_Context->PSSetShader(m_pShader.Get(), nullptr, 0);
	}

	void D3D11PixelShader::Unbind() const
	{
		RL_PROFILE_FUNCTION();
		s_Context->PSSetShader(nullptr, nullptr, 0);
	}
	
	std::string D3D11PixelShader::ReadFile(const std::string_view filepath)
	{
		RL_PROFILE_FUNCTION();

		std::string result;
		std::ifstream reader(filepath.data(), std::ios::in | std::ios::binary);

		if (reader)
		{
			reader.seekg(0, std::ios::end);
			size_t size = reader.tellg();
			if (size != -1)
			{
				result.resize(size);
				reader.seekg(0, std::ios::beg);
				reader.read(&result[0], size);
			}
			else
			{
				RL_LOG_ERROR("[OpenGLPixelShader] Was unable to read the file {0}", filepath);
			}
		}
		else
		{
			RL_LOG_ERROR("[OpenGLPixelShader] Was unable to open the file {0}", filepath);
		}

		return result;
	}
}


#endif // _WIN32