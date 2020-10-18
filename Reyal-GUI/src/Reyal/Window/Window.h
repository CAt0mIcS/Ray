#pragma once

#include "BaseWindow.h"


namespace Zeal::Reyal
{
	class RL_API Window : public BaseWindow<Window>
	{
	public:
		Window(const std::wstring& windowTitle, bool isMainWindow = false);

		void Show(uint8_t cmdShow = SW_SHOWDEFAULT);

		void Hide();

		void Close();

		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		bool IsMainWindow() const { return m_IsMainWindow; }

		bool ShouldClose();

		int GetExitCode() const { return m_ExitCode; }

	private:
		bool m_IsMainWindow;
		int m_ExitCode;
	};
}
