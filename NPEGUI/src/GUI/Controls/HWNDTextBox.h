#pragma once

#include "GUI/Window/BaseWindow.h"


namespace GUI
{
	class TextBox;

	class HWNDTextBox : public BaseWindow<HWNDTextBox>
	{
	public:
		/**
		* HWNDTextBox Constructor
		* 
		* @param parent is the parent textbox
		* @param x is the x position of the new window
		* @param y is the y position of the new window
		* @param width is the width of the new window
		* @param height is the height of the new window
		*/
		HWNDTextBox(TextBox* parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height);

		/**
		* Shows the edit window with the current textbox text
		*/
		void Show();

		/**
		* Hides the window
		*/
		void Hide();

		/**
		* Handles edit window messages
		* 
		* @param uMsg is the message code
		* @param wParam is the wParam
		* @param lParam is the lParam
		* 
		* @returns handled status
		*/
		virtual LRESULT CALLBACK HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		void OnOkButtonPressed();

		void OnCancelBtnPressed();

	private:
		static HWND s_hWndEdit;
		static HWND s_hWndOkBtn;
		static HWND s_HWndCancelBtn;

		TextBox* m_Parent;

		static constexpr char s_OkBtnId = 0x1;
		static constexpr char s_CancelBtnId = 0x2;
	};

}

