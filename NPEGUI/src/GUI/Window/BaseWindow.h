#pragma once

#include "Win.h"


namespace GUI
{
    template <class DERIVED_TYPE>
    class BaseWindow
    {
    public:
        /// <summary>
        /// Getter for window handle
        /// </summary>
        /// <returns>A handle to this window</returns>
        HWND GetNativeWindow() const { return m_hWnd; }

        /// <summary>
        /// Virtual BaseWindow Deconstructor
        /// </summary>
        virtual ~BaseWindow()
        {
            UnregisterClass(s_MainWindowClassName, NULL);
        }

    protected:
        /// <summary>
        /// Window procedure which calls the DERIVED_TYPE::HandleMessage function
        /// </summary>
        /// <param name="hwnd">Is the window handle to the window which should receive the event</param>
        /// <param name="uMsg">Is the message code</param>
        /// <param name="wParam">Is an additional parameter</param>
        /// <param name="lParam">Is an additional parameter</param>
        /// <returns>LRESULT code</returns>
        static LRESULT CALLBACK WindowProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
        {
            DERIVED_TYPE* pThis = nullptr;

            if (uMsg == WM_NCCREATE)
            {
                CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
                pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
                SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

                pThis->m_hWnd = hwnd;
            }
            else
            {
                pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            }
            if (pThis)
            {
                return pThis->HandleMessage(uMsg, wParam, lParam);
            }
            else
            {
                return DefWindowProc(hwnd, uMsg, wParam, lParam);
            }
        }

        /// <summary>
        /// BaseWindow Constructor
        /// </summary>
        BaseWindow() : m_hWnd(NULL) { }

        /// <summary>
        /// Creates the native window
        /// </summary>
        /// <param name="lpWindowName">Is the title of the window</param>
        /// <param name="dwStyle">Are window styles</param>
        /// <param name="dwExStyle">Are extended window styles</param>
        /// <param name="x">Is the x-position of the window</param>
        /// <param name="y">Is the y-position of the window</param>
        /// <param name="nWidth">Is the width of the window</param>
        /// <param name="nHeight">Is the height of the window</param>
        /// <param name="hWndParent">Is a handle to the parent window</param>
        /// <param name="hMenu">Is a window Id</param>
        /// <returns>TRUE(1) if the window was created successfully, FALSE(0) otherwise</returns>
        BOOL CreateNativeWindow(
            _In_ PCWSTR lpWindowName,
            _In_ DWORD dwStyle,
            _In_opt_ _Maybenull_ DWORD dwExStyle = 0,
            _In_opt_ int x = CW_USEDEFAULT,
            _In_opt_ int y = CW_USEDEFAULT,
            _In_opt_ int nWidth = CW_USEDEFAULT,
            _In_opt_ int nHeight = CW_USEDEFAULT,
            _In_opt_ _Maybenull_ HWND hWndParent = 0,
            _In_opt_ _Maybenull_ HMENU hMenu = 0
        )
        {
            WNDCLASS wc = { 0 };

            wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
            wc.hInstance = GetModuleHandle(NULL);
            wc.lpszClassName = s_MainWindowClassName;

            RegisterClass(&wc);

            m_hWnd = CreateWindowEx(
                dwExStyle, s_MainWindowClassName, lpWindowName, dwStyle, x, y,
                nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
            );

            return (m_hWnd ? TRUE : FALSE);
        }

        /// <summary>
        /// Handles all window messages
        /// </summary>
        /// <param name="uMsg">Is the message code</param>
        /// <param name="wParam">Is an additional parameter</param>
        /// <param name="lParam">Is an additional parameter</param>
        /// <returns>LRESULT code</returns>
        virtual LRESULT HandleMessage(_In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) = 0;

    protected:
        /// <summary>
        /// Window handle
        /// </summary>
        HWND m_hWnd;

        /// <summary>
        /// Name of the window class
        /// </summary>
        static constexpr const wchar_t* s_MainWindowClassName = L"NPE_MAINWINDOWCLASS";
    };
}

