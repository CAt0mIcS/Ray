#include "rlpch.h"
#include "Application.h"

#include "Reyal/Layers/Layer.h"
#include "Reyal/Debug/ReyalLogger.h"


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

	void Application::PushLayer(_In_ Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	Application::~Application()
	{
		ZL_PROFILE_FUNCTION();
	}
}


