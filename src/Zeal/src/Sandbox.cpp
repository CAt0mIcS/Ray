#include "pch.h"
#include "Sandbox.h"

#include <Ray/Ray.h>

#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RLogger.h>

#include "ExtensionLoader/ExtensionLoader.h"


namespace At0::Zeal
{
	static ExtensionLoader s_ExtensionLoader;

	Sandbox::Sandbox()
	{
		RAY_PROFILE_FUNCTION();

		Ray::Util::AllocateConsole();

		GetMainWindow().SetTitle("Zeal");
		GetMainWindow().Show();
		GetMainWindow().SetIcon("Resources/Icon.png");

		//Ray::Window& win = PushWindow(Ray::Window::Create("Win0", { 150, 150 }, { 960, 540 }));
		//win.SetTitle("Win0");
		//win.Show();

		//Ray::Window& win1 = PushWindow(Ray::Window::Create("Win1"));
		//win.SetTitle("Win1");
		//win.Show();

		//Ray::Window& win2 = PushWindow(Ray::Window::Create("Win2"));
		//win.SetTitle("Win2");
		//win.Show();

		//Ray::Window& win3 = PushWindow(Ray::Window::Create("Win3"));
		//win.SetTitle("Win3");
		//win.Show();

		//Ray::Window& win4 = PushWindow(Ray::Window::Create("Win4"));
		//win.SetTitle("Win4");
		//win.Show();

		s_ExtensionLoader.Start("Editors", [this](Ray::Layer* layer)
			{
				PushLayer(layer);
			}
		);

	}

	Sandbox::~Sandbox()
	{
		//RAY_PROFILE_FUNCTION();
	}

}


#include <Ray/REntryPoint.h>

// #include <RayUtil/RException.h>


///////////////////////////////////////////////////////////////////////////
////////// Called in EntryPoint.h /////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int At0::Ray::Awake(std::string commandLineArguments)
{
	using namespace At0;

	try
	{
		Ray::Log::Begin("../../Zeal.log", Log::LogLevel::Trace);

		Ray::Profile::BeginSession("Startup", "../../Profiling/Profile-Startup.json");
		Ray::RendererAPI::SetAPI(Ray::RendererAPI::OpenGL);
		Zeal::Sandbox* app = new Zeal::Sandbox();
		Ray::Profile::EndSession();

		Ray::Profile::BeginSession("Runtime", "../../Profiling/Profile-Runtime.json");
		int exitCode = app->Run();
		Ray::Profile::EndSession();

		Ray::Profile::BeginSession("Shutdown", "../../Profiling/Profile-Shutdown.json");
		delete app;
		Ray::Profile::EndSession();

		Ray::Log::End();
		return exitCode;
	}
	catch (At0::Ray::Exception& e)
	{
		Ray::RMessageBox(e.GetType(), e.what(), Ray::RMessageBox::Button::Ok, Ray::RMessageBox::Icon::Error, Ray::RMessageBox::Flags::DefaultDesktop);
		Ray::Log::Critical("[Awake] Exception occured: {0}", e.what());
	}
	catch (std::exception& e)
	{
		Ray::RMessageBox("Standard Exception", e.what(), Ray::RMessageBox::Button::Ok, Ray::RMessageBox::Icon::Error, Ray::RMessageBox::Flags::DefaultDesktop);
		Ray::Log::Critical("[Awake] Exception occured: {0}", e.what());
	}
	catch (...)
	{
		Ray::RMessageBox("Unknown Exception", "An unknown exception occurred", Ray::RMessageBox::Button::Ok, Ray::RMessageBox::Icon::Error, Ray::RMessageBox::Flags::DefaultDesktop);
		Ray::Log::Critical("[Awake] Unknown Exception occured");
	}
	Ray::Log::End();
	return -1;
}

