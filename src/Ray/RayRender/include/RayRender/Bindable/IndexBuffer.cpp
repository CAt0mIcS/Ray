#include "rlrpch.h"
#include "IndexBuffer.h"

#include <RayUtil/Exception.h>
#include <RayDebug/RInstrumentor.h>


namespace At0::Ray
{
    IndexBuffer::IndexBuffer(const std::vector<uint16_t>& indices)
        : m_Count(indices.size())
    {
		RAY_PROFILE_FUNCTION();
		D3D11_BUFFER_DESC ibd{};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = uint32_t(m_Count * sizeof(uint16_t));
		ibd.StructureByteStride = sizeof(uint16_t);

		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices.data();
		RAY_GFX_THROW_FAILED(s_pDevice->CreateBuffer(&ibd, &isd, &m_pIndexBuffer));
    }
    
    void IndexBuffer::Bind()
    {
		s_pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
    }
}