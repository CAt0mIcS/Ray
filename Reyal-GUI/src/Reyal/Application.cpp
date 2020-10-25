#include "rlpch.h"
#include "Application.h"

#include "Reyal/Layers/Layer.h"
#include "Reyal/Debug/ReyalLogger.h"

#include "Reyal/Events/KeyboardEvent.h"


namespace Zeal::Reyal
{
	std::shared_ptr<Application> Application::s_Instance = nullptr;

	Application::Application()
		: m_MainWindow(L"MainWindow", nullptr, true), m_LayerStack{}
	{
	}

	void Application::Create(Application* app)
	{
		s_Instance.reset(app);
	}

	void Application::Destroy()
	{
		s_Instance.reset();
	}

	static std::thread s_EventThread;
	int Application::Run()
	{
		while (!m_MainWindow.ShouldClose())
		{
			for (auto* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			if (s_EventThread.joinable())
				s_EventThread.join();


			// TODO: Strip in better builds
			for (EventMessage& eMsg : m_MainWindow.GetEventQueue())
			{
				ZL_LOG_DEBUG(eMsg.e->ToString());
			}

			s_EventThread = std::thread([this]()
				{
					if (!m_MainWindow.GetEventQueue().Empty())
					{
						EventMessage eMsg = m_MainWindow.GetEventQueue().PopFront();
						OnEventReceived(eMsg.receiver, std::move(eMsg.e));
					}
				}
			);

		}

		return m_MainWindow.GetExitCode();
	}

	void Application::PushLayer(_In_ Layer* layer)
	{
		ZL_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
	}

	Application::~Application()
	{
		ZL_PROFILE_FUNCTION();
	}
	
	void Application::OnEventReceived(_In_ Widget* receiver, Scope<Event>&& e)
	{
		ZL_PROFILE_FUNCTION();

		// Dispatch event to every layer
		for (auto* layer : m_LayerStack)
		{
			if (e->Handled)
				break;
			DispatchEvent(layer, receiver, *e);
		}
	}
	
	void Application::DispatchEvent(_In_ Layer* layer, _In_ Widget* receiver, Event& e)
	{
		ZL_PROFILE_FUNCTION();

		//TODO: Implement receiver
		switch (e.GetType())
		{
		case EventType::MouseButtonPressedEvent:	layer->OnMousePress(receiver, e); break;
		case EventType::MouseButtonReleasedEvent:	layer->OnMouseRelease(receiver, e); break;
		case EventType::MouseWheelUpEvent:			layer->OnMouseWheelUp(receiver, e); break;
		case EventType::MouseWheelDownEvent:		layer->OnMouseWheelDown(receiver, e); break;
		case EventType::MouseMoveEvent:				layer->OnMouseMove(receiver, e); break;
		case EventType::HoverEnterEvent:			layer->OnHoverEnter(receiver, e); break;
		case EventType::HoverLeaveEvent:			layer->OnHoverLeave(receiver, e); break;
		case EventType::KeyPressedEvent:			layer->OnKeyPress(receiver, e); break;
		case EventType::CharEvent:					layer->OnChar(receiver, e); break;
		case EventType::KeyReleasedEvent:			layer->OnKeyRelease(receiver, e); break;
		case EventType::WindowResizeEvent:			layer->OnResize(receiver, e); break;
		//case EventType::WindowCloseEvent:			layer->OnWindowClose(receiver, e); break;
		case EventType::WindowMoveEvent:			layer->OnWindowMove(receiver, e); break;
		case EventType::PaintEvent:					layer->OnPaint(receiver, e); break;
		//case EventType::SetCursorEvent:			layer->OnSetCursor(receiver, e); break;
		default:
			assert(false, "Unimplemented event");
		}
	}
}


