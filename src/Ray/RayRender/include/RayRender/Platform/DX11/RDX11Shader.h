#pragma once

#ifdef _WIN32
#include "../../RShader.h"
#include "RDX11GraphicsResources.h"

#include <wrl.h>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D10Blob;

namespace At0::Ray
{
	class RR_API DX11Shader : public Shader, DX11GraphicsResources
	{
		friend class DX11VertexBuffer;
	public:
		DX11Shader(std::string_view vertexFilepath, std::string_view pixelFilepath);

		virtual void Bind() override;

	private:
		// Shader buffer access functions for the InputLayout
		void* GetBufferPointer() const;
		size_t GetBufferSize() const;

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;

		Microsoft::WRL::ComPtr<ID3D10Blob> m_VertexShaderBlob;
	};
}

#endif // _WIN32