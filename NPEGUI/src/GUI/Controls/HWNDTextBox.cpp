#include "pch.h"
#include "HWNDTextBox.h"

#include "Util/Exceptions.h"

#include "TextBox.h"
#include "GUI/Graphics/Renderer.h"


namespace GUI
{
	HWND HWNDTextBox::s_hWndEdit = 0;
	HWND HWNDTextBox::s_hWndOkBtn = 0;
	HWND HWNDTextBox::s_HWndCancelBtn = 0;

	HWNDTextBox::HWNDTextBox(TextBox* parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
		: m_Parent(parent)
	{
		if (!CreateNativeWindow(L"", 0, 0, x, y, width, height))
			NPE_THROW_WND_EXCEPT(GetLastError());

		s_hWndEdit = CreateWindowEx(
			0, L"EDIT", L"", 
			WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
			20, 20, width - 40, height - 120, m_hWnd, 0, 0, 0
		);

		if (!s_hWndEdit)
			NPE_THROW_WND_EXCEPT(GetLastError());

		s_hWndOkBtn = CreateWindowEx(
			0, L"BUTTON", L"Ok", 
			WS_TABSTOP | BS_DEFPUSHBUTTON | WS_BORDER | WS_CHILD | WS_VISIBLE,
			20, height - 90, 150, 40, m_hWnd, (HMENU)s_OkBtnId, GetModuleHandle(NULL), 0
		);
		
		if (!s_hWndOkBtn)
			NPE_THROW_WND_EXCEPT(GetLastError());
		
		s_HWndCancelBtn = CreateWindowEx(
			0, L"BUTTON", L"Cancel",
			WS_TABSTOP | BS_DEFPUSHBUTTON | WS_BORDER | WS_CHILD | WS_VISIBLE,
			190, height - 90, 150, 40, m_hWnd, (HMENU)s_CancelBtnId, GetModuleHandle(NULL), 0
		);
	}

	void HWNDTextBox::Hide()
	{
		ShowWindow(m_hWnd, SW_HIDE);
	}

	void HWNDTextBox::Show()
	{
		ShowWindow(m_hWnd, SW_SHOWDEFAULT); 
		SetWindowText(s_hWndEdit, m_Parent->m_Text.c_str());
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
		case WM_COMMAND:
		{
			//OkBtn
			if (LOWORD(wParam) == s_OkBtnId)
			{
				OnOkButtonPressed();
			}
			//CancelBtn
			else if (LOWORD(wParam) == s_CancelBtnId)
			{
				OnCancelBtnPressed();
			}

			return 0;
		}
		case WM_PAINT:
		{
			
		}
		}

		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	void HWNDTextBox::OnOkButtonPressed()
	{
		int len = GetWindowTextLength(s_hWndEdit);
		if (len < m_Parent->m_Text.capacity())
			m_Parent->m_Text.reserve(len + 1ll);

		GetWindowText(s_hWndEdit, (wchar_t*)m_Parent->m_Text.c_str(), (int)m_Parent->m_Text.capacity());

		Renderer::Get().BeginDraw();

		m_Parent->Render();

		Renderer::Get().EndDraw();
		Hide();
	}

	void HWNDTextBox::OnCancelBtnPressed()
	{
		SetWindowText(s_hWndEdit, L"");
		Hide();
	}
}


