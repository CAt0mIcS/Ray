#include "rlpch.h"
#include "Application.h"

#include "Reyal/Layers/Layer.h"
#include "Reyal/Debug/ReyalLogger.h"

#include "Reyal/Events/KeyboardEvent.h"


namespace Zeal::Reyal
{
	Application::Application()
		: m_MainWindow(L"Title", true), m_LayerStack{}
	{
		m_MainWindow.SetEventCallback([this](Widget* receiver, Event& e) { return OnEventReceived(receiver, e); });
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
	
	bool Application::OnEventReceived(_In_ Widget* receiver, _In_ Event& e)
	{
		ZL_PROFILE_FUNCTION();

		// Dispatch event to every layer
		for (auto* layer : m_LayerStack)
		{
			if (e.Handled)
				return true;
			layer->OnEvent(receiver, e);
		}

		// If no layer handled the event (e.g. WindowCloseEvent) then dispatch it to the Application
		if (!e.Handled)
			this->OnEvent(receiver, e);

		return e.Handled;
	}
}


