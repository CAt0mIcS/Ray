#include "Rpch.h"
#include "RApplication.h"

#include "Devices/RWindow.h"


namespace At0::Ray
{
	Application& Application::Get()
	{
		static Application instance;
		return instance;
	}

	int Application::Run()
	{
		while (Window::Get().Update())
		{

		}

		return 0;
	}
}