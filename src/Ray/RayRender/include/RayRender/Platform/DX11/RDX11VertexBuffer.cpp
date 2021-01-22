#include "RayRender/RRpch.h"

#ifdef _WIN32
#include "RDX11VertexBuffer.h"
#include "../../Core/RVertex.h"

#include <RayUtil/RException.h>


namespace At0::Ray
{
	DX11VertexBuffer::DX11VertexBuffer(const VertexData& data)
		: m_Strides(data.Layout().SizeBytes())
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = (uint32_t)data.SizeBytes();
		bd.StructureByteStride = m_Strides;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = data.Data();

		RAY_GFX_THROW_FAILED(GetDevice()->CreateBuffer(&bd, &sd, &m_pBuffer));
	}

	void DX11VertexBuffer::Bind()
	{
		const UINT offset = 0;
		GetContext()->IASetVertexBuffers(0, 1, m_pBuffer.GetAddressOf(), &m_Strides, &offset);
	}
}

#endif // _WIN32


