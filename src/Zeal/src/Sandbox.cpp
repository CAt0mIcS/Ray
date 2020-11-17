#include "pch.h"
#include "Sandbox.h"

#include <Reyal/Reyal.h>

#include "ExtensionLoader/ExtensionLoader.h"


namespace At0::Zeal
{
	Sandbox::Sandbox()
	{
		//RL_PROFILE_FUNCTION();
		
		 m_MainWindow->InitRenderer3D();
		 m_MainWindow->SetTitle("Zeal");
		 m_MainWindow->Show();

		ExtensionLoader loader;
#ifdef NDEBUG
	#ifdef _MSC_VER
		loader.Start("../../bin/Release/Editors", [this](Reyal::Layer* layer) { PushLayer(layer); });
	#else
		loader.Start("./bin/Release/Editors", [this](Reyal::Layer* layer) { PushLayer(layer); });
	#endif
#else
	#ifdef _MSC_VER
		loader.Start("../../bin/Debug/Editors", [this](Reyal::Layer* layer) { PushLayer(layer); });
	#else
		loader.Start("./bin/Debug/Editors", [this](Reyal::Layer* layer) { PushLayer(layer); });
	#endif
#endif
	}
	
	Sandbox::~Sandbox()
	{
		//RL_PROFILE_FUNCTION();
	}

	void Sandbox::OnEvent(Reyal::Widget* receiver, Reyal::Event& e)
	{
		//RL_PROFILE_FUNCTION();
	}

}



/////////////////////////////////////////////////////////////////////////////////////////////////////
////////// Entry Point (TODO) ///////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include <RlDebug/ReyalLogger.h>
#include <RlDebug/Instrumentor.h>
#include <RlUtil/Exception.h>

#include <signal.h>


/// <summary>
/// Handles any unexpected signals, logs the signal and closes the logger
/// </summary>
/// <param name="signum">Is the received signal</param>
void SignalHandler(int signum)
{
	RL_LOG_CRITICAL("Signal '{0}' received, terminating program", signum);

	// Note: We do not need to do this as the destructor does it automatically, 
	// but it's good to have it here
	RL_LOG_END();

	// We need to do this though
	RL_PROFILE_END_SESSION();

	exit(signum);
}

#ifdef _WIN32

#include <sal.h>


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int pCmdShow)
{
	using namespace At0;

	RL_LOG_BEGIN("../../Zeal.log", Log::LogLevel::Trace);

	/// <summary>
	/// TODO:
	///		Util::SetSignals (TODO)
	/// </summary>
	signal(SIGABRT, SignalHandler);
	signal(SIGFPE, SignalHandler);
	signal(SIGILL, SignalHandler);
	signal(SIGINT, SignalHandler);
	signal(SIGSEGV, SignalHandler);
	signal(SIGTERM, SignalHandler);

	//TODO: Awake function (maybe)?

	#ifndef NDEBUG
	{
		AllocConsole();
		FILE* file;
		freopen_s(&file, "CONOUT$", "w", stdout);
	}
	#endif

	try
	{
		RL_PROFILE_BEGIN_SESSION("Startup", "Profiling/Profile-Startup.json");
		Reyal::CreateApplication();
		RL_PROFILE_END_SESSION();

		RL_PROFILE_BEGIN_SESSION("Runtime", "Profiling/Profile-Runtime.json");
		int exitCode = Reyal::Application::Get().Run();
		RL_PROFILE_END_SESSION();

		RL_PROFILE_BEGIN_SESSION("Shutdown", "Profiling/Profile-Shutdown.json");
		Reyal::Application::Destroy();
		RL_PROFILE_END_SESSION();

#ifndef NDEBUG
		FreeConsole();
#endif
		RL_LOG_END();
		return exitCode;
	}
	// TODO: Custom Window for errors
	catch (At0::Reyal::Exception& e)
	{
		MessageBoxA(NULL, e.what(), e.GetType(), MB_OK | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
	}
	catch (std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Standard Exception", MB_OK | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
	}
	catch (...)
	{
		MessageBoxA(NULL, "An unknown exception occurred", "Unknown Exception", MB_OK | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
	}

	return -1;
}

#elif defined(__linux__)


#include <signal.h>

int main(int argc, char** argv)
{
	using namespace At0;
	RL_LOG_BEGIN("Zeal.log", Log::LogLevel::Trace);

	signal(SIGABRT, SignalHandler);
	signal(SIGFPE, SignalHandler);
	signal(SIGILL, SignalHandler);
	signal(SIGINT, SignalHandler);
	signal(SIGSEGV, SignalHandler);
	signal(SIGTERM, SignalHandler);


	Reyal::CreateApplication();
	Reyal::Application::Get().Run();
	Reyal::Application::Destroy();
	
	RL_LOG_END();
}

#endif
