#include "../../RRpch.h"

#ifdef _WIN32
#include "RDX11IndexBuffer.h"

#include <RayUtil/RException.h>
#include <RayDebug/RInstrumentor.h>


namespace At0::Ray
{
	DX11IndexBuffer::DX11IndexBuffer(std::initializer_list<uint32_t> indices)
	{
		D3D11_BUFFER_DESC bd{};
		bd.ByteWidth = sizeof(uint32_t) * indices.size();
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = sizeof(uint32_t);

		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = indices.begin();

		RAY_DX_THROW_FAILED(GetDevice()->CreateBuffer(&bd, &sd, &m_pIndexBuffer));
	}

	void DX11IndexBuffer::Bind()
	{
		GetContext()->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	uint32_t DX11IndexBuffer::GetIndicesCount() const
	{
		RAY_PROFILE_FUNCTION();
		// RAY_TODO: Test if it's faster to store indexcount as member variable

		D3D11_BUFFER_DESC bd{};
		m_pIndexBuffer->GetDesc(&bd);
		return bd.ByteWidth / sizeof(uint32_t);
	}
}

#endif // _WIN32


