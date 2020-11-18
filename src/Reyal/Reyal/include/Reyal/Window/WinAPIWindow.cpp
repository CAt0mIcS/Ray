#include "rlpch.h"

#ifdef _WIN32

#include "WinAPIWindow.h"

#include <RlDebug/Instrumentor.h>
#include <RlDebug/ReyalLogger.h>
#include <RlDebug/RlAssert.h>

#include <RlUtil/Exception.h>
#include <RlUtil/Random.h>

#include <RlRender/Renderer3D.h>

#include "Reyal/Events/ApplicationEvent.h"
#include "Reyal/Events/MouseEvent.h"
#include "Reyal/Events/KeyboardEvent.h"

#include <Windows.h>


namespace At0::Reyal
{
	WinAPIWindow::WinAPIWindow(const std::string_view name, Widget* parent, bool isMainWindow)
		: Window(name, parent, isMainWindow)
	{
		RL_PROFILE_FUNCTION();

		auto rnd = Util::GenerateRandomToken<std::wstring>(5);
		RL_EXPECTS(!rnd.empty());
		RL_LOG_INFO("[Window] Creating Window Class with Name '{0}'", rnd);
		RL_THROW_LAST_WND_EXCEPT(CreateNativeWindow(L"", rnd.c_str(), WS_OVERLAPPEDWINDOW));
	}

	WinAPIWindow::~WinAPIWindow()
	{
		RL_PROFILE_FUNCTION();
	}

	bool WinAPIWindow::ShouldClose()
	{
		RL_PROFILE_FUNCTION();

		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				RL_LOG_DEBUG("[MessageLoop] WM_QUIT Message Received");
				return true;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//TODO: Change to something more appropriate (CPU Usage too high without it)
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		return false;
	}

	bool WinAPIWindow::CreateNativeWindow(
		const wchar_t* windowName, 
		const wchar_t* windowClassName, 
		DWORD style, DWORD exStyle, 
		uint32_t x, uint32_t y, 
		uint32_t width, uint32_t height, 
		HWND hWndParent, HMENU hMenu
	)
	{
		RL_PROFILE_FUNCTION();

		WNDCLASS wc{};
		wc.lpfnWndProc = WinAPIWindow::WindowProcSetup;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = windowClassName;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);

		RegisterClass(&wc);

		m_hWnd = (void*)CreateWindowEx(
			exStyle, windowClassName, windowName, style, x, y, width, height,
			hWndParent, hMenu, wc.hInstance, this
		);

		return m_hWnd ? true : false;
	}
	
	LRESULT WinAPIWindow::WindowProcSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		RL_PROFILE_FUNCTION();

		WinAPIWindow* pThis;
		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (WinAPIWindow*)pCreate->lpCreateParams;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);

			pThis->m_hWnd = hWnd;
		}
		else
		{
			pThis = (WinAPIWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		}

		if (pThis)
			return pThis->HandleMessage(uMsg, wParam, lParam);

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	
	LRESULT WinAPIWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		RL_PROFILE_FUNCTION();

		switch (uMsg)
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
			POINTS pt = MAKEPOINTS(lParam);
			Mouse.SetPos({ (float)pt.x, (float)pt.y });

			Scope<MouseMoveEvent> e = MakeScope<MouseMoveEvent>(Mouse.GetOldPos(), Mouse.GetPos());
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });

			// Loop over the widgets and check if the mouse left any
			SetHoveringWidget();

			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			Mouse.SetLeftPressed(true);

			Scope<MouseButtonPressedEvent> e = MakeScope<MouseButtonPressedEvent>(MouseButton::Left);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_LBUTTONUP:
		{
			Mouse.SetLeftPressed(false);

			Scope<MouseButtonReleasedEvent> e = MakeScope<MouseButtonReleasedEvent>(MouseButton::Left);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			Mouse.SetMiddlePressed(true);

			Scope<MouseButtonPressedEvent> e = MakeScope<MouseButtonPressedEvent>(MouseButton::Middle);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_MBUTTONUP:
		{
			Mouse.SetMiddlePressed(false);

			Scope<MouseButtonReleasedEvent> e = MakeScope<MouseButtonReleasedEvent>(MouseButton::Middle);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			Mouse.SetRightPressed(true);

			Scope<MouseButtonPressedEvent> e = MakeScope<MouseButtonPressedEvent>(MouseButton::Right);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_RBUTTONUP:
		{
			Mouse.SetRightPressed(false);

			Scope<MouseButtonReleasedEvent> e = MakeScope<MouseButtonReleasedEvent>(MouseButton::Right);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_KEYDOWN:
		{
			m_Renderer3D->RenderTestTriangle();
			m_Renderer3D->EndDraw();

			Keyboard.SetKeyState((unsigned char)wParam, true);

			Scope<KeyPressedEvent> e = MakeScope<KeyPressedEvent>((unsigned char)wParam, (uint32_t)(lParam & 0xff));
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_KEYUP:
		{
			Keyboard.SetKeyState((unsigned char)wParam, false);

			Scope<KeyReleasedEvent> e = MakeScope<KeyReleasedEvent>((unsigned char)wParam);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_CHAR:
		{
			Scope<CharEvent> e = MakeScope<CharEvent>((unsigned char)wParam);
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
		case WM_MOUSEHWHEEL:
		{
			int delta = GET_WHEEL_DELTA_WPARAM(wParam);

			if (delta > 0)
			{
				Scope<MouseWheelRightEvent> e = MakeScope<MouseWheelRightEvent>(delta);
				m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			}
			else if (delta < 0)
			{
				Scope<MouseWheelLeftEvent> e = MakeScope<MouseWheelLeftEvent>(delta);
				m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			}

			return 0;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint((HWND)m_hWnd, &ps);
			EndPaint((HWND)m_hWnd, &ps);

			Scope<PaintEvent> e = MakeScope<PaintEvent>();
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });
			return 0;
		}
		case WM_SIZE:
		{
			Size2 newSize = { (float)LOWORD(lParam), (float)HIWORD(lParam) };
			ResizeTo(newSize);

			//TODO: Read how windows handles events (how they're built, how they handle it)

			Scope<WindowResizeEvent> e = MakeScope<WindowResizeEvent>(m_OldSize, newSize);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });

			m_OldSize = newSize;
			return 0;
		}
		case WM_MOVE:
		{
			Point2 newPos = { (float)LOWORD(lParam), (float)HIWORD(lParam) };
			MoveTo(newPos);

			Scope<WindowMoveEvent> e = MakeScope<WindowMoveEvent>(m_OldPos, newPos);
			m_EventQueue.PushBack({ GetEventReceiver(*e, Mouse), std::move(e) });

			m_OldPos = newPos;
			return 0;
		}
		case WM_CLOSE:
		{
			RL_LOG_DEBUG("[MessageLoop] WM_CLOSE Message in HandleMessage received");
			if (m_ImmediateEventFn)
			{
				if (!m_ImmediateEventFn(this, WindowCloseEvent()))
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

		return DefWindowProc((HWND)m_hWnd, uMsg, wParam, lParam);
	}

	std::string WinAPIWindow::GetTitle() const
	{
		std::string str;
		int len = GetWindowTextLengthA((HWND)m_hWnd);
		if (len > 0)
		{
			str.resize(len);
			GetWindowTextA((HWND)m_hWnd, str.data(), len + 1);
		}

		return str;
	}

	void WinAPIWindow::SetTitle(const std::string_view title)
	{
		RL_THROW_LAST_WND_EXCEPT(SetWindowTextA((HWND)m_hWnd, title.data()));
	}

	void WinAPIWindow::Show() const
	{
		ShowWindow((HWND)m_hWnd, SW_SHOW);
	}

	void WinAPIWindow::Hide() const
	{
		ShowWindow((HWND)m_hWnd, SW_HIDE);
	}

	void WinAPIWindow::Maximize() const
	{
		ShowWindow((HWND)m_hWnd, SW_MAXIMIZE);
	}

	void WinAPIWindow::Minimize() const
	{
		ShowWindow((HWND)m_hWnd, SW_MINIMIZE);
	}

	void WinAPIWindow::Close() const
	{
		SendMessage((HWND)m_hWnd, WM_CLOSE, 0, 0);
	}

	bool WinAPIWindow::IsOpen() const
	{
		return IsWindowVisible((HWND)m_hWnd);
	}
}

#endif // _WIN32