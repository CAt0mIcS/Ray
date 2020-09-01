#include "Node.h"

#include "NPE/Window/MainWindow.h"


namespace NPE
{
	LRESULT CALLBACK CtrlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	Node::Node(MainWindow* parent)
	{

		WNDCLASS wc{};
		wc.lpfnWndProc = CtrlProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = L"MyBtn";
		wc.style = CS_OWNDC;
		RegisterClass(&wc);

		m_hWnd = CreateWindowEx(0, L"MyBtn", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, 100, 100, parent->GetNativeWindow(), (HMENU)1, 0, 0);

		TRACKMOUSEEVENT e{};
		e.cbSize = sizeof(TRACKMOUSEEVENT);
		e.dwFlags = TME_HOVER | TME_LEAVE;
		e.dwHoverTime = 10;
		e.hwndTrack = m_hWnd;
		TrackMouseEvent(&e);

		ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	}

	void CustomPaint(HDC hdc, RECT rcPaint, COLORREF col)
	{
		FillRect(hdc, &rcPaint, CreateSolidBrush(col));
	}

	LRESULT CALLBACK CtrlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			//FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			EndPaint(hWnd, &ps);
			ReleaseDC(hWnd, hdc);
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			InvalidateRect(hWnd, NULL, TRUE);
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			CustomPaint(hdc, ps.rcPaint, RGB(50, 50, 50));
			EndPaint(hWnd, &ps);
			ReleaseDC(hWnd, hdc);
			
			TRACKMOUSEEVENT e{};
			e.cbSize = sizeof(TRACKMOUSEEVENT);
			e.dwFlags = TME_LEAVE;
			e.dwHoverTime = 10;
			e.hwndTrack = hWnd;
			TrackMouseEvent(&e);
			
			return 0;
		}
		case WM_MOUSELEAVE:
		{
			InvalidateRect(hWnd, NULL, TRUE);
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			CustomPaint(hdc, ps.rcPaint, RGB(255, 255, 255));
			EndPaint(hWnd, &ps);
			ReleaseDC(hWnd, hdc);
			
			return 0;
		}
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}

}



