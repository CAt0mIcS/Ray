#include "RayRender/rlrpch.h"
#include "Topology.h"

#include <RayDebug/RInstrumentor.h>


namespace At0::Ray
{
    Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
        : m_Type(type)
    {
        RAY_PROFILE_FUNCTION();
    }

    void Topology::Bind()
    {
        s_pContext->IASetPrimitiveTopology(m_Type);
    }
}