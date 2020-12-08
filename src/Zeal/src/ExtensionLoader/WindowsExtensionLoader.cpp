#include "pch.h"
#include "ExtensionLoader.h"


#ifdef _WIN32

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
		const std::string ending = ".dll";
		using LayerCreateFunc = Reyal::Layer* (*)();

		RL_LOG_WARN("[ExtLoader] Searching path '{0}' for Layers", std::filesystem::absolute(searchPath));
		for (const std::filesystem::directory_entry& dir : std::filesystem::recursive_directory_iterator(searchPath))
		{
			// Check if file is a .dll file
			if (dir.path().string().compare(dir.path().string().size() - ending.size(), ending.size(), ending) != 0) continue;

			HMODULE lib = LoadLibraryA(dir.path().string().c_str());
			if (!lib || lib == INVALID_HANDLE_VALUE)
			{
				RL_LOG_WARN("[ExtLoader] Was unable to load the library with path '{0}'", std::filesystem::absolute(dir.path().string()));
				continue;
			}

			LayerCreateFunc fn = (LayerCreateFunc)GetProcAddress(lib, "CreateLayer");
			if (!fn)
			{
				RL_LOG_WARN("[ExtLoader] Was unable to find 'CreateLayer' function in Module '{0}' with path '{1}'", lib, std::filesystem::absolute(dir.path().string()));
				FreeLibrary(lib);
				continue;
			}

			m_LibHandles.push_back(lib);
			onNewLayer(fn());
		}
	}

	ExtensionLoader::~ExtensionLoader()
	{
		for (void* lib : m_LibHandles)
		{
			FreeLibrary((HMODULE)lib);
		}
	}
}


#endif