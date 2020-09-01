#include "Node.h"

#include "NPE/Window/MainWindow.h"


namespace NPE
{
	LRESULT CALLBACK CtrlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	Node::Node(MainWindow* parent, const NPoint pos, const NSize size)
		: Control(Type::Node, pos, size)
	{

		WNDCLASS wc{};
		wc.lpfnWndProc = CtrlProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = L"NODE";
		wc.style = CS_OWNDC;
		wc.hbrBackground = CreateSolidBrush(RGB(15, 17, 19));
		RegisterClass(&wc);

		m_hWnd = CreateWindowEx(0, L"NODE", L"", WS_CHILD | WS_VISIBLE, pos.x, pos.y, size.width, size.height, parent->GetNativeWindow(), (HMENU)1, 0, 0);

		TRACKMOUSEEVENT e{};
		e.cbSize = sizeof(TRACKMOUSEEVENT);
		e.dwFlags = TME_HOVER | TME_LEAVE;
		e.dwHoverTime = 10;
		e.hwndTrack = m_hWnd;
		TrackMouseEvent(&e);

		ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	}

	LRESULT CALLBACK CtrlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps);
			FillRect(hDC, &ps.rcPaint, CreateSolidBrush(RGB(16, 17, 19)));
			EndPaint(hWnd, &ps);
			ReleaseDC(hWnd, hDC);
			return 0;
		}
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}

}



