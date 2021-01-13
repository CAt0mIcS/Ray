#pragma once

#include "../../RVertexBuffer.h"
#include "RDX11GraphicsResources.h"

#include <wrl.h>

struct ID3D11Buffer;


namespace At0::Ray
{
	class RR_API DX11VertexBuffer : public VertexBuffer, DX11GraphicsResources
	{
	public:
		DX11VertexBuffer(const std::vector<Vertex>& data);

		virtual void Bind() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;
		uint32_t m_Strides;
	};
}