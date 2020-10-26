#include "pch.h"
#include "Sandbox.h"


//QUESTION: When entry point is included in Reyal.h
//#include <Reyal/Reyal.h>
#include <Reyal/EntryPoint.h>

#include <filesystem>

// TODO: Temporary
#if defined(_DEBUG) || defined(DEBUG)
	#if defined(_WIN64)
		#define ZL_OUT_FOLDER "Release-x64"
	#elif defined(_WIN32)
		#define ZL_OUT_FOLDER "Release-Win32"
	#endif
#elif defined(NDEBUG) || defined(_NDEBUG)
	#if defined(_WIN64)
		#define ZL_OUT_FOLDER "Release-x64"
	#elif defined(_WIN32)
		#define ZL_OUT_FOLDER "Release-Win32"
	#endif
#else
	#error "Unknown build configuration"
#endif

#define ZL_OUT_DIR "D:\\dev\\Cpp\\Projects\\NodePlanningEditor\\bin\\" ZL_OUT_FOLDER "\\Editors"


namespace At0
{
	Sandbox::Sandbox()
	{
		ZL_PROFILE_FUNCTION();

		m_MainWindow.SetTitle(L"Zeal");
		m_MainWindow.Show();

		// Test loading in layers
		using LayerCreateFunc = Reyal::Layer* (*)();
		const std::wstring ending = L".dll";

		for (const std::filesystem::directory_entry& dirEntry : std::filesystem::recursive_directory_iterator(ZL_OUT_DIR))
		{
			std::wstring_view path = dirEntry.path().native();

			if (path.size() < ending.size() || path.compare(path.size() - ending.size(), ending.size(), ending) != 0)
			{
				continue;
			}

			HMODULE hDll = LoadLibrary(path.data());
			if (!hDll || hDll == INVALID_HANDLE_VALUE)
			{
				DWORD err = GetLastError();
				__debugbreak();
			}

			LayerCreateFunc layerCreateFunc = (LayerCreateFunc)GetProcAddress(hDll, "CreateLayer");
			if (!layerCreateFunc)
			{
				DWORD err = GetLastError();
				__debugbreak();
			}

			PushLayer(layerCreateFunc());
		}

	}
	
	Sandbox::~Sandbox()
	{
		ZL_PROFILE_FUNCTION();
	}

	void Sandbox::OnEvent(Reyal::Widget* receiver, Reyal::Event& e)
	{
		ZL_PROFILE_FUNCTION();

	}

}
