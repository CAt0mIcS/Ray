#include "pch.h"
#include "HWNDTextBox.h"

#include "Util/Exceptions.h"

#include "TextBox.h"
#include "GUI/Graphics/Renderer.h"


namespace GUI
{
	HWNDTextBox::HWNDTextBox(TextBox* parent)
		: m_Parent(parent)
	{

	}

	void HWNDTextBox::CreateTextWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	{
		if (!CreateNativeWindow(L"", WS_OVERLAPPEDWINDOW, 0, x, y, width, height))
		{
			NPE_THROW_WND_EXCEPT(GetLastError());
		}

		m_hWndEdit = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 0, 0, width, height, m_hWnd, 0, 0, 0);

		if (!m_hWndEdit)
		{
			NPE_THROW_WND_EXCEPT(GetLastError());
		}
		ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	}

	LRESULT HWNDTextBox::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
		{
			int len = GetWindowTextLength(m_hWndEdit);
			if(len < m_Text.capacity())
				m_Text.reserve(len + 1);
			
			GetWindowText(m_hWndEdit, (wchar_t*)m_Parent->m_Text.c_str(), (int)m_Text.capacity());

			Renderer::Get().BeginDraw();
			
			m_Parent->Render();
			
			Renderer::Get().EndDraw();

			return 0;
		}
		case WM_PAINT:
		{
			
		}
		}

		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
}


