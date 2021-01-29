#include "Rpch.h"
#include "RApplication.h"

#include "Devices/RWindow.h"
#include "Debug/RLogger.h"


namespace At0::Ray
{
	Application* Application::s_Instance = nullptr;

	Application& Application::Get()
	{
		RAY_MEXPECTS(s_Instance, "[Application::Get] Application not initialized.");
		return *s_Instance;
	}

	int Application::Run()
	{
		while (Window::Get().Update())
		{
		}

		return 0;
	}

	Application::Application()
	{
		RAY_MEXPECTS(!s_Instance, "[Application] ALready initialized.");
		s_Instance = this;
	}
}  // namespace At0::Ray
