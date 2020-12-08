#include "rlrpch.h"
#include "Topology.h"

#include <RlDebug/Instrumentor.h>


namespace At0::Reyal
{
    Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
        : m_Type(type)
    {
        RL_PROFILE_FUNCTION();
    }

    void Topology::Bind()
    {
        RL_PROFILE_FUNCTION();
        s_pContext->IASetPrimitiveTopology(m_Type);
    }
}