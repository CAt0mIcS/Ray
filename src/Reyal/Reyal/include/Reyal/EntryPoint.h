#ifndef RL_ENTRYPOINT_H
#define RL_ENTRYPOINT_H


#include "Reyal/Application.h"
#include <string>

extern void At0::Reyal::Awake(int argc, char** argv);


#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
namespace At0::Reyal
{
	static std::string WCharConv_(const std::wstring_view str)
	{
		auto size = ::WideCharToMultiByte(CP_UTF8, WC_COMPOSITECHECK, str.data(), -1, nullptr, 0, NULL, NULL);

		std::string buff;
		buff.resize(size);
		::WideCharToMultiByte(CP_UTF8, WC_COMPOSITECHECK, str.data(), -1, buff.data(), size, NULL, NULL);

		return buff;
	}
}

static char** ExpandArgs(PWSTR cmdLine, int* argc)
{
	return nullptr;
}

#include <sal.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int pCmdShow)
{
	using namespace At0;

	int argc;
	char** argv = ExpandArgs(pCmdLine, &argc);
	At0::Reyal::Awake(argc, argv);
}

#elif defined(__linux__)


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

	system("mkdir ./Profiling");
	RL_PROFILE_BEGIN_SESSION("Startup", "Profiling/Profile-Startup.json");
	Reyal::RendererAPI::SetAPI(Reyal::RendererAPI::API::OpenGL);
	Reyal::Application::Create(new Zeal::Sandbox(argv)); // TODO
	RL_PROFILE_END_SESSION();

	RL_PROFILE_BEGIN_SESSION("Runtime", "Profiling/Profile-Runtime.json");
	int exitCode = Reyal::Application::Get().Run();
	RL_PROFILE_END_SESSION();

	RL_PROFILE_BEGIN_SESSION("Shutdown", "Profiling/Profile-Shutdown.json");
	Reyal::Application::Destroy();
	RL_PROFILE_END_SESSION();

	RL_LOG_END();

	return exitCode;
}

#endif // _WIN32

#else
	//#error "Application Entry Point file included twice."
#endif // RL_ENTRYPOINT_H