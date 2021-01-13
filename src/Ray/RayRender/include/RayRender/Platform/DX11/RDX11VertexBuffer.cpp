#include "RayRender/RRpch.h"
#include "RDX11VertexBuffer.h"

#include <RayUtil/RException.h>


namespace At0::Ray
{
	DX11VertexBuffer::DX11VertexBuffer()
	{
		D3D11_BUFFER_DESC bd{};


		D3D11_SUBRESOURCE_DATA sd{};

		RAY_GFX_THROW_FAILED(GetDevice()->CreateBuffer(&bd, &sd, &m_pBuffer));
	}

	void DX11VertexBuffer::Bind()
	{
		UINT stride = 0;
		UINT offset = 0;
		GetContext()->IASetVertexBuffers(0, 1, m_pBuffer.GetAddressOf(), &stride, &offset);
	}
}


