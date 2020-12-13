#pragma once

#include "../RlRBase.h"
#include "Bindable.h"
#include "RayRender/GraphicsResource.h"

#include <wrl.h>

struct ID3D11PixelShader;


namespace At0::Ray
{
	class RR_API PixelShader : public Bindable, private GraphicsResource
	{
	public:
		PixelShader(std::string_view filepath);
		virtual void Bind() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	};
}