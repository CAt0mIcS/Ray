#include "rlpch.h"
#include "Window.h"

#include "Reyal/Exception.h"
#include "Reyal/Debug/ReyalLogger.h"

#include <Util/Random.h>


namespace Zeal::Reyal
{
	Window::Window(const std::wstring& windowTitle, bool isMainWindow)
		: m_IsMainWindow(isMainWindow), m_ExitCode(0)
	{
		ZL_PROFILE_FUNCTION();

		auto rnd = Util::GenerateRandomToken<std::wstring>(5);
		ZL_LOG_INFO("Creating Window class with name '{0}'", rnd);
		RL_THROW_LAST_WND_EXCEPT(CreateNativeWindow(windowTitle.c_str(), rnd.c_str(), WS_OVERLAPPEDWINDOW));
	}

	Window::~Window()
	{
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
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			return 0;
		}
		case WM_LBUTTONUP:
		{
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			return 0;
		}
		case WM_MBUTTONUP:
		{
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			return 0;
		}
		case WM_RBUTTONUP:
		{
			return 0;
		}
		case WM_KEYDOWN:
		{
			return 0;
		}
		case WM_KEYUP:
		{
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			return 0;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(m_hWnd, &ps);
			FillRect(hDC, &ps.rcPaint, CreateSolidBrush(RGB(35, 37, 40)));
			EndPaint(m_hWnd, &ps);
			return 0;
		}
		case WM_SIZE:
		{
			return 0;
		}
		case WM_CLOSE:
		{
			break;
		}
		}

		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	
	void Window::Show(_In_opt_ ShowCommand cmdShow) const
	{
		ShowWindow(m_hWnd, (int)cmdShow);
	}

	void Window::Hide() const
	{
		this->Show(ShowCommand::Hide);
	}

	void Window::Close() const
	{
		SendMessage(m_hWnd, WM_CLOSE, 0, 0);
	}

	bool Window::IsOpen() const
	{
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
}


