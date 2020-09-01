#include "MainWindow.h"

#include "NPE/Controls/Node.h"


namespace NPE
{
	MainWindow::MainWindow(unsigned short width, unsigned short height, PCWSTR name)
		: m_Controls{}
	{
		if (!CreateNativeWindow(name, WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, width, height))
			return;

		ShowWindow(m_hWnd, SW_MAXIMIZE);
		m_Controls.emplace_back(Node(this, { 150, 540 }, { 200, 270 }));
		m_Controls.emplace_back(Node(this, { 450, 200 }, { 200, 150 }));
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
			POINTS pt = MAKEPOINTS(lParam);
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