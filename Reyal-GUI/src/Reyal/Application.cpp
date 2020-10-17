#include "rlpch.h"
#include "Application.h"



namespace Zeal::Reyal
{
	Application::Application()
		: m_MainWindow(L"Title"), m_Window(L"FDs")
	{
		m_MainWindow.Show();
		m_Window.Show();
	}

	int Application::Run()
	{
		return Window::ProcessMessages();
	}

	Application::~Application()
	{

	}
}


