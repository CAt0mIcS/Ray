#pragma once


#include "rlpch.h"

#include "Reyal/Application.h"
#include "Reyal/Debug/ReyalLogger.h"
#include "Reyal/Exception.h"

#include <signal.h>


/// <summary>
/// Handles any unexpected signals, logs the signal and closes the logger
/// </summary>
/// <param name="signum">Is the received signal</param>
void SignalHandler(int signum)
{
	ZL_LOG_CRITICAL("Signal '{0}' received, terminating program", signum);

	// Note: We do not need to do this as the destructor does it automatically, 
	// but it's good to have it here
	ZL_LOG_END();

	// We need to do this tough
	ZL_PROFILE_END_SESSION();

	exit(signum);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int pCmdShow)
{
	using namespace Zeal;

	ZL_LOG_BEGIN("Zeal.log", Zeal::Log::LogLevel::Trace);

	/// <summary>
	/// QUESTION:
	///		Should I handle signals like this or is there a better way? Like not doing it at all?
	/// </summary>
	signal(SIGABRT, SignalHandler);
	signal(SIGFPE, SignalHandler);
	signal(SIGILL, SignalHandler);
	signal(SIGINT, SignalHandler);
	signal(SIGSEGV, SignalHandler);
	signal(SIGTERM, SignalHandler);
	
#ifdef _DEBUG
	{
		AllocConsole();
		FILE* file;
		freopen_s(&file, "CONOUT$", "w", stdout);
	}
#endif

	try
	{
		ZL_PROFILE_BEGIN_SESSION("Startup", "Profile-Startup.json");
		Reyal::CreateApplication();
		ZL_PROFILE_END_SESSION();

		ZL_PROFILE_BEGIN_SESSION("Runtime", "Profile-Runtime.json");
		int exitCode = Reyal::Application::Get()->Run();
		ZL_PROFILE_END_SESSION();

		ZL_PROFILE_BEGIN_SESSION("Shutdown", "Profile-Shutdown.json");
		Reyal::Application::Destroy();
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

