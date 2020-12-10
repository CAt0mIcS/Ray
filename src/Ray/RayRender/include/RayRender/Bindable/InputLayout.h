#pragma once

#include "RlRBase.h"
#include "Bindable.h"
#include "RayRender/GraphicsResource.h"

#include <vector>

struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;

namespace At0::Ray
{
	class RR_API InputLayout : public Bindable, private GraphicsResource
	{
	public:
		InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* vertexShaderBytecode);
		virtual void Bind() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	};
}