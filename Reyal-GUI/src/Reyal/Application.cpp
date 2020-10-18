#include "rlpch.h"
#include "Application.h"

#include "Layer.h"



namespace Zeal::Reyal
{
	Application::Application()
		: m_MainWindow(L"Title", true), m_LayerStack{}
	{
	}

	int Application::Run()
	{
		while (!m_MainWindow.ShouldClose())
		{
			for (auto* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}
		}

		return m_MainWindow.GetExitCode();
	}

	Application::~Application()
	{
		ZL_PROFILE_FUNCTION();
	}
}


