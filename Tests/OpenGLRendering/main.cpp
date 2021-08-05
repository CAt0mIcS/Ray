#include <Ray/Utils/RLogger.h>
#include <Ray/Utils/RException.h>
#include <signal.h>

#undef RAY_EXPORT
#define RAY_EXPORT
#include <Ray/Core/RRendererLoader.h>
#include <Ray/Core/RRendererLoader.cpp>

using namespace At0::Ray;
using namespace At0;


void SignalHandler(int signal)
{
	Log::Critical("Signal {0} received", signal);
	Log::Close();
}

int main()
{
	signal(SIGABRT, SignalHandler);
	signal(SIGILL, SignalHandler);
	signal(SIGINT, SignalHandler);

	Log::Open("Ray.log");
	Log::SetLogLevel(Violent::LogLevel::Debug);

	try
	{
		LoadRenderer(RendererAPI::OpenGL);
	}
	catch (Exception& e)
	{
		Log::Critical("{0}: {1}", e.GetType(), e.what());
	}
	catch (std::exception& e)
	{
		Log::Critical("Standard Exception: {0}", e.what());
	}
	catch (...)
	{
		Log::Critical("Unknown exception occured.");
	}
}
