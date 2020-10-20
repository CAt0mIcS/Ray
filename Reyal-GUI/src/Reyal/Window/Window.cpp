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
	Window::Window(const std::wstring_view windowTitle, bool isMainWindow)
		: m_IsMainWindow(isMainWindow), m_ExitCode(0)
	{
		ZL_PROFILE_FUNCTION();

		auto rnd = Util::GenerateRandomToken<std::wstring>(5);
		ZL_LOG_INFO("Creating Window class with name '{0}'", rnd);
		RL_THROW_LAST_WND_EXCEPT(CreateNativeWindow(windowTitle.data(), rnd.c_str(), WS_OVERLAPPEDWINDOW));
	}

	Window::~Window()
	{
		ZL_PROFILE_FUNCTION();
		Close();
	}

	LRESULT Window::HandleMessage(_In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
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
			Mouse.SetMousePos({ (float)pt.x, (float)pt.y });

			MouseMoveEvent e;
			if (m_CallbackFunc(GetEventReceiver(e), e)) break;
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			MouseButtonPressedEvent e(MouseButton::Left);
			if (m_CallbackFunc(GetEventReceiver(e), e)) break;
			return 0;
		}
		case WM_LBUTTONUP:
		{
			MouseButtonReleasedEvent e(MouseButton::Left);
			if (m_CallbackFunc(GetEventReceiver(e), e)) break;
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			MouseButtonPressedEvent e(MouseButton::Middle);
			if (m_CallbackFunc(GetEventReceiver(e), e)) break;
			return 0;
		}
		case WM_MBUTTONUP:
		{
			MouseButtonReleasedEvent e(MouseButton::Middle);
			if (m_CallbackFunc(GetEventReceiver(e), e)) break;
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			MouseButtonPressedEvent e(MouseButton::Right);
			if (m_CallbackFunc(GetEventReceiver(e), e)) break;
			return 0;
		}
		case WM_RBUTTONUP:
		{
			MouseButtonReleasedEvent e(MouseButton::Right);
			if (m_CallbackFunc(GetEventReceiver(e), e)) break;
			return 0;
		}
		case WM_KEYDOWN:
		{
			KeyPressedEvent e(wParam);
			if (m_CallbackFunc(GetEventReceiver(e), e)) break;
			return 0;
		}
		case WM_KEYUP:
		{
			KeyReleasedEvent e(wParam);
			if (m_CallbackFunc(GetEventReceiver(e), e)) break;
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			int delta = GET_WHEEL_DELTA_WPARAM(wParam);

			if (delta > 0)
			{
				MouseWheelUpEvent e(delta);
				if (m_CallbackFunc(GetEventReceiver(e), e)) break;
			}
			else if (delta < 0)
			{
				MouseWheelDownEvent e(delta);
				if (m_CallbackFunc(GetEventReceiver(e), e)) break;
			}

			return 0;
		}
		case WM_PAINT:
		{
			PaintEvent e;
			if (m_CallbackFunc(GetEventReceiver(e), e)) break;
			return 0;
		}
		case WM_SIZE:
		{
			WindowResizeEvent e;
			if (m_CallbackFunc(GetEventReceiver(e), e)) break;
			return 0;
		}
		case WM_CLOSE:
		{
			WindowCloseEvent e;
			if (m_CallbackFunc(GetEventReceiver(e), e)) return 0;
			break;
		}
		}

		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	
	void Window::Show(_In_opt_ ShowCommand cmdShow) const
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
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
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
}


