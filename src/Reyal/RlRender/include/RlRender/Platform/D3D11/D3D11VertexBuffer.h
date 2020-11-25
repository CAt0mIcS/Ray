#pragma once

#include "RlRender/RlRBase.h"

#include "RlRender/VertexBuffer.h"
#include "D3D11GraphicsResource.h"

struct ID3D11VertexBuffer;

namespace At0::Reyal
{
	class RLR_API D3D11VertexBuffer : public VertexBuffer, private D3D11GraphicsResource
	{
	public:
		D3D11VertexBuffer();

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexBuffer> m_pBuffer;
	};
}