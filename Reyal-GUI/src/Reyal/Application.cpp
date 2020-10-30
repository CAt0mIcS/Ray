#include "rlpch.h"
#include "Application.h"

#include "Reyal/Layers/Layer.h"
#include "Reyal/Debug/ReyalLogger.h"

#include "Reyal/Events/KeyboardEvent.h"


namespace At0::Reyal
{
	Ref<Application> Application::s_Instance = nullptr;

	Application::Application()
		: m_MainWindow(L"MainWindow", nullptr, true), m_LayerStack{}, m_Running(true)
	{
		m_MainWindow.SetImmediateEventHandler([this](Widget* receiver, Event& e) { return OnImmediateEvent(receiver, e); });
	}

	void Application::Create(_In_ Application* app)
	{
		s_Instance.reset(app);
	}

	void Application::Destroy()
	{
		s_Instance.reset();
	}

	int Application::Run()
	{
		/// <summary>
		/// QUESTION: Threads in a large program (thread-pools?)
		/// </summary>
		auto dispatchEvents = [this]()
		{
			auto& queue = m_MainWindow.GetEventQueue();
			while (m_Running)
			{
				while (!queue.Empty())
				{
					EventMessage eMsg = queue.PopFront();
					OnEventReceived(eMsg.receiver, std::move(eMsg.e));
				}
			}
		};
		m_ThreadPool.AddTask(dispatchEvents);

		int exitCode = 0;
		while (!m_MainWindow.ShouldClose(&exitCode))
		{
			for (auto* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}
		}

		m_Running = false;
		return exitCode;
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

	bool Application::OnImmediateEvent(_In_ Widget* receiver, Event& e)
	{
		ZL_PROFILE_FUNCTION();

		// Wait for queue to be empty (TODO)
		while (!m_MainWindow.GetEventQueue().Empty());

		for (auto* layer : m_LayerStack)
		{
			switch (e.GetType())
			{
			case EventType::WindowCloseEvent:			return layer->OnWindowClose(receiver, (WindowCloseEvent&)e);
			default:
				assert(false && "Unimplemented event");
			}
		}
		
		return false;
	}
	
	void Application::DispatchEvent(_In_ Layer* layer, _In_ Widget* receiver, Event& e)
	{
		ZL_PROFILE_FUNCTION();

		RL_EXPECTS(e.GetType() != EventType::INVALID);

		//TODO: Implement receiver
		switch (e.GetType())
		{
		case EventType::MouseButtonPressedEvent:	layer->OnMousePress(receiver, (MouseButtonPressedEvent&)e); break;
		case EventType::MouseButtonReleasedEvent:	layer->OnMouseRelease(receiver, (MouseButtonReleasedEvent&)e); break;
		case EventType::MouseWheelUpEvent:			layer->OnMouseWheelUp(receiver, (MouseWheelUpEvent&)e); break;
		case EventType::MouseWheelDownEvent:		layer->OnMouseWheelDown(receiver, (MouseWheelDownEvent&)e); break;
		case EventType::MouseMoveEvent:				layer->OnMouseMove(receiver, (MouseMoveEvent&)e); break;
		case EventType::HoverEnterEvent:			layer->OnHoverEnter(receiver, (HoverEnterEvent&)e); break;
		case EventType::HoverLeaveEvent:			layer->OnHoverLeave(receiver, (HoverLeaveEvent&)e); break;
		case EventType::KeyPressedEvent:			layer->OnKeyPress(receiver, (KeyPressedEvent&)e); break;
		case EventType::KeyReleasedEvent:			layer->OnKeyRelease(receiver, (KeyReleasedEvent&)e); break;
		case EventType::CharEvent:					layer->OnChar(receiver, (CharEvent&)e); break;
		case EventType::WindowResizeEvent:			layer->OnResize(receiver, (WindowResizeEvent&)e); break;
		case EventType::WindowMoveEvent:			layer->OnWindowMove(receiver, (WindowMoveEvent&)e); break;
		case EventType::PaintEvent:					layer->OnPaint(receiver, (PaintEvent&)e); break;
		//case EventType::SetCursorEvent:			layer->OnSetCursor(receiver, (SetCursorEvent&)e); break;
		default:
			RL_ASSERT(false, "Unimplemented Event");
		}
	}
}


