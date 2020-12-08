#include "pch.h"
#include "ExtensionLoader.h"

#ifdef __linux__

#include <RlDebug/ReyalLogger.h>

#include <filesystem>
#include <dlfcn.h>

namespace At0::Zeal
{
    void ExtensionLoader::Start(const std::string_view searchPath, const std::function<void(Reyal::Layer*)>& onNewLayer)
    {
        const std::string ending = ".so";
        using LayerCreateFunc = Reyal::Layer* (*)();
        RL_LOG_DEBUG("[ExtLoader] Startup");

        for (const std::filesystem::directory_entry& dir : std::filesystem::recursive_directory_iterator(searchPath))
        {
            RL_LOG_DEBUG("[ExtLoader] Checking file {0}", dir.path().string());

            // Check if file is a .so file
            if (dir.path().string().compare(dir.path().string().size() - ending.size(), ending.size(), ending) != 0) continue;

            void* handle = dlopen(dir.path().string().c_str(), RTLD_LAZY);
            if (!handle)
            {
                RL_LOG_WARN("[ExtLoader] Was unable to load the library with path '{0}'", std::filesystem::absolute(dir.path().string()));
                continue;
            }
            
            LayerCreateFunc layerCreateFunc = (LayerCreateFunc)dlsym(handle, "CreateLayer");
            if (!layerCreateFunc)
            {
                RL_LOG_WARN("[ExtLoader] Was unable to find 'CreateLayer' function in Module '{0}' with path '{1}'", handle, std::filesystem::absolute(dir.path().string()));
                dlclose(handle);
                continue;
            }

            m_LibHandles.push_back(handle);
            onNewLayer(layerCreateFunc());
        }
    }

    ExtensionLoader::~ExtensionLoader()
    {
        RL_ASSERT(false, "Missing implementation (Free loaded libraries)");
    }
}

#endif