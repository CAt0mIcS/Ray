#pragma once

#include "GUI/Window/BaseWindow.h"


namespace GUI
{
	class TextBox;

	class HWNDTextBox : public BaseWindow<HWNDTextBox>
	{
	public:
		HWNDTextBox(TextBox* parent);

		void CreateTextWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

		const std::wstring& GetText() const { return m_Text; }

		void FlushText() { m_Text = L""; }

		virtual LRESULT CALLBACK HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		HWND m_hWndEdit;
		std::wstring m_Text;
		TextBox* m_Parent;
	};

}

