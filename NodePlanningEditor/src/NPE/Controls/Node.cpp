#include "Node.h"

#include "NPE/Window/MainWindow.h"


namespace NPE
{
	Node::Node(MainWindow* parent, const NPoint pos, const NSize size)
		: Control(Type::Node, pos, size)
	{

		WNDCLASS wc{};
		wc.lpfnWndProc = HandleMessageSetup;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = L"NODE";
		wc.style = CS_OWNDC;
		wc.hbrBackground = CreateSolidBrush(RGB(15, 17, 19));
		RegisterClass(&wc);

		m_hWnd = CreateWindowEx(0, L"NODE", L"", WS_CHILD | WS_VISIBLE, pos.x, pos.y, size.width, size.height, parent->GetNativeWindow(), (HMENU)m_Id, 0, this);

		TRACKMOUSEEVENT e{};
		e.cbSize = sizeof(TRACKMOUSEEVENT);
		e.dwFlags = TME_HOVER | TME_LEAVE;
		e.dwHoverTime = 10;
		e.hwndTrack = m_hWnd;
		TrackMouseEvent(&e);

		ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	}

	LRESULT CALLBACK Node::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps);
			//FillRect(hDC, &ps.rcPaint, CreateSolidBrush(RGB(16, 17, 19)));
			
			if(this)
				RoundRect(hDC, 0, 0, m_Size.width, m_Size.height, 100, 100);
			
			EndPaint(hWnd, &ps);
			ReleaseDC(hWnd, hDC);
			return 0;
		}
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}

	LRESULT CALLBACK Node::HandleMessageSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			Node* node = (Node*)pCreate->lpCreateParams;

			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)node);
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)HandleMessagePass);
			return node->HandleMessage(hWnd, uMsg, wParam, lParam);
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	LRESULT Node::HandleMessagePass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Node* node = (Node*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		return node->HandleMessage(hWnd, uMsg, wParam, lParam);
	}

}



