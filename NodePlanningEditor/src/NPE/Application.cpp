#include "pch.h"
#include "Application.h"

#include "Window/MainWindow.h"


namespace NPE
{
	void Application::Run()
	{
		MainWindow win;
		win.Show(SW_MAXIMIZE);
		win.EnterMessageLoop();
	}
}


