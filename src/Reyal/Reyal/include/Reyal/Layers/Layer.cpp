#include "rlpch.h"
#include "Reyal/Layers/Layer.h"

#include <RlDebug/Instrumentor.h>


namespace At0::Reyal
{
    Layer::Layer(const std::string_view name)
        : m_Name(name)
    {
        RL_PROFILE_FUNCTION();

    }
}

