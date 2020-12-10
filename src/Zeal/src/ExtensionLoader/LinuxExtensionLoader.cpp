#include "pch.h"
#include "ExtensionLoader.h"

#ifdef __linux__

#include <RayDebug/RLogger.h>

#include <filesystem>
#include <dlfcn.h>

namespace At0::Zeal
{
    void ExtensionLoader::Start(const std::string_view searchPath, const std::function<void(Ray::Layer*)>& onNewLayer)
    {
        const std::string ending = ".so";
        using LayerCreateFunc = Ray::Layer* (*)();
        RAY_LOG_DEBUG("[ExtLoader] Startup");

        RAY_LOG_WARN("[ExtLoader] Searching path '{0}' for Layers", std::filesystem::absolute(searchPath));
        for (const std::filesystem::directory_entry& dir : std::filesystem::recursive_directory_iterator(searchPath))
        {
            RAY_LOG_DEBUG("[ExtLoader] Checking file {0}", dir.path().string());

            // Check if file is a .so file
            if (dir.path().string().compare(dir.path().string().size() - ending.size(), ending.size(), ending) != 0) continue;

            void* handle = dlopen(dir.path().string().c_str(), RTLD_LAZY);
            if (!handle)
            {
                RAY_LOG_WARN("[ExtLoader] Was unable to load the library with path '{0}'", std::filesystem::absolute(dir.path().string()));
                continue;
            }
            
            LayerCreateFunc layerCreateFunc = (LayerCreateFunc)dlsym(handle, "CreateLayer");
            if (!layerCreateFunc)
            {
                RAY_LOG_WARN("[ExtLoader] Was unable to find 'CreateLayer' function in Module '{0}' with path '{1}'", handle, std::filesystem::absolute(dir.path().string()));
                dlclose(handle);
                continue;
            }

            m_LibHandles.push_back(handle);
            onNewLayer(layerCreateFunc());
        }
    }

    ExtensionLoader::~ExtensionLoader()
    {
        RAY_ASSERT(false, "Missing implementation (Free loaded libraries)");
    }
}

#endif