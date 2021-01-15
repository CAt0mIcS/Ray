#pragma once

#include "../../RInputLayout.h"
#include "RDX11GraphicsResources.h"

#include <wrl.h>


struct ID3D11InputLayout;


namespace At0::Ray
{
	class VertexShader;

	uint32_t ShaderDataTypeSize(ShaderDataType type);

	class RR_API DX11InputLayout : public InputLayout, DX11GraphicsResources
	{
	public:
		DX11InputLayout(std::initializer_list<InputElement> inputElements, const VertexShader* vShader);

		virtual void Bind() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pLayout;
	};
}