#include "MainWindow.h"

#include "NPE/Controls/Node.h"


namespace NPE
{
	MainWindow::MainWindow(unsigned short width, unsigned short height, PCWSTR name)
	{
		if (!CreateNativeWindow(name, WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, width, height))
			return;

		ShowWindow(m_hWnd, SW_MAXIMIZE);
		Node node(this);
	}

	std::optional<int> MainWindow::ProcessMessage()
	{
		MSG msg{};
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				return (int)msg.wParam;
		
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(10);
		return {};
	}

	LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_KILLFOCUS:
		{
			mouse.ClearStates();
			keyboard.ClearStates();
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			POINTS pt = MAKEPOINTS(lParam);
			mouse.OnMouseMove(pt.x, pt.y);
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			mouse.OnLButtonDown();
			return 0;
		}
		case WM_LBUTTONUP:
		{
			mouse.OnLButtonUp();
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			mouse.OnRButtonDown();
			return 0;
		}
		case WM_RBUTTONUP:
		{
			mouse.OnRButtonUp();
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			mouse.OnMButtonDown();
			return 0;
		}
		case WM_MBUTTONUP:
		{
			mouse.OnMButtonUp();
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			const auto delta = GET_WHEEL_DELTA_WPARAM(wParam);
			mouse.OnMouseWheelDelta(delta);
			return 0;
		}
		case WM_KEYDOWN:
		{
			keyboard.OnKeyPressed((unsigned char)wParam);
			return 0;
		}
		case WM_KEYUP:
		{
			keyboard.OnKeyReleased((unsigned char)wParam);
			return 0;
		}
		case WM_CHAR:
		{
			keyboard.OnChar((unsigned char)wParam);
			return 0;
		}
		}

		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	void MainWindow::Paint(HDC hDC, RECT* rcDirty, BOOL bErase)
	{
		FillRect(hDC, rcDirty, CreateSolidBrush(RGB(35, 38, 40)));
	}
	
}