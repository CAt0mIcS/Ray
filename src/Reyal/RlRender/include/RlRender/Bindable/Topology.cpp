#include "rlrpch.h"
#include "Topology.h"


namespace At0::Reyal
{
    Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
        : m_Type(type)
    {
    }

    void Topology::Bind()
    {
        s_pContext->IASetPrimitiveTopology(m_Type);
    }
}