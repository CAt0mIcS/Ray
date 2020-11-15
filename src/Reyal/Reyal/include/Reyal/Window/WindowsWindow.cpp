#include "rlpch.h"

#ifdef _WIN32

#include "Reyal/Window/Window.h"

#include <RlUtil/Exception.h>

#include <RlUtil/Random.h>

#include "Reyal/Events/KeyboardEvent.h"
#include "Reyal/Events/MouseEvent.h"
#include "Reyal/Events/ApplicationEvent.h"

#include <RlDebug/ReyalLogger.h>
#include <RlDebug/RlAssert.h>
#include <RlDebug/Instrumentor.h>

#include <RlRender/Renderer3D.h>


namespace At0::Reyal
{
	Window::Window(const std::wstring_view name, Widget* parent, bool isMainWindow)
		: Widget(name, parent), m_IsMainWindow(isMainWindow), m_OldWindowPos{}, m_CurrentlyHovering(this), m_OldSize{}
	{
		RL_PROFILE_FUNCTION();

		auto rnd = Util::GenerateRandomToken<std::wstring>(5);
		RL_EXPECTS(!rnd.empty());
		RL_LOG_INFO("[Window] Creating Window Class with Name '{0}'", rnd);
		RL_THROW_LAST_WND_EXCEPT(CreateNativeWindow(L"", rnd.c_str(), WS_OVERLAPPEDWINDOW));
	}

	Window::~Window()
	{
		RL_PROFILE_FUNCTION();
		Close();
	}

	int64_t Window::HandleMessage(const WindowMessage& msg)
	{
		switch (msg.uMsg)
		{
		case WM_DESTROY:
		{
			// TODO: Check if resources of the closed window are destroyed correctly
			RL_LOG_DEBUG("[MessageLoop] WM_DESTROY Message in HandleMessage received");
			if (this->IsMainWindow())
			{
				PostQuitMessage(0);
				return 0;
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			POINTS pt = MAKEPOINTS(msg.lParam);
			Mouse.SetPos({ (float)pt.x, (float)pt.y });

			Scope<MouseMoveEvent> e = MakeScope<MouseMoveEvent>(Mouse.GetOldPos(), Mouse.GetPos());
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });

			// Loop over the widgets and check if the mouse left any
			SetHoveringWidget();

			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			Scope<MouseButtonPressedEvent> e = MakeScope<MouseButtonPressedEvent>(MouseButton::Left);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_LBUTTONUP:
		{
			Scope<MouseButtonReleasedEvent> e = MakeScope<MouseButtonReleasedEvent>(MouseButton::Left);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			Scope<MouseButtonPressedEvent> e = MakeScope<MouseButtonPressedEvent>(MouseButton::Middle);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_MBUTTONUP:
		{
			Scope<MouseButtonReleasedEvent> e = MakeScope<MouseButtonReleasedEvent>(MouseButton::Middle);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			Scope<MouseButtonPressedEvent> e = MakeScope<MouseButtonPressedEvent>(MouseButton::Right);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_RBUTTONUP:
		{
			Scope<MouseButtonReleasedEvent> e = MakeScope<MouseButtonReleasedEvent>(MouseButton::Right);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_KEYDOWN:
		{
			m_Renderer3D->RenderTestTriangle();
			m_Renderer3D->EndDraw();

			Scope<KeyPressedEvent> e = MakeScope<KeyPressedEvent>((unsigned char)msg.wParam, (uint32_t)(msg.lParam & 0xff));
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_KEYUP:
		{
			Scope<KeyReleasedEvent> e = MakeScope<KeyReleasedEvent>((unsigned char)msg.wParam);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_CHAR:
		{
			Scope<CharEvent> e = MakeScope<CharEvent>((unsigned char)msg.wParam);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			int delta = GET_WHEEL_DELTA_WPARAM(msg.wParam);

			if (delta > 0)
			{
				Scope<MouseWheelUpEvent> e = MakeScope<MouseWheelUpEvent>(delta);
				m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			}
			else if (delta < 0)
			{
				Scope<MouseWheelDownEvent> e = MakeScope<MouseWheelDownEvent>(delta);
				m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			}

			return 0;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(m_hWnd, &ps);
			EndPaint(m_hWnd, &ps);

			Scope<PaintEvent> e = MakeScope<PaintEvent>();
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_SIZE:
		{
			Size2 newSize = { (float)LOWORD(msg.lParam), (float)HIWORD(msg.lParam) };
			ResizeTo(newSize);

			//TODO: Read how windows handles events (how they're built, how they handle it)

			Scope<WindowResizeEvent> e = MakeScope<WindowResizeEvent>(m_OldSize, newSize);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });

			m_OldSize = newSize;
			return 0;
		}
		case WM_MOVE:
		{
			Point2 newPos = { (float)LOWORD(msg.lParam), (float)HIWORD(msg.lParam) };
			MoveTo(newPos);

			Scope<WindowMoveEvent> e = MakeScope<WindowMoveEvent>(m_OldWindowPos, newPos);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });

			m_OldWindowPos = newPos;
			return 0;
		}
		case WM_CLOSE:
		{
			RL_LOG_DEBUG("[MessageLoop] WM_CLOSE Message in HandleMessage received");
			if (m_ImmediateEvent)
			{
				if (!m_ImmediateEvent(this, WindowCloseEvent()))
				{
					break;
				}
			}
			return 0;
		}
		case WM_QUIT:
		{
			RL_LOG_DEBUG("[MessageLoop] WM_QUIT Message in HandleMessage received");
			break;
		}
		}

		return DefWindowProc(m_hWnd, msg.uMsg, msg.wParam, msg.lParam);
	}

	Renderer3D* Window::GetRenderer3D() const
	{
		RL_PROFILE_FUNCTION();

		if (GetParent())
		{
			return GetParent()->GetRenderer3D();
		}
		return m_Renderer3D.get();
	}

	WindowHandle Window::GetNativeWindow() const
	{
		return m_hWnd;
	}

	bool Window::ShouldClose(int* exitCode)
	{
		/// <summary>
		/// QUESTION:
		///		Should I use GetMessage in a while loop with a function to execute every "frame"?
		///		The application class should be able to hook into the while loop and execute some code
		/// </summary>

		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				RL_LOG_DEBUG("[MessageLoop] WM_QUIT Message Received");
				if (exitCode)
					*exitCode = (int)msg.wParam;
				return true;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//TODO: Change to something more appropriate (CPU Usage too high without it)
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		return false;
	}
	
	void Window::InitRenderer2D()
	{
		//if (!m_Renderer2D)
		//{
		//	m_Renderer2D = MakeRef<Renderer2D>();
		//	m_Renderer2D->Init(m_hWnd);
		//}
	}

	void Window::InitRenderer3D()
	{
		if (!m_Renderer3D)
		{
			m_Renderer3D = MakeScope<Renderer3D>();
			m_Renderer3D->Init(m_hWnd);
		}
	}

	void Window::SetHoveringWidget()
	{
		RL_PROFILE_FUNCTION();

		bool setNew = false;
		static auto generateEvents = [this](Widget* child)
		{
			//QUESTION: Use the HoverEnter object in receiver and event?

			Scope<HoverLeaveEvent> e = MakeScope<HoverLeaveEvent>(m_CurrentlyHovering);
			m_EventQueue.PushBack({ m_CurrentlyHovering, std::move(e) });

			m_CurrentlyHovering = child;

			Scope<HoverEnterEvent> e2 = MakeScope<HoverEnterEvent>(m_CurrentlyHovering);
			m_EventQueue.PushBack({ m_CurrentlyHovering, std::move(e) });
		};

		for (Scope<Widget>& child : m_Children)
		{
			if (Mouse.IsOnWidget(child) && *m_CurrentlyHovering != child)
			{
				generateEvents(child.get());
				setNew = true;
			}
		}

		if (!setNew && !Mouse.IsOnWidget(m_CurrentlyHovering) && Mouse.IsOnWidget(this))
		{
			generateEvents(this);
		}
		// We can assume that no widget is in focus if the mouse is outside the window rect
		// TODO: Implement IsOnWidget function
		// else if (!setNew && !Mouse.IsOnWidget(this))
		// {
		// 	generateEvents(nullptr);
		// }
	}
}


#endif // _WIN32
