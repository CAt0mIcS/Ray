#pragma once

#include "BaseWindow.h"


namespace Zeal::Reyal
{
	class RL_API Window : public BaseWindow<Window>
	{
	public:
		/// <summary>
		/// Window Constructor
		/// </summary>
		/// <param name="windowTitle">Is the window title</param>
		/// <param name="isMainWindow">Specifies whether this window is the main one which will exit the application when closed</param>
		Window(const std::wstring& windowTitle, bool isMainWindow = false);

		/// <summary>
		/// Window Deconstructor
		/// </summary>
		~Window();

		/// <summary>
		/// Handles all window messages
		/// </summary>
		/// <param name="uMsg">Is the message code</param>
		/// <param name="wParam">Is an additional parameter</param>
		/// <param name="lParam">Is an additional parameter</param>
		/// <returns>LRESULT code</returns>
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		/// <summary>
		/// Checks if this window is the main Window
		/// </summary>
		/// <returns>True if it's the main Window, false otherwise</returns>
		bool IsMainWindow() const { return m_IsMainWindow; }

		/// <summary>
		/// Shows the window
		/// </summary>
		/// <param name="cmdShow">Is a flag to set how the window should be shown</param>
		void Show(_In_opt_ uint8_t cmdShow = SW_SHOWDEFAULT) const;

		/// <summary>
		/// Hides the Window
		/// </summary>
		void Hide() const;

		/// <summary>
		/// Sends a WM_CLOSE message to the window, thus closing it
		/// </summary>
		void Close() const;

		/// <summary>
		/// Checks if a window is open using the Window Class name
		/// </summary>
		/// <returns>True if the Window is open, false otherwise</returns>
		bool IsOpen() const;

		/// <summary>
		/// Will return false as long as the window hasn't been closed
		/// </summary>
		/// <returns>True if the window was closed, false otherwise</returns>
		bool ShouldClose();

		/// <summary>
		/// Gets the exit code which is set when the main Window was closed
		/// </summary>
		/// <returns></returns>
		int GetExitCode() const { return m_ExitCode; }

	private:
		bool m_IsMainWindow;
		int m_ExitCode;
	};
}
