#include "Ray/rpch.h"
#include "Ray/Layers/RLayer.h"

#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RLogger.h>


namespace At0::Ray
{
    Layer::Layer(std::string_view name)
        : m_Name(name)
    {
        RAY_PROFILE_FUNCTION();

    }
    
    Layer::~Layer()
    {
        RAY_LOG_DEBUG("[Layer] '{0}' destroyed", m_Name);
    }
}

