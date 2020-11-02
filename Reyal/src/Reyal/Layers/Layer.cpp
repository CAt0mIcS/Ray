#include "rlpch.h"
#include "Layer.h"

#include "Reyal/Debug/Instrumentor.h"


namespace At0::Reyal
{
    Layer::Layer(const std::wstring_view name)
        : m_Name(name)
    {
        ZL_PROFILE_FUNCTION();

    }
}

