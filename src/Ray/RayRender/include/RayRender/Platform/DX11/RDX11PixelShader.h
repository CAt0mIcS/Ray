#pragma once

#include "../../RPixelShader.h"
#include "RDX11GraphicsResources.h"

#include <wrl.h>


struct ID3D10Blob;
struct ID3D11PixelShader;


namespace At0::Ray
{
	class RR_API DX11PixelShader : public PixelShader, DX11GraphicsResources
	{
	public:
		DX11PixelShader(std::string_view filepath, bool compiled);

		virtual void Bind() override;

	private:
		Microsoft::WRL::ComPtr<ID3D10Blob> m_pBlob;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	};
}