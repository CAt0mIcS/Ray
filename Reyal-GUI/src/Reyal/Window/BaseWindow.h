#pragma once

#include "RlBase.h"
#include "RlWin.h"

#include <stdint.h>


namespace Zeal::Reyal
{
	template<typename DERIVED_TYPE>
	class RL_API BaseWindow
	{
	public:
		/// <summary>
		/// BaseWindow Constructor
		/// </summary>
		BaseWindow()
			: m_hWnd(0) {}

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
			_In_ wchar_t* windowName,
			_In_ wchar_t* windowClassName,
			_In_ DWORD style,
			_In_opt_ _Maybenull_ DWORD exStyle = 0,
			_In_opt_ int x = CW_USEDEFAULT,
			_In_opt_ int y = CW_USEDEFAULT,
			_In_opt_ int width = CW_USEDEFAULT,
			_In_opt_ int height = CW_USEDEFAULT,
			_In_opt_ _Maybenull_ HWND hWndParent = 0,
			_In_opt_ _Maybenull_ HMENU hMenu = 0
		)
		{
			WNDCLASS wc{};
			wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
			wc.hInstance = GetModuleHandle(L"Reyal"); //TODO: Test if that works
			wc.lpszClassName = windowClassName;

			RegisterClass(&wc);

			n_hWnd = CreateWindowEx(
				exStyle, windowClassName, windowName, style, x, y, width, height,
				hWndParent, hMenu, GetModuleHandle(L"Reyal"), this
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
			}
			else
			{
				pDerived = GetWindowLongPtr(hWnd, GWLP_USERDATA);
			}

			if (pDerived)
				return pDerived->HandleMessage(uMsg, wParam, lParam);
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
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	protected:
		/// <summary>
		/// Window handle
		/// </summary>
		HWND m_hWnd;
	};
}



