#include "pch.h"
#include "Sandbox.h"

#include <Ray/REntryPoint.h>

#include <../../RayDebug/include/RayDebug/RInstrumentor.h>


namespace At0::RayEditor
{
	Sandbox::Sandbox()
	{
		GetMainWindow().SetTitle("Ray-Editor");
		GetMainWindow().Show();
	}

	Sandbox::~Sandbox()
	{

	}
}


namespace At0
{
	int Ray::Awake(std::string commandLineArguments)
	{
		try
		{
			Ray::Log::Begin("../../Zeal.log", Log::LogLevel::Trace);

			RayEditor::Sandbox* app = new RayEditor::Sandbox();
			int exitCode = app->Run();
			delete app;

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
}