#include "rlpch.h"
#include "Window.h"

#include "Reyal/Exception.h"


namespace Zeal::Reyal
{
	Window::Window(const std::wstring& windowTitle)
	{
		RL_THROW_LAST_WND_EXCEPT(CreateNativeWindow(windowTitle.c_str(), L"WinCls", WS_OVERLAPPEDWINDOW));
	}

	void Window::Show(uint8_t cmdShow)
	{
		ShowWindow(m_hWnd, cmdShow);
	}

	void Window::Hide()
	{
		ShowWindow(m_hWnd, SW_HIDE);
	}

	void Window::Close()
	{
		SendMessage(m_hWnd, WM_CLOSE, 0, 0);
	}

	int Window::ProcessMessages()
	{
		MSG msg;
		while (true)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					break;
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		return (int)msg.wParam;
	}

	LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		}

		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
}


