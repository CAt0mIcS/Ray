#include "RayRender/RRpch.h"

#ifdef _WIN32
#include "RDX11VertexBuffer.h"

#include <RayUtil/RException.h>


namespace At0::Ray
{
	DX11VertexBuffer::DX11VertexBuffer(std::initializer_list<Vertex> data)
		: m_Strides(sizeof(Vertex))
	{
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vertex) * data.size();
		bd.StructureByteStride = sizeof(Vertex);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = data.begin();

		RAY_GFX_THROW_FAILED(GetDevice()->CreateBuffer(&bd, &sd, &m_pBuffer));
	}

	void DX11VertexBuffer::Bind()
	{
		UINT offset = 0;
		GetContext()->IASetVertexBuffers(0, 1, m_pBuffer.GetAddressOf(), &m_Strides, &offset);
	}
}

#endif // _WIN32


