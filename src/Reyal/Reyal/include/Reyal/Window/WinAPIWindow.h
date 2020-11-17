#ifndef RL_WINAPIWINDOW_H
#define RL_WINAPIWINDOW_H

#include "RlBase.h"
#include "Window.h"

#include <Windows.h>

#include <../../RlRender/include/RlRender/TypeDefines.h>


namespace At0::Reyal
{
	class RL_API WinAPIWindow : public Window
	{
	public:
		/// <summary>
		/// Window Constructor
		/// </summary>
		/// <param name="name">Is the unique name of the window</param>
		/// <param name="parent">Is the parent of this window</param>
		/// <param name="isMainWindow">Specifies whether this window is the main one which will exit the application when closed</param>
		WinAPIWindow(const std::string_view name, Widget* parent = nullptr, bool isMainWindow = false);
		
		/// <summary>
		/// Window Deconstructor
		/// </summary>
		~WinAPIWindow();

		/// <summary>
		/// Shows the Window
		/// </summary>
		virtual void Show() override;

		/// <summary>
		/// Sets the Window Title
		/// </summary>
		/// <param name="title">Is the new Title of the Window</param>
		virtual void SetTitle(const std::string_view title) override;

		/// <summary>
		/// Will return false as long as the window hasn't been closed
		/// </summary>
		/// <param name="exitCode">Will be filled with the exit code when the application quits, specify nullptr to ignore</param>
		/// <returns>True if the window was closed, false otherwise</returns>
		bool ShouldClose() override;
	private:
		/// <summary>
		/// Creates the native window
		/// </summary>
		/// <param name="windowName">Is the title of the window</param>
		/// <param name="windowClassName">Is the window class name</param>
		/// <param name="style">Are window styles</param>
		/// <param name="exStyle">Are extended window styles</param>
		/// <param name="x">Is the x-position of the window</param>
		/// <param name="y">Is the y-position of the window</param>
		/// <param name="width">Is the width of the window</param>
		/// <param name="height">Is the height of the window</param>
		/// <param name="hWndParent">Is a handle to the parent window</param>
		/// <param name="hMenu">Is a window Id</param>
		/// <returns>true if the window was created successfully, false otherwise</returns>
		bool CreateNativeWindow(
			const wchar_t* windowName,
			const wchar_t* windowClassName,
			DWORD style,
			DWORD exStyle = 0,
			uint32_t x = CW_USEDEFAULT,
			uint32_t y = CW_USEDEFAULT,
			uint32_t width = CW_USEDEFAULT,
			uint32_t height = CW_USEDEFAULT,
			HWND hWndParent = 0,
			HMENU hMenu = 0
		);

		/// <summary>
		/// Passes the message to HandleMessage
		/// </summary>
		/// <param name="hWnd">Is the Window Handle which received the message</param>
		/// <param name="uMsg">Is the message code</param>
		/// <param name="wParam">Is an additional parameter</param>
		/// <param name="lParam">Is an additional parameter</param>
		/// <returns>LRESULT code</returns>
		static LRESULT WindowProcSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		/// <summary>
		/// Handles all window messages
		/// </summary>
		/// <param name="uMsg">Is the message code</param>
		/// <param name="wParam">Is an additional parameter</param>
		/// <param name="lParam">Is an additional parameter</param>
		/// <returns>LRESULT code</returns>
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		/// <summary>
		/// Loops over all children and figures out if we need to send a HoverEnter/HoverLeave Event
		/// </summary>
		void SetHoveringWidget();

	private:
		/// <summary>
		/// Specifies the control where the mouse is currently on
		/// </summary>
		Widget* m_CurrentlyHovering;

		/// <summary>
		/// Specifies the old size of this window
		/// </summary>
		Size2 m_OldSize;

		/// <summary>
		/// Specifies the old position of this window
		/// </summary>
		Point2 m_OldPos;
	};
}




#endif // RL_WINAPIWINDOW_H