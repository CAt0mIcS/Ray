#include "Node.h"

#include "NPE/Window/MainWindow.h"

HWND hWndParent;

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
		//wc.hbrBackground = CreateSolidBrush(RGB(15, 17, 19));

		RegisterClass(&wc);

		hWndParent = parent->GetNativeWindow();
		m_hWnd = CreateWindowEx(0, L"NODE", L"", WS_CHILD | WS_VISIBLE, pos.x, pos.y, size.width, size.height, parent->GetNativeWindow(), (HMENU)m_Id, NULL, this);

		ShowWindow(m_hWnd, SW_SHOWDEFAULT);
		UpdateWindow(m_hWnd);
	}

	LRESULT CALLBACK Node::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hWnd, &ps);

			//if (m_Size.width < 5000 && m_Size.width > -5000)
			//{
				FillRect(hDC, &ps.rcPaint, CreateSolidBrush(RGB(35, 38, 40)));
				//RoundRect(hDC, 0, 0, m_Size.width, m_Size.height, 100, 100);
				//UpdateWindow(hWndParent);
			//}

			EndPaint(hWnd, &ps);
			ReleaseDC(hWnd, hDC);
			return 0;
		}
		case WM_SIZE:
		{
			POINTS pt = MAKEPOINTS(lParam);
			m_Size.width = pt.x;
			m_Size.height = pt.y;
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

