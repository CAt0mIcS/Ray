#include "rlrpch.h"
#include "VertexBuffer.h"

#include <RlUtil/Exception.h>


namespace At0::Reyal
{
    void VertexBuffer::Bind()
    {
        uint32_t offset = 0;
        s_pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &m_Strides, &offset);
    }
    
    inline void VertexBuffer::CreateBuffer(const D3D11_BUFFER_DESC& bd, const D3D11_SUBRESOURCE_DATA sd)
    {
        RL_GFX_THROW_FAILED(s_pDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer));
    }
}