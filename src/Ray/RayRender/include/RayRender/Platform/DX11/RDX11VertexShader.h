#pragma once

#include "../../RVertexShader.h"
#include "RDX11GraphicsResources.h"

#include <wrl.h>


struct ID3D10Blob;
struct ID3D11VertexShader;

namespace At0::Ray
{
	class RR_API DX11VertexShader : public VertexShader, DX11GraphicsResources
	{
	public:
		DX11VertexShader(std::string_view filepath, bool compiled);

		virtual void Bind() override;

	private:
		Microsoft::WRL::ComPtr<ID3D10Blob> m_pBlob;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	};
}