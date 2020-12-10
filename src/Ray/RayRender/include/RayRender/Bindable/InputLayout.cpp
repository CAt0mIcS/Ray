#include "rlrpch.h"
#include "InputLayout.h"

#include <RayUtil/Exception.h>
#include <RayDebug/RInstrumentor.h>

namespace At0::Ray
{
	InputLayout::InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* vertexShaderBytecode)
	{
		RAY_PROFILE_FUNCTION();
		RAY_GFX_THROW_FAILED(s_pDevice->CreateInputLayout(
			layout.data(),
			layout.size(),
			vertexShaderBytecode->GetBufferPointer(),
			vertexShaderBytecode->GetBufferSize(),
			&m_pInputLayout
		));
	}

	void InputLayout::Bind()
	{
		s_pContext->IASetInputLayout(m_pInputLayout.Get());
	}
}


