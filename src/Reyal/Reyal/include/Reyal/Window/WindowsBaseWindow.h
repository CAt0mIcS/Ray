#ifndef RL_WINDOWSBASEWINDOW_H
#define RL_WINDOWSBASEWINDOW_H


#ifdef _WIN32

#include "Reyal/RlBase.h"

#include <Windows.h>
#include <stdint.h>
#include <string>

#pragma warning(disable : 4251)


namespace At0::Reyal
{
	struct WindowMessage
	{
		UINT uMsg;
		WPARAM wParam;
		LPARAM lParam;
	};


	template<typename DERIVED_TYPE>
	class BaseWindow
	{
	public:
		/// <summary>
		/// BaseWindow Constructor
		/// </summary>
		BaseWindow()
			: m_hWnd(0) {}

		/// <summary>
		/// Getter for the current Window Title
		/// </summary>
		/// <returns>The title of this Window</returns>
		std::wstring GetTitle() const
		{
			std::wstring str;
			int len = GetWindowTextLength(m_hWnd);
			if (len > 0)
			{
				str.resize(len);
				GetWindowText(m_hWnd, str.data(), len + 1);
			}

			return str;
		}

		/// <summary>
		/// Sets the Window Title
		/// </summary>
		/// <param name="title">Is the new Window Title</param>
		void SetTitle(const std::wstring_view title)
		{
			SetWindowText(m_hWnd, title.data());
		}

		/// <summary>
		/// Shows the Window
		/// </summary>
		void Show() const
		{
			ShowWindow(m_hWnd, SW_SHOW);
		}

		/// <summary>
		/// Hides the Window
		/// </summary>
		void Hide() const
		{
			ShowWindow(m_hWnd, SW_HIDE);
		}

		/// <summary>
		/// Maximizes the Window
		/// </summary>
		void Maximize() const
		{
			ShowWindow(m_hWnd, SW_MAXIMIZE);
		}

		/// <summary>
		/// Minimizes the Window
		/// </summary>
		void Minimize() const
		{
			ShowWindow(m_hWnd, SW_MINIMIZE);
		}

		/// <summary>
		/// Closes the Window
		/// </summary>
		void Close() const
		{
			SendMessage(m_hWnd, WM_CLOSE, 0, 0);
		}

		/// <summary>
		/// Checks if the Window is open (IsVisible)
		/// </summary>
		/// <returns>True if the Window is open, false otherwise</returns>
		bool IsOpen() const
		{
			return IsWindowVisible(m_hWnd);
		}

		/// <summary>
		/// Virtual BaseWindow Deconstructor, unregisters the window class
		/// </summary>
		virtual ~BaseWindow()
		{
			//QUESTION: Global constant for this and the amount of characters generated? (Window.cpp)
			//wchar_t className[5];
			//GetClassName(m_hWnd, className, std::size(className));
			//UnregisterClass(className, GetModuleHandle(NULL));
			UnregisterClass(m_ClassName.c_str(), GetModuleHandle(NULL));
		}

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
		/// <returns>TRUE(1) if the window was created successfully, FALSE(0) otherwise</returns>
		uint8_t CreateNativeWindow(
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
		)
		{
			m_ClassName = windowClassName;

			WNDCLASS wc{};
			wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
			wc.hInstance = GetModuleHandle(NULL);
			wc.lpszClassName = m_ClassName.c_str();
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);

			RegisterClass(&wc);

			m_hWnd = CreateWindowEx(
				exStyle, windowClassName, windowName, style, x, y, width, height,
				hWndParent, hMenu, wc.hInstance, this
			);

			return m_hWnd ? TRUE : FALSE;
		}

	private:
		/// <summary>
		/// Window procedure which calls the DERIVED_TYPE::HandleMessage function
		/// </summary>
		/// <param name="hwnd">Is the window handle to the window which should receive the event</param>
		/// <param name="uMsg">Is the message code</param>
		/// <param name="wParam">Is an additional parameter</param>
		/// <param name="lParam">Is an additional parameter</param>
		/// <returns>LRESULT code</returns>
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			DERIVED_TYPE* pDerived = nullptr;
			if (uMsg == WM_NCCREATE)
			{
				CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
				pDerived = (DERIVED_TYPE*)pCreate->lpCreateParams;
				SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pDerived);

				pDerived->m_hWnd = hWnd;
			}
			else
			{
				pDerived = (DERIVED_TYPE*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			}

			if (pDerived)
				return (LRESULT)pDerived->HandleMessage({ uMsg, wParam, lParam });
			else
				return DefWindowProc(hWnd, uMsg, wParam, lParam);

		}

	protected:
		/// <summary>
		/// Handles all window messages
		/// </summary>
		/// <param name="uMsg">Is the message code</param>
		/// <param name="wParam">Is an additional parameter</param>
		/// <param name="lParam">Is an additional parameter</param>
		/// <returns>LRESULT code</returns>
		virtual int64_t HandleMessage(const WindowMessage& msg) = 0;

		/// <summary>
		/// Getter for the window class name of this window
		/// </summary>
		/// <returns>The window class name</returns>
		const std::wstring_view GetWindowClassName() const { return m_ClassName; }

	protected:
		/// <summary>
		/// Window handle
		/// </summary>
		HWND m_hWnd;

	private:
		/// <summary>
		/// Stored to unregister class in Deconstructor
		/// <TODO>Try using GetClassName instead of storing it</TODO>
		/// </summary>
		std::wstring m_ClassName;
	};
}

#endif // _WIN32

#endif // RL_WINDOWSBASEWINDOW_H
