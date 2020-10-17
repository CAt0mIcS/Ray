#pragma once
#include "rlpch.h"

#include "Reyal/Exception.h"

#include <Instrumentor/InstrumentationTools.h>
#include <ZealLogger/Log.h>



int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int pCmdShow)
{
	ZL_LOG_INIT("Zeal.log", Zeal::Log::LogLevel::Trace);

#ifdef _DEBUG
	{
		AllocConsole();
		FILE* pFile;
		freopen_s(&pFile, "CONOUT$", "w", stdout);
	}
#endif

	try
	{
		ZL_PROFILE_BEGIN_SESSION("Startup", "Profile-Startup.json");
		auto app = Zeal::Reyal::CreateApplication();
		ZL_PROFILE_END_SESSION();

		ZL_PROFILE_BEGIN_SESSION("Runtime", "Profile-Runtime.json");
		int exitCode = app->Run();
		ZL_PROFILE_END_SESSION();

		ZL_PROFILE_BEGIN_SESSION("Shutdown", "Profile-Shutdown.json");
		delete app;
		ZL_PROFILE_END_SESSION();

#ifdef _DEBUG
		FreeConsole();
#endif
		return exitCode;
	}
	// TODO: Custom Window for errors
	catch (Zeal::Reyal::Exception& e)
	{
		MessageBoxA(NULL, e.what(), e.GetType(), MB_OK | MB_ICONERROR);
	}
	catch (std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Standard Exception", MB_OK | MB_ICONERROR);
	}
	catch (...)
	{
		MessageBoxA(NULL, "An unknown exception occurred", "Unknown Exception", MB_OK | MB_ICONERROR);
	}

	return -1;
}

