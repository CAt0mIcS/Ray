#include "pch.h"
#include "Sandbox.h"

#include <Ray/Ray.h>

#include "ExtensionLoader/ExtensionLoader.h"

namespace At0::Zeal
{
	Sandbox::Sandbox(const std::string_view commandLine)
		: Application(commandLine)
	{
		//RL_PROFILE_FUNCTION();
		
		m_MainWindow->InitRenderer3D();
		m_MainWindow->SetTitle("Zeal");
		m_MainWindow->Show();

		//Ray::Window* win = PushWindow(Ray::Window::Create("Win0"));
		//win->InitRenderer3D();
		//win->SetTitle("Win0");
		//win->Show();

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

		m_LayerLoader.Start("Editors", [this](Ray::Layer* layer) { PushLayer(layer); });

	}
	
	Sandbox::~Sandbox()
	{
		//RL_PROFILE_FUNCTION();
	}

	void Sandbox::OnEvent(Ray::Widget* receiver, Ray::Event& e)
	{
		//RL_PROFILE_FUNCTION();
	}

}


#include <Ray/EntryPoint.h>


#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RLogger.h>
#include <RayUtil/Exception.h>


///////////////////////////////////////////////////////////////////////////
////////// Called in EntryPoint.h /////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void At0::Ray::Awake(int argc, char** argv)
{
	using namespace At0;

	try
	{
		RL_LOG_BEGIN("../../Zeal.log", Log::LogLevel::Trace);

		RL_PROFILE_BEGIN_SESSION("Startup", "../../Profiling/Profile-Startup.json");
		Zeal::Sandbox* app = new Zeal::Sandbox("");
		RL_PROFILE_END_SESSION();

		RL_PROFILE_BEGIN_SESSION("Runtime", "../../Profiling/Profile-Runtime.json");
		int exitCode = app->Run();
		RL_PROFILE_END_SESSION();

		RL_PROFILE_BEGIN_SESSION("Shutdown", "../../Profiling/Profile-Shutdown.json");
		delete app;
		RL_PROFILE_END_SESSION();

		RL_LOG_END();

	}
	catch (At0::Ray::Exception& e)
	{
		Ray::ZMessageBox(e.GetType(), e.what(), Ray::ZMessageBox::Button::Ok, Ray::ZMessageBox::Icon::Error, Ray::ZMessageBox::Flags::DefaultDesktop);
		RL_LOG_CRITICAL("[Main] Exception occured: {0}", e.what());
	}
	catch (std::exception& e)
	{
		Ray::ZMessageBox("Standard Exception", e.what(), Ray::ZMessageBox::Button::Ok, Ray::ZMessageBox::Icon::Error, Ray::ZMessageBox::Flags::DefaultDesktop);
		RL_LOG_CRITICAL("[Main] Exception occured: {0}", e.what());
	}
	catch (...)
	{
		Ray::ZMessageBox("Unknown Exception", "An unknown exception occurred", Ray::ZMessageBox::Button::Ok, Ray::ZMessageBox::Icon::Error, Ray::ZMessageBox::Flags::DefaultDesktop);
		RL_LOG_CRITICAL("[Main] Unknown Exception occured");
	}
	RL_LOG_END();
}

