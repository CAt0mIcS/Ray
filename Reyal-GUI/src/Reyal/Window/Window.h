#pragma once

#include "BaseWindow.h"


namespace Zeal::Reyal
{
	class RL_API Window : public BaseWindow<Window>
	{
	public:
		Window(const std::wstring& windowTitle);

		void Show(uint8_t cmdShow = SW_SHOWDEFAULT);

		void Hide();

		void Close();

		static int ProcessMessages();

		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	};
}
