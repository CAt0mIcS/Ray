#include "rpch.h"
#include "Ray/Layers/Layer.h"

#include <RayDebug/RInstrumentor.h>


namespace At0::Ray
{
    Layer::Layer(const std::string_view name)
        : m_Name(name)
    {
        RAY_PROFILE_FUNCTION();

    }
}

