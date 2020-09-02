#include "MainWindow.h"

#include "NPE/Controls/Node.h"


namespace NPE
{
	MainWindow::MainWindow(unsigned short width, unsigned short height, PCWSTR name)
	{
		if (!CreateNativeWindow(name, WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, width, height))
			return;

		//TODO: should be done in manifest
		//SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);


		ShowWindow(m_hWnd, SW_MAXIMIZE);
		m_Controls.emplace_back(Node(this, { 10, 10 }, { 200, 200 }));
		m_Controls.emplace_back(Node(this, { 400, 400 }, { 200, 200 }));
	}

	void MainWindow::Update(const RECT* rc, BOOL bErase)
	{
		InvalidateRect(m_hWnd, rc, FALSE);
		UpdateWindow(m_hWnd);
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
			Mouse.ClearStates();
			Keyboard.ClearStates();
			return 0;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(m_hWnd, &ps);
			Paint(hDC, &ps.rcPaint, FALSE);
			EndPaint(m_hWnd, &ps);
			ReleaseDC(m_hWnd, hDC);
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			POINTS pt = MAKEPOINTS(lParam);
			Mouse.OnMouseMove(pt.x, pt.y);
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			Mouse.OnLButtonDown();
			return 0;
		}
		case WM_LBUTTONUP:
		{
			Mouse.OnLButtonUp();
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			Mouse.OnRButtonDown();
			return 0;
		}
		case WM_RBUTTONUP:
		{
			Mouse.OnRButtonUp();
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			Mouse.OnMButtonDown();
			return 0;
		}
		case WM_MBUTTONUP:
		{
			Mouse.OnMButtonUp();
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			const auto delta = GET_WHEEL_DELTA_WPARAM(wParam);
			Mouse.OnMouseWheelDelta(delta);
			return 0;
		}
		case WM_KEYDOWN:
		{
			Keyboard.OnKeyPressed((unsigned char)wParam);
			return 0;
		}
		case WM_KEYUP:
		{
			Keyboard.OnKeyReleased((unsigned char)wParam);
			return 0;
		}
		case WM_CHAR:
		{
			Keyboard.OnChar((unsigned char)wParam);
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