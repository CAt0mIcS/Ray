#include "rlpch.h"
#include "Window.h"

#include "Reyal/Exception.h"
#include "Reyal/Debug/ReyalLogger.h"

#include "Reyal/Util/Random.h"

#include "Reyal/Events/KeyboardEvent.h"
#include "Reyal/Events/MouseEvent.h"
#include "Reyal/Events/ApplicationEvent.h"


namespace At0::Reyal
{
	Window::Window(const std::wstring_view name, _In_opt_ Widget* parent, bool isMainWindow)
		: Widget(name, parent), m_IsMainWindow(isMainWindow), m_ExitCode(0), m_OldWindowPos{}
	{
		ZL_PROFILE_FUNCTION();

		auto rnd = GenerateRandomToken<std::wstring>(5);
		ZL_LOG_INFO("[Window] Creating Window Class with Name '{0}'", rnd);
		RL_THROW_LAST_WND_EXCEPT(CreateNativeWindow(L"", rnd.c_str(), WS_OVERLAPPEDWINDOW));

		if(!parent || !parent->GetRenderer2D())
			m_Renderer2D.Init(m_hWnd);

		m_Renderer3D.Init(m_hWnd);
	}

	Window::~Window()
	{
		ZL_PROFILE_FUNCTION();
		Close();
	}

	LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
		{
			// TODO: Check if resources of the closed window are destroyed correctly
			if (this->IsMainWindow())
			{
				PostQuitMessage(0);
				return 0;
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			POINTS pt = MAKEPOINTS(lParam);
			Mouse.SetPos({ (float)pt.x, (float)pt.y });

			Scope<MouseMoveEvent> e = MakeScope<MouseMoveEvent>(Mouse.GetOldPos(), Mouse.GetPos());
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });

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
			m_Renderer3D.RenderTestTriangle();
			m_Renderer3D.EndDraw();

			Scope<KeyPressedEvent> e = MakeScope<KeyPressedEvent>((unsigned char)wParam, lParam & 0xff);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_KEYUP:
		{
			Scope<KeyReleasedEvent> e = MakeScope<KeyReleasedEvent>((unsigned char)wParam);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			int delta = GET_WHEEL_DELTA_WPARAM(wParam);

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
			Size newSize = { (float)LOWORD(lParam), (float)HIWORD(lParam) };
			ResizeTo(newSize);

			//TODO: Read how windows handles events (how they're built, how they handle it)

			Scope<WindowResizeEvent> e = MakeScope<WindowResizeEvent>(m_OldSize, newSize);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });

			m_OldSize = newSize;
			return 0;
		}
		case WM_MOVE:
		{
			Point newPos = { (float)LOWORD(lParam), (float)HIWORD(lParam) };
			MoveTo(newPos);

			Scope<WindowMoveEvent> e = MakeScope<WindowMoveEvent>(m_OldWindowPos, newPos);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });

			m_OldWindowPos = newPos;
			return 0;
		}
		case WM_CLOSE:
		{
			//WindowCloseEvent e;
			//if (DispatchEvent(e)) return 0;
			break;
		}
		}

		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	
	std::wstring Window::GetTitle() const
	{
		ZL_PROFILE_FUNCTION();

		std::wstring str = L"";
		int len = GetWindowTextLength(m_hWnd);
		if (len > 0)
		{
			str.resize(len);
			GetWindowText(m_hWnd, str.data(), len + 1);
		}

		return str;
	}

	Renderer2D* Window::GetRenderer2D()
	{
		ZL_PROFILE_FUNCTION();

		if (GetParent())
		{
			return GetParent()->GetRenderer2D();
		}
		return &m_Renderer2D;
	}

	void Window::SetTitle(const std::wstring_view title)
	{
		ZL_PROFILE_FUNCTION();

		SetWindowText(m_hWnd, title.data());
	}

	void Window::Show(ShowCommand cmdShow) const
	{
		ZL_PROFILE_FUNCTION();
		ShowWindow(m_hWnd, (int)cmdShow);
	}

	void Window::Hide() const
	{
		ZL_PROFILE_FUNCTION();
		this->Show(ShowCommand::Hide);
	}

	void Window::Close() const
	{
		ZL_PROFILE_FUNCTION();
		SendMessage(m_hWnd, WM_CLOSE, 0, 0);
	}

	bool Window::IsOpen() const
	{
		ZL_PROFILE_FUNCTION();
		return IsWindowVisible(m_hWnd);
	}

	bool Window::ShouldClose()
	{
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				m_ExitCode = (int)msg.wParam;
				return true;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return false;
	}
}


