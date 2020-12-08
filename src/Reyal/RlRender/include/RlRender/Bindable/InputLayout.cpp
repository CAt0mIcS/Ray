#include "rlrpch.h"
#include "InputLayout.h"

#include <RlUtil/Exception.h>
#include <RlDebug/Instrumentor.h>

namespace At0::Reyal
{
	InputLayout::InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* vertexShaderBytecode)
	{
		RL_PROFILE_FUNCTION();
		RL_GFX_THROW_FAILED(s_pDevice->CreateInputLayout(
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


