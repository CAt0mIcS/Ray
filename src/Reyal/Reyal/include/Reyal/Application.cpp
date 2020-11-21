#include "rlpch.h"
#include "Reyal/Application.h"

#include "Reyal/Layers/Layer.h"

#include "Reyal/Events/KeyboardEvent.h"
#include "Reyal/Events/MouseEvent.h"
#include "Reyal/Events/ApplicationEvent.h"

// Include Renderer because of incomplete type in Window
#include <RlRender/Renderer3D.h>

#include <RlDebug/RlAssert.h>
#include <RlDebug/Instrumentor.h>
#include <RlDebug/ReyalLogger.h>


namespace At0::Reyal
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_LayerStack{}
	{
		m_MainWindow = PushWindow(Window::Create("MainWindow", nullptr));
		Window::SetImmediateEventHandler([this](Widget* receiver, Event& e) { return OnImmediateEvent(receiver, e); });
	}

	void Application::Create(Application* app)
	{
		s_Instance = app;
	}

	void Application::Destroy()
	{
		delete s_Instance;
	}

	int Application::Run()
	{
		// -----------------------------------------------------------------------------------------
		// Main Application Loop
		while (m_MainWindow->IsOpen())
		{
			// -------------------------------------------------------------------------------------
			// Pop invalid windows and update valid ones
			for (uint32_t i = 0; i < m_WindowStack.Size(); ++i)
			{
				if(m_WindowStack[i]->IsOpen())
					m_WindowStack[i]->OnUpdate();
				else
				{
					m_WindowStack.PopIndexed(i);
					--i;
				}

			}

			// -------------------------------------------------------------------------------------
			// Update Layers
			for (auto* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			//TODO: Change to something more appropriate (CPU Usage too high without it)
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		Cleanup();
		return 0;
	}

	Layer* Application::PushLayer(Layer* layer)
	{
		RL_PROFILE_FUNCTION();

		m_LayerStack.PushBack(layer);
		return layer;
	}

	Window* Application::PushWindow(Ref<Window> window)
	{
		RL_PROFILE_FUNCTION();

		m_WindowStack.PushBack(window);

		// Start thread for the pushed Window
		m_EventLoopThreads.emplace_back([this, window]()
			{
				auto& queue = window->GetEventQueue();
				// Wait for the first event to come
				queue.WaitFor([&queue]() { return !queue.Empty(); });

				while (window->IsOpen())
				{
					EventMessage eMsg = queue.PopFront();
					// Dispatch the event to the layers
					OnEventReceived(eMsg.receiver, std::move(eMsg.e));
					// Wait for new events to come in
					queue.WaitFor([&queue]() { return !queue.Empty(); });
				}
			}
		);

		return window.get();
	}

	Application::~Application()
	{
		RL_PROFILE_FUNCTION();
	}
	
	void Application::OnEventReceived(Widget* receiver, Scope<Event>&& e)
	{
		RL_PROFILE_FUNCTION();

		// Dispatch event to every layer
		for (auto* layer : m_LayerStack)
		{
			DispatchEvent(layer, receiver, *e);
		}
	}

	bool Application::OnImmediateEvent(Widget* receiver, Event& e)
	{
		RL_PROFILE_FUNCTION();
		RL_EXPECTS(e.GetType() <= EventType::LAST && e.GetType() >= EventType::FIRST);

		for (auto* layer : m_LayerStack)
		{
			switch (e.GetType())
			{
			case EventType::WindowCloseEvent:
			{
				// Notify that the window has been closed
				m_MainWindow->GetEventQueue().GetWaiter().notify_all();
				return layer->OnWindowClose(receiver, (WindowCloseEvent&)e);
			}
			default:
				RL_ASSERT(false, "Unimplemented Event");
			}
		}
		return false;
	}
	
	void Application::DispatchEvent(Layer* layer, Widget* receiver, Event& e)
	{
		RL_PROFILE_FUNCTION();

		RL_EXPECTS(e.GetType() <= EventType::LAST && e.GetType() >= EventType::FIRST);
		RL_EXPECTS(layer != nullptr);

		switch (e.GetType())
		{
		case EventType::MouseButtonPressedEvent:	layer->OnMousePress(receiver, (MouseButtonPressedEvent&)e); break;
		case EventType::MouseButtonReleasedEvent:	layer->OnMouseRelease(receiver, (MouseButtonReleasedEvent&)e); break;
		case EventType::MouseWheelUpEvent:			layer->OnMouseWheelUp(receiver, (MouseWheelUpEvent&)e); break;
		case EventType::MouseWheelDownEvent:		layer->OnMouseWheelDown(receiver, (MouseWheelDownEvent&)e); break;
		case EventType::MouseWheelLeftEvent:		layer->OnMouseWheelLeft(receiver, (MouseWheelLeftEvent&)e); break;
		case EventType::MouseWheelRightEvent:		layer->OnMouseWheelRight(receiver, (MouseWheelRightEvent&)e); break;
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
			break;
		}
	}
	
	void Application::Cleanup()
	{
		// Close all Windows when the MainWindow was closed
		for (auto& window : m_WindowStack)
		{
			window->Close();
		}

		for (std::thread& thread : m_EventLoopThreads)
		{
			if (thread.joinable())
				thread.join();
		}

		// Need this here to stop threads from waiting until the static ThreadPool object is destroyed
		ThreadPool::Get().Shutdown();
	}
}


