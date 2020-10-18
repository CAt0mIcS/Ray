#include "rlpch.h"
#include "Window.h"

#include "Reyal/Exception.h"
#include "Reyal/Debug/ReyalLogger.h"

#include <Util/Random.h>
#include <iostream>

namespace Zeal::Reyal
{
	Window::Window(const std::wstring& windowTitle, bool isMainWindow)
		: m_IsMainWindow(isMainWindow), m_ExitCode(0)
	{
		ZL_PROFILE_FUNCTION();

		auto rnd = Util::GenerateRandomToken<std::wstring>(20);
		ZL_LOG_INFO("Creating Window class with name '{0}'", rnd);
		RL_THROW_LAST_WND_EXCEPT(CreateNativeWindow(windowTitle.c_str(), rnd.c_str(), WS_OVERLAPPEDWINDOW));
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

	LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
		{
			// TODO: Check resources of the closed window are destroyed correctly if the closed window is not the main window
			if (this->IsMainWindow())
			{
				PostQuitMessage(0);
				return 0;
			}
			break;
		}
		}

		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
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
		//TODO: Change to something more appropriate (needed so cpu doesn't go up to 30% usage when using program)
		Sleep(1);
		return false;
	}
}


