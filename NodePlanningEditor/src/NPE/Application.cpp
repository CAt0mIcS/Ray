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
		return MainWindow::ProcessMessage([this]() { OnFrame(); });
	}
	
	void Application::OnFrame()
	{
		
	}
}


