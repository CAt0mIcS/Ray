#include "pch.h"
#include "Application.h"

#include "Window/MainWindow.h"


namespace NPE
{
	Application::Application()
		: m_Window(800, 600, L"NodePlanningEditor")
	{
	}

	int Application::Run()
	{
		while (true)
		{
			if (const auto ecode = MainWindow::ProcessMessage())
			{
				return *ecode;
			}
			OnFrame();
		}

	}
	
	void Application::OnFrame()
	{
		
	}
}


