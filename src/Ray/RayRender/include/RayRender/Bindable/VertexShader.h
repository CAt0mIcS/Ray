#pragma once

#include "../RlRBase.h"
#include "Bindable.h"
#include "RayRender/GraphicsResource.h"

#include <wrl.h>
#include <d3dcommon.h>

struct ID3D11VertexShader;


namespace At0::Ray
{
	class RR_API VertexShader : public Bindable, private GraphicsResource
	{
	public:
		VertexShader(const std::string_view filepath);
		virtual void Bind() override;
		ID3DBlob* GetBytecode() const;

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> m_pBlob;
	};
}