#include "pch.h"
#include "ExtensionLoader.h"

#include <RlUtil/PlatformDetection.h>

#ifdef RL_PLATFORM_LINUX

namespace At0::Zeal
{
    void ExtensionLoader::Start(const std::string_view searchPath, const std::function<void(Reyal::Layer*)>& onNewLayer)
    {

    }
}

#endif