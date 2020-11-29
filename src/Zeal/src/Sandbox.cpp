#include "pch.h"
#include "Sandbox.h"

#include <Reyal/Reyal.h>

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

		 Reyal::Window* win = PushWindow(Reyal::Window::Create("Win0"));
		 win->InitRenderer3D();
		 win->SetTitle("Win0");
		 win->Show();

		 Reyal::Window* win1 = PushWindow(Reyal::Window::Create("Win1"));
		 win1->InitRenderer3D();
		 win1->SetTitle("Win1");
		 win1->Show();

		 Reyal::Window* win2 = PushWindow(Reyal::Window::Create("Win2"));
		 win2->InitRenderer3D();
		 win2->SetTitle("Win2");
		 win2->Show();

		 Reyal::Window* win3 = PushWindow(Reyal::Window::Create("Win3"));
		 win3->InitRenderer3D();
		 win3->SetTitle("Win3");
		 win3->Show();

		 Reyal::Window* win4 = PushWindow(Reyal::Window::Create("Win4"));
		 win4->InitRenderer3D();
		 win4->SetTitle("Win4");
		 win4->Show();

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


#include <Reyal/EntryPoint.h>


#include <RlDebug/Instrumentor.h>
#include <RlDebug/ReyalLogger.h>
#include <RlUtil/Exception.h>


///////////////////////////////////////////////////////////////////////////
////////// Called in EntryPoint.h /////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void At0::Reyal::Awake(int argc, char** argv)
{
	using namespace At0;

	try
	{
		RL_LOG_BEGIN("../../Zeal.log", Log::LogLevel::Trace);

		RL_PROFILE_BEGIN_SESSION("Startup", "../../Profiling/Profile-Startup.json");
		Reyal::Application::Create(new Zeal::Sandbox(""));
		RL_PROFILE_END_SESSION();

		RL_PROFILE_BEGIN_SESSION("Runtime", "../../Profiling/Profile-Runtime.json");
		int exitCode = Reyal::Application::Get().Run();
		RL_PROFILE_END_SESSION();

		RL_PROFILE_BEGIN_SESSION("Shutdown", "../../Profiling/Profile-Shutdown.json");
		Reyal::Application::Destroy();
		RL_PROFILE_END_SESSION();

		RL_LOG_END();

	}
	catch (At0::Reyal::Exception& e)
	{
		Reyal::ZMessageBox(e.GetType(), e.what(), Reyal::ZMessageBox::Button::Ok, Reyal::ZMessageBox::Icon::Error, Reyal::ZMessageBox::Flags::DefaultDesktop);
		RL_LOG_CRITICAL("[Main] Exception occured: {0}", e.what());
	}
	catch (std::exception& e)
	{
		Reyal::ZMessageBox("Standard Exception", e.what(), Reyal::ZMessageBox::Button::Ok, Reyal::ZMessageBox::Icon::Error, Reyal::ZMessageBox::Flags::DefaultDesktop);
		RL_LOG_CRITICAL("[Main] Exception occured: {0}", e.what());
	}
	catch (...)
	{
		Reyal::ZMessageBox("Unknown Exception", "An unknown exception occurred", Reyal::ZMessageBox::Button::Ok, Reyal::ZMessageBox::Icon::Error, Reyal::ZMessageBox::Flags::DefaultDesktop);
		RL_LOG_CRITICAL("[Main] Unknown Exception occured");
	}
	RL_LOG_END();
}

