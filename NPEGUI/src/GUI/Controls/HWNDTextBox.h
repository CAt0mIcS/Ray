#pragma once

#include "GUI/Window/BaseWindow.h"


namespace GUI
{
	class TextBox;

	class HWNDTextBox : public BaseWindow<HWNDTextBox>
	{
	public:
		HWNDTextBox(TextBox* parent, float x, float y, float width, float height);

		void Show();

		void Hide();

		virtual LRESULT CALLBACK HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		const std::wstring& GetText() const { return m_Text; }

		void FlushText() { m_Text = L""; }

	private:
		HWND m_hWndEdit;
		std::wstring m_Text;
		TextBox* m_Parent;
	};

}

