#include "pch.h"
#include "Sandbox.h"


//QUESTION: When entry point is included in Reyal.h
//#include <Reyal/Reyal.h>
#include <Reyal/EntryPoint.h>

#include <filesystem>


namespace At0
{
	Sandbox::Sandbox()
	{
		ZL_PROFILE_FUNCTION();

		m_MainWindow.SetTitle(L"Zeal");
		m_MainWindow.Show();

		// Test loading in layers
		typedef Reyal::Layer* (*LayerCreateFunc)();
		const std::string dllDir = "D:\\dev\\Cpp\\Projects\\NodePlanningEditor\\bin\\Debug-Win32\\Editors";

		for (const std::filesystem::directory_entry& dirEntry : std::filesystem::recursive_directory_iterator(dllDir))
		{
			std::wstring path = dirEntry.path().c_str();
			std::wstring ending = L".dll";

			if (path.length() > ending.length())
			{
				if (path.compare(path.length() - ending.length(), ending.length(), ending) != 0)
					continue;
			}
			else
			{
				continue;
			}

			HMODULE hDll = LoadLibrary(path.c_str());
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
