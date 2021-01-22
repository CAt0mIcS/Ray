#pragma once

#ifdef _WIN32
#include "../../RVertexBuffer.h"
#include "RDX11GraphicsResources.h"

#include <wrl.h>

struct ID3D11Buffer;
struct ID3D11InputLayout;


namespace At0::Ray
{
	class VertexShader;
	class DX11VertexShader;
	class VertexLayout;

	class RR_API DX11VertexBuffer : public VertexBuffer, DX11GraphicsResources
	{
	public:
		DX11VertexBuffer(const VertexData& data, const Scope<VertexShader>& vShader);

		virtual void Bind() override;

	private:
		void SetInputLayout(const VertexLayout& layout, const DX11VertexShader* vShader);

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pLayout;
		uint32_t m_Strides;
	};
}

#endif // _WIN32