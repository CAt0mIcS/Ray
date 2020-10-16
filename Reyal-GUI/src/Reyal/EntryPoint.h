#pragma once
#include "rlpch.h"

#include <Instrumentor/InstrumentationTools.h>
#include <ZealLogger/Log.h>



int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int pCmdShow)
{
	ZL_LOG_INIT("Zeal.log", Zeal::Log::LogLevel::Trace);

	ZL_PROFILE_BEGIN_SESSION("Startup", "Profile-Startup.json");
	auto app = Zeal::Reyal::CreateApplication();
	ZL_PROFILE_END_SESSION();

	ZL_PROFILE_BEGIN_SESSION("Runtime", "Profile-Runtime.json");
	int exitCode = app->Run();
	ZL_PROFILE_END_SESSION();

	ZL_PROFILE_BEGIN_SESSION("Shutdown", "Profile-Shutdown.json");
	delete app;
	ZL_PROFILE_END_SESSION();

	return exitCode;
}

