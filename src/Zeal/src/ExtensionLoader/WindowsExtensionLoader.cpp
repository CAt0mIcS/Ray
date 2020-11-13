#include "pch.h"
#include "ExtensionLoader.h"

#include <RlUtil/PlatformDetection.h>


#ifdef RL_PLATFORM_WINDOWS

#include <filesystem>
#include <Windows.h>

#include "Reyal/Layers/Layer.h"


namespace At0::Zeal
{
	void ExtensionLoader::Start(const std::string_view searchPath)
	{
		using DirectoryIterator = std::filesystem::directory_iterator;
		const std::wstring ending = L".dll";
		using LayerCreateFunc = Reyal::Layer* (*)();

		for (const std::filesystem::directory_entry& dir : std::filesystem::recursive_directory_iterator(searchPath))
		{
			std::wstring path = dir.path().c_str();

			// Check if file is a .dll file
			if (path.compare(path.size() - ending.size(), ending.size(), ending) == 0) continue;

			HMODULE lib = LoadLibraryW(path.c_str());
			if (!lib || lib == INVALID_HANDLE_VALUE)
			{
				// TODO: Throw exception (Warning)
				return;
			}

			LayerCreateFunc fn = (LayerCreateFunc)GetProcAddress(lib, "CreateLayer");
			if (!fn)
			{
				// TODO: Throw exception (Warning)
				return;
			}
		}
	}
}


#endif