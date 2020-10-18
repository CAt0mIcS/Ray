#include "rlpch.h"
#include "Application.h"



namespace Zeal::Reyal
{
	Application::Application()
		: m_MainWindow(L"Title")
	{
	}

	int Application::Run()
	{
		return Window::ProcessMessages();
	}

	Application::~Application()
	{

	}
}


