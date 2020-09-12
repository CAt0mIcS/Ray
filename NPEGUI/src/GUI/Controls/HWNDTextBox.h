#pragma once

#include "GUI/Window/BaseWindow.h"


namespace GUI
{
	class TextBox;

	class HWNDTextBox : public BaseWindow<HWNDTextBox>
	{
	public:
		HWNDTextBox(TextBox* parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height);

		void Hide();

		void Show();

		virtual LRESULT CALLBACK HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		static HWND m_hWndEdit;
		//static HWND m_hWndOkBtn;
		//static HWND m_HWndCancelBtn;

		TextBox* m_Parent;
	};

}

