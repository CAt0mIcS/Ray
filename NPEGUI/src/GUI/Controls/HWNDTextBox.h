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

		virtual LRESULT CALLBACK HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		HWND m_hWndEdit;
		HWND m_hWndOkBtn;
		HWND m_HWndCancelBtn;

		TextBox* m_Parent;
	};

}

