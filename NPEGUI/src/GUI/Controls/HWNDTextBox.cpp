#include "pch.h"
#include "HWNDTextBox.h"

#include "Util/Exceptions.h"

#include "TextBox.h"
#include "GUI/Graphics/Renderer.h"


namespace GUI
{
	HWND HWNDTextBox::m_hWndEdit = 0;
	HWND HWNDTextBox::m_hWndOkBtn = 0;
	HWND HWNDTextBox::m_HWndCancelBtn = 0;

	HWNDTextBox::HWNDTextBox(TextBox* parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
		: m_Parent(parent)
	{
		if (!CreateNativeWindow(L"", 0, 0, x, y, width, height))
		{
			NPE_THROW_WND_EXCEPT(GetLastError());
		}

		m_hWndEdit = CreateWindowEx(0, L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 20, 20, width - 40, height - 120, m_hWnd, 0, 0, 0);

		if (!m_hWndEdit)
			NPE_THROW_WND_EXCEPT(GetLastError());

		m_hWndOkBtn = CreateWindowEx(0, L"BUTTON", L"Ok", WS_TABSTOP | BS_DEFPUSHBUTTON | WS_BORDER | WS_CHILD | WS_VISIBLE, 20, height - 90, 150, 40, m_hWnd, (HMENU)1, GetModuleHandle(NULL), 0);

		if (!m_hWndOkBtn)
			NPE_THROW_WND_EXCEPT(GetLastError());

		m_HWndCancelBtn = CreateWindowEx(0, L"BUTTON", L"Cancel", WS_TABSTOP | BS_DEFPUSHBUTTON | WS_BORDER | WS_CHILD | WS_VISIBLE, 190, height - 90, 150, 40, m_hWnd, (HMENU)2, GetModuleHandle(NULL), 0);
	}

	void HWNDTextBox::Hide()
	{
		ShowWindow(m_hWnd, SW_HIDE);
	}

	void HWNDTextBox::Show()
	{
		ShowWindow(m_hWnd, SW_SHOWDEFAULT); 
		SetWindowText(m_hWndEdit, m_Parent->m_Text.c_str());
	}

	LRESULT HWNDTextBox::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CREATE:
		{
			// Disable default titlebar icon 
			int extendedStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
			SetWindowLong(m_hWnd, GWL_EXSTYLE, extendedStyle | WS_EX_DLGMODALFRAME);

			// Update non-client area of the dialog, for the changes to take effect
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER, 0);
			return 0;
		}
		case WM_DESTROY:
		{
			return 0;
		}
		case WM_COMMAND:
		{
			//OkBtn
			if (LOWORD(wParam) == 1)
			{
				int len = GetWindowTextLength(m_hWndEdit);
				if (len < m_Parent->m_Text.capacity())
					m_Parent->m_Text.reserve(len + 1ll);

				GetWindowText(m_hWndEdit, (wchar_t*)m_Parent->m_Text.c_str(), (int)m_Parent->m_Text.capacity());

				Renderer::Get().BeginDraw();

				m_Parent->Render();

				Renderer::Get().EndDraw();
				Hide();
			}
			//CancelBtn
			else if (LOWORD(wParam) == 2)
			{
				SetWindowText(m_hWndEdit, L"");
				Hide();
			}

			return 0;
		}
		case WM_PAINT:
		{
			
		}
		}

		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
}


