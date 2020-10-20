#pragma once

#include "BaseWindow.h"

#include "Reyal/Input/Keyboard.h"
#include "Reyal/Input/Mouse.h"

#include <functional>


namespace Zeal::Reyal
{
	class Event;

	/// <summary>
	/// Receives all events
	/// <param name="receiver">Is the Widget which receives the Event</param>
	/// <param name="e">Is the received Event</param>
	/// <returns>True if the event was handled by the client, false if the event should be handled by the Window</returns>
	/// </summary>
	using EventCallbackFn = std::function<bool(ZWidget* receiver, Event& e)>;

	class RL_API Window : public BaseWindow<Window>
	{
	public:
		/// <summary>
		/// From https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
		/// Controls how the window is to be shown
		/// </summary>
		enum class ShowCommand : uint8_t
		{
			/// <summary>
			/// Hides the window and activates another window.
			/// </summary>
			Hide		= SW_HIDE,

			/// <summary>
			/// Activates and displays a window. If the window is minimized or maximized,
			///  the system restores it to its original size and position. 
			/// An application should specify this flag when displaying the window for the first time.
			/// </summary>
			Normal		= SW_SHOWNORMAL,

			/// <summary>
			/// Activates the window and displays it as a minimized window.
			/// </summary>
			Minimized	= SW_SHOWMINIMIZED,

			/// <summary>
			/// Activates the window and displays it as a maximized window.
			/// </summary>
			Maximized	= SW_SHOWMAXIMIZED,

			/// <summary>
			/// Maximizes the specified window.
			/// </summary>
			Maximize	= SW_MAXIMIZE,

			/// <summary>
			/// Activates the window and displays it in its current size and position.
			/// </summary>
			Show		= SW_SHOW,

			/// <summary>
			/// Minimizes the specified window and activates the next top-level window in the Z order.
			/// </summary>
			Minimize	= SW_MINIMIZE,

			/// <summary>
			/// Activates and displays the window. If the window is minimized or maximized, the
			///  system restores it to its original size and position. 
			/// An application should specify this flag when restoring a minimized window.
			/// </summary>
			Restore		= SW_RESTORE,

			/// <summary>
			/// Sets the show state based on the SW_ value specified in the STARTUPINFO structure
			/// passed to the CreateProcess function by the program that started the application.
			/// </summary>
			Default		= SW_SHOWDEFAULT
		};
	public:
		/// <summary>
		/// Window Constructor
		/// </summary>
		/// <param name="windowTitle">Is the window title</param>
		/// <param name="isMainWindow">Specifies whether this window is the main one which will exit the application when closed</param>
		Window(const std::wstring_view windowTitle, bool isMainWindow = false);

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
		virtual LRESULT HandleMessage(_In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) override;

		/// <summary>
		/// Checks if this window is the main Window
		/// </summary>
		/// <returns>True if it's the main Window, false otherwise</returns>
		bool IsMainWindow() const { return m_IsMainWindow; }

		/// <summary>
		/// Shows the window
		/// </summary>
		/// <param name="cmdShow">Is a flag to set how the window should be shown</param>
		void Show(_In_opt_ ShowCommand cmdShow = ShowCommand::Default) const;

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

		/// <summary>
		/// Sets the function which will receive events
		/// </summary>
		/// <typeparam name="F">Is any callable, to EventCallbackFn castable function pointer</typeparam>
		/// <param name="func">Is the function which will receive all events</param>
		template<typename F, typename = std::enable_if_t<std::is_invocable_v<F, ZWidget*, Event&>>, typename = std::enable_if_t<std::is_convertible_v<F, EventCallbackFn>>>
		void SetEventCallback(F&& func) { m_CallbackFunc = func; }

	public:
		Keyboard Keyboard;
		Mouse Mouse;

	private:
		EventCallbackFn m_CallbackFunc;
		bool m_IsMainWindow;
		int m_ExitCode;
	};
}
