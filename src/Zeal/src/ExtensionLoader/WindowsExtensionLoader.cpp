#include "pch.h"
#include "ExtensionLoader.h"

#include <RlUtil/PlatformDetection.h>


#ifdef RL_PLATFORM_WINDOWS

#include <filesystem>
#include <Windows.h>
#include <functional>

#include <RlDebug/ReyalLogger.h>

#include "Reyal/Layers/Layer.h"


namespace At0::Zeal
{
	void ExtensionLoader::Start(const std::string_view searchPath, const std::function<void(Reyal::Layer*)>& onNewLayer)
	{
		using DirectoryIterator = std::filesystem::directory_iterator;
		const std::wstring ending = L".dll";
		using LayerCreateFunc = Reyal::Layer* (*)();

		for (const std::filesystem::directory_entry& dir : std::filesystem::recursive_directory_iterator(searchPath))
		{
			std::wstring path = dir.path().c_str();

			// Check if file is a .dll file
			if (path.compare(path.size() - ending.size(), ending.size(), ending) != 0) continue;

			// TODO: Separate folder for Editor extensions
			HMODULE lib = LoadLibraryW(path.c_str());
			if (!lib || lib == INVALID_HANDLE_VALUE)
			{
				RL_LOG_WARN("[ExtLoader] Was unable to load the library with path '{0}'", std::filesystem::absolute(path));
				continue;
			}

			LayerCreateFunc fn = (LayerCreateFunc)GetProcAddress(lib, "CreateLayer");
			if (!fn)
			{
				RL_LOG_WARN("[ExtLoader] Was unable to find 'CreateLayer' function in Module '{0}' with path '{1}'", lib, std::filesystem::absolute(path));
				continue;
			}

			onNewLayer(fn());
		}
	}
}


#endif