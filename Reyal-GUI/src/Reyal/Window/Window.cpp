#include "rlpch.h"
#include "Window.h"

#include "Reyal/Exception.h"
#include "Reyal/Debug/ReyalLogger.h"

#include <Util/Random.h>

#include "Reyal/Events/KeyboardEvent.h"
#include "Reyal/Events/MouseEvent.h"
#include "Reyal/Events/ApplicationEvent.h"


namespace Zeal::Reyal
{
	Window::Window(const std::wstring_view name, _In_opt_ Widget* parent, bool isMainWindow)
		: Widget(name, parent), m_IsMainWindow(isMainWindow), m_ExitCode(0), m_CurrentHover()
	{
		ZL_PROFILE_FUNCTION();

		auto rnd = Util::GenerateRandomToken<std::wstring>(5);
		ZL_LOG_INFO("[Window] Creating Window Class with Name '{0}'", rnd);
		RL_THROW_LAST_WND_EXCEPT(CreateNativeWindow(L"", rnd.c_str(), WS_OVERLAPPEDWINDOW));

		if(!parent || !parent->GetRenderer())
			m_Renderer.Init(m_hWnd);
	}

	Window::~Window()
	{
		ZL_PROFILE_FUNCTION();
		Close();
	}

	LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (!m_CallbackFunc)
			return DefWindowProc(m_hWnd, uMsg, wParam, lParam);

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
			Mouse.SetMousePos({ (float)pt.x, (float)pt.y });

			MouseMoveEvent e;
			if (DispatchEvent(e)) break;

			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			MouseButtonPressedEvent e(MouseButton::Left);
			if (DispatchEvent(e)) break;
			return 0;
		}
		case WM_LBUTTONUP:
		{
			MouseButtonReleasedEvent e(MouseButton::Left);
			if (DispatchEvent(e)) break;
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			MouseButtonPressedEvent e(MouseButton::Middle);
			if (DispatchEvent(e)) break;
			return 0;
		}
		case WM_MBUTTONUP:
		{
			MouseButtonReleasedEvent e(MouseButton::Middle);
			if (DispatchEvent(e)) break;
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			MouseButtonPressedEvent e(MouseButton::Right);
			if (DispatchEvent(e)) break;
			return 0;
		}
		case WM_RBUTTONUP:
		{
			MouseButtonReleasedEvent e(MouseButton::Right);
			if (DispatchEvent(e)) break;
			return 0;
		}
		case WM_KEYDOWN:
		{
			KeyPressedEvent e(wParam);
			if (DispatchEvent(e)) break;
			return 0;
		}
		case WM_KEYUP:
		{
			KeyReleasedEvent e(wParam);
			if (DispatchEvent(e)) break;
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			int delta = GET_WHEEL_DELTA_WPARAM(wParam);

			if (delta > 0)
			{
				MouseWheelUpEvent e(delta);
				if (DispatchEvent(e)) break;
			}
			else if (delta < 0)
			{
				MouseWheelDownEvent e(delta);
				if (DispatchEvent(e)) break;
			}

			return 0;
		}
		case WM_PAINT:
		{
			PaintEvent e;
			if (DispatchEvent(e)) break;
			return 0;
		}
		case WM_SIZE:
		{
			ResizeTo({ (float)LOWORD(lParam), (float)HIWORD(lParam) });

			WindowResizeEvent e;
			if (DispatchEvent(e)) break;
			return 0;
		}
		case WM_MOVE:
		{
			MoveTo({ (float)LOWORD(lParam), (float)HIWORD(lParam) });

			WindowMoveEvent e;
			if (DispatchEvent(e)) break;
			return 0;
		}
		case WM_CLOSE:
		{
			WindowCloseEvent e;
			if (DispatchEvent(e)) return 0;
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

	WindowRenderer* Window::GetRenderer()
	{
		ZL_PROFILE_FUNCTION();

		if (GetParent())
		{
			return GetParent()->GetRenderer();
		}
		return &m_Renderer;
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
		//TODO: Change to something more appropriate (needed so cpu doesn't go up to 30% usage when running)
		Sleep(1);
		return false;
	}
	
	bool Window::DispatchEvent(Event& e)
	{
		Widget* receiver = GetEventReceiver(e, Mouse);

		if (!receiver)
			receiver = this;

		return m_CallbackFunc(receiver, e);
	}
}


