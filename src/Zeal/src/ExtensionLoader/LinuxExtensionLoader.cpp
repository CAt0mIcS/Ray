#include "pch.h"
#include "ExtensionLoader.h"

#include <RlUtil/PlatformDetection.h>

#ifdef RL_PLATFORM_LINUX

#include <RlDebug/ReyalLogger.h>

#include <filesystem>
#include <dlfcn.h>

namespace At0::Zeal
{
    void ExtensionLoader::Start(const std::string_view searchPath, const std::function<void(Reyal::Layer*)>& onNewLayer)
    {
        const std::string ending = ".so";
        using LayerCreateFunc = Reyal::Layer* (*)();

        for (const std::filesystem::directory_entry& dir : std::filesystem::recursive_directory_iterator(searchPath))
        {
            // Check if file is a .so file
            if (dir.path().string().compare(dir.path().string().size() - ending.size(), ending.size(), ending) != 0) continue;

            void* handle = dlopen(dir.path().string().c_str(), RTLD_GLOBAL);
            if (!handle)
            {
                RL_LOG_WARN("[ExtLoader] Was unable to load the library with path '{0}'", std::filesystem::absolute(dir.path().string()));
                continue;
            }
            
            LayerCreateFunc layerCreateFunc = (LayerCreateFunc)dlsym(handle, "CreateLayer");
            if (!layerCreateFunc)
            {
                RL_LOG_WARN("[ExtLoader] Was unable to find 'CreateLayer' function in Module '{0}' with path '{1}'", handle, std::filesystem::absolute(dir.path().string()));
                continue;
            }

            onNewLayer(layerCreateFunc());
        }
    }
}

#endif