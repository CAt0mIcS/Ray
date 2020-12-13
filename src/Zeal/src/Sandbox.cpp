#include "pch.h"
#include "Sandbox.h"

#include <Ray/Ray.h>

#include "ExtensionLoader/ExtensionLoader.h"

namespace At0::Zeal
{
	Sandbox::Sandbox(const std::string_view commandLine)
		: Application(commandLine)
	{
		//RAY_PROFILE_FUNCTION();

		m_MainWindow->InitRenderer3D();
		m_MainWindow->SetTitle("Zeal");
		m_MainWindow->Show();

		Ray::Window* win = PushWindow(Ray::Window::Create("Win0", { 150, 150 }, { 960, 540 }));
		//win->InitRenderer3D();
		win->SetTitle("Win0");
		win->Show();

		//Ray::Window* win1 = PushWindow(Ray::Window::Create("Win1"));
		//win1->InitRenderer3D();
		//win1->SetTitle("Win1");
		//win1->Show();

		//Ray::Window* win2 = PushWindow(Ray::Window::Create("Win2"));
		//win2->InitRenderer3D();
		//win2->SetTitle("Win2");
		//win2->Show();

		//Ray::Window* win3 = PushWindow(Ray::Window::Create("Win3"));
		//win3->InitRenderer3D();
		//win3->SetTitle("Win3");
		//win3->Show();

		//Ray::Window* win4 = PushWindow(Ray::Window::Create("Win4"));
		//win4->InitRenderer3D();
		//win4->SetTitle("Win4");
		//win4->Show();

		m_LayerLoader.Start("Editors", [this](Ray::Layer* layer) 
			{
				std::shared_ptr<Ray::Layer> ptr(layer);
				PushLayer(std::move(ptr));
			}
		);

	}
	
	Sandbox::~Sandbox()
	{
		//RAY_PROFILE_FUNCTION();
	}

	void Sandbox::OnEvent(Ray::Widget* receiver, Ray::Event& e)
	{
		//RAY_PROFILE_FUNCTION();
	}

}


#include <Ray/REntryPoint.h>


#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RLogger.h>
#include <RayUtil/Exception.h>
#include <RayRender/RendererAPI.h>


///////////////////////////////////////////////////////////////////////////
////////// Called in EntryPoint.h /////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int At0::Ray::Awake(int argc, char** argv)
{
	using namespace At0;

	try
	{
		RAY_LOG_BEGIN("../../Zeal.log", Log::LogLevel::Trace);

		RAY_PROFILE_BEGIN_SESSION("Startup", "../../Profiling/Profile-Startup.json");
		Ray::RendererAPI::SetAPI(Ray::RendererAPI::D3D11);
		Zeal::Sandbox* app = new Zeal::Sandbox("");
		RAY_PROFILE_END_SESSION();

		RAY_PROFILE_BEGIN_SESSION("Runtime", "../../Profiling/Profile-Runtime.json");
		int exitCode = app->Run();
		RAY_PROFILE_END_SESSION();

		RAY_PROFILE_BEGIN_SESSION("Shutdown", "../../Profiling/Profile-Shutdown.json");
		delete app;
		RAY_PROFILE_END_SESSION();

		RAY_LOG_END();
		return exitCode;
	}
	catch (At0::Ray::Exception& e)
	{
		Ray::RMessageBox(e.GetType(), e.what(), Ray::RMessageBox::Button::Ok, Ray::RMessageBox::Icon::Error, Ray::RMessageBox::Flags::DefaultDesktop);
		RAY_LOG_CRITICAL("[Main] Exception occured: {0}", e.what());
	}
	catch (std::exception& e)
	{
		Ray::RMessageBox("Standard Exception", e.what(), Ray::RMessageBox::Button::Ok, Ray::RMessageBox::Icon::Error, Ray::RMessageBox::Flags::DefaultDesktop);
		RAY_LOG_CRITICAL("[Main] Exception occured: {0}", e.what());
	}
	catch (...)
	{
		Ray::RMessageBox("Unknown Exception", "An unknown exception occurred", Ray::RMessageBox::Button::Ok, Ray::RMessageBox::Icon::Error, Ray::RMessageBox::Flags::DefaultDesktop);
		RAY_LOG_CRITICAL("[Main] Unknown Exception occured");
	}
	RAY_LOG_END();
	return -1;
}

