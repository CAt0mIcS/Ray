#include <Ray/Utils/RLogger.h>
#include <Ray/Utils/RException.h>
#include <signal.h>

#include <Ray/Graphics/RGraphics.h>
#include <Ray/Core/RRendererLoader.h>

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

		Window::Create();
		Window::Get().Show();

		Graphics::Create(Graphics::NoDepthImage);

		const RendererInstance& instance = Graphics::Get().GetInstance();
		const PhysicalDevice& physicalDevice = Graphics::Get().GetPhysicalDevice();
		const Surface& surface = Graphics::Get().GetSurface();
		const LogicalDevice& device = Graphics::Get().GetDevice();

		while (Window::Get().Update())
		{
		}
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
