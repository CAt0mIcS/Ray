#include "RayRender/rlrpch.h"
#include "VertexBuffer.h"

#include <RayUtil/Exception.h>
#include <RayDebug/RInstrumentor.h>


namespace At0::Ray
{
    void VertexBuffer::Bind()
    {
        uint32_t offset = 0;
        s_pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &m_Strides, &offset);
    }
    
    inline void VertexBuffer::CreateBuffer(const D3D11_BUFFER_DESC& bd, const D3D11_SUBRESOURCE_DATA sd)
    {
        RAY_PROFILE_FUNCTION();
        RAY_GFX_THROW_FAILED(s_pDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer));
    }
}