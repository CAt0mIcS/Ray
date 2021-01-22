#pragma once

#ifdef _WIN32
#include "../../RVertexShader.h"
#include "RDX11GraphicsResources.h"

#include <wrl.h>


struct ID3D10Blob;
struct ID3D11VertexShader;

namespace At0::Ray
{
	class RR_API DX11VertexShader : public VertexShader, DX11GraphicsResources
	{
		friend class DX11VertexBuffer;
	public:
		DX11VertexShader(std::string_view filepath, bool compiled);

		virtual void Bind() override;

	private:
		// Shader buffer access functions for the InputLayout
		void* GetBufferPointer() const;
		size_t GetBufferSize() const;

	private:
		Microsoft::WRL::ComPtr<ID3D10Blob> m_pBlob;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	};
}

#endif // _WIN32