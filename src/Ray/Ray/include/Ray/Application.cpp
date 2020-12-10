#include "rpch.h"
#include "Ray/Application.h"

#include "Ray/Layers/Layer.h"

// Include Renderer because of incomplete type in Window
#include <RayRender/Renderer3D.h>

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RLogger.h>
#include <RayUtil/Exception.h>


namespace At0::Ray
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string_view commandLine)
		: m_LayerStack{}
	{
		if (s_Instance)
			RAY_ASSERT(false, "Another Application Instance already exists");
		s_Instance = this;

		StartupSetup();
		m_MainWindow = PushWindow(Window::Create("MainWindow"));
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

	#include <DirectXMath.h>
	int Application::Run()
	{
		// TODO: Only TEMPORARY
		RECT rc;
		GetClientRect((HWND)m_MainWindow->GetNativeWindow(), &rc);
		m_MainWindow->GetRenderer3D()->SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, (float)rc.bottom / (float)rc.right, 0.5f, 40.0f));


		auto lastFrameTime = std::chrono::high_resolution_clock::now();
		
		// -----------------------------------------------------------------------------------------
		// Main Application Loop
		while (m_MainWindow->IsOpen())
		{
			// -------------------------------------------------------------------------------------
			// Setting timestep
			auto tNow = std::chrono::high_resolution_clock::now();
			Timestep timestep = ((float)(tNow - lastFrameTime).count()) / 1000.0f / 1000.0f / 1000.0f;
			lastFrameTime = std::move(tNow);
			
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
				layer->OnUpdate(timestep);
			}

			//CPU Usage too high without it (not ideal)
			std::this_thread::sleep_for(std::chrono::nanoseconds(1500));
		}

		Cleanup();
		return 0;
	}

	Layer* Application::PushLayer(Layer* layer)
	{
		RAY_PROFILE_FUNCTION();

		m_LayerStack.PushBack(layer);
		return layer;
	}

	Window* Application::PushWindow(Ref<Window> window)
	{
		RAY_PROFILE_FUNCTION();

		m_WindowStack.PushBack(window);

		//// Start thread for the pushed Window
		//m_EventLoopThreads.emplace_back([this, window]()
		//	{
		//		auto& queue = window->GetEventQueue();
		//		// Wait for the first event to come
		//		queue.WaitFor([&queue]() { return !queue.Empty(); });

		//		while (window->IsOpen())
		//		{
		//			EventMessage eMsg = queue.PopFront();
		//			// Dispatch the event to the layers
		//			OnEventReceived(eMsg.receiver, std::move(eMsg.e));
		//			// Wait for new events to come in
		//			queue.WaitFor([&queue]() { return !queue.Empty(); });
		//		}
		//	}
		//);

		return window.get();
	}

	Application::~Application()
	{
		RAY_PROFILE_FUNCTION();
	}
	
	void Application::OnEventReceived(Widget* receiver, Scope<Event>&& e)
	{
		RAY_PROFILE_FUNCTION();

		// Dispatch event to every layer
		for (auto* layer : m_LayerStack)
		{
			DispatchEvent(layer, receiver, *e);
		}
	}

	bool Application::OnImmediateEvent(Widget* receiver, Event& e)
	{
		RAY_PROFILE_FUNCTION();
		RAY_EXPECTS(e.GetType() <= EventType::LAST && e.GetType() >= EventType::FIRST);

		for (auto* layer : m_LayerStack)
		{
			switch (e.GetType())
			{
			case EventType::WindowCloseEvent:
			{
				// Notify that the window has been closed
				//m_MainWindow->GetEventQueue().GetWaiter().notify_all();
				return layer->OnWindowClose(receiver, (WindowCloseEvent&)e);
			}
			default:
				RAY_ASSERT(false, "Unimplemented Event");
			}
		}
		return false;
	}
	
	void Application::DispatchEvent(Layer* layer, Widget* receiver, Event& e)
	{
		RAY_PROFILE_FUNCTION();

		RAY_EXPECTS(e.GetType() <= EventType::LAST && e.GetType() >= EventType::FIRST);
		RAY_EXPECTS(layer != nullptr);

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
			RAY_ASSERT(false, "Unimplemented Event");
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
	
	void Application::StartupSetup()
	{
		/// <summary>
		/// Handles any unexpected signals, logs the signal and closes the logger
		/// </summary>
		/// <param name="signum">Is the received signal</param>
		Util::SetSignals([](int signum)
			{
				RAY_LOG_CRITICAL("Signal '{0}' received, terminating program", signum);

				// Note: We do not need to do this as the destructor does it automatically, 
				// but it's good to have it here
				RAY_LOG_END();

				// We need to do this though
				RAY_PROFILE_END_SESSION();

				exit(signum);
			}
		);
		Util::ConsoleSetup();
	}
}


