#include "MainWindow.h"


namespace NPE
{
    MainWindow::MainWindow()
    {
        CreateNativeWindow(L"NodePlanningEditor", WS_OVERLAPPEDWINDOW);
    }

    void MainWindow::Show(_In_ int nCmdShow)
    {
        ShowWindow(m_hWnd, nCmdShow);
    }

    void MainWindow::EnterMessageLoop()
    {
        MSG msg{};
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return FALSE;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(m_hWnd, &ps);

            Paint(hdc, &ps.rcPaint, TRUE);
            
            EndPaint(m_hWnd, &ps);
            ReleaseDC(m_hWnd, hdc);
            break;
        }
        case WM_RBUTTONDOWN:
        {
            HDC hdc = GetDC(m_hWnd);
            wchar_t buffer[65];

            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            wsprintfW(buffer, L"%d", x);
            TextOut(hdc, 100, 100, buffer, 5);

            wsprintfW(buffer, L"%d", y);
            TextOut(hdc, 100, 130, buffer, 5);
            break;
        }
        case WM_MOUSEWHEEL:
        {
            //int fwKeys = GET_KEYSTATE_WPARAM(wParam);     //To check if some keys (Control/Shift/...) are pressed
            int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);

            wchar_t buffer[256];
            wsprintfW(buffer, L"Delta:%d\nxPos:%d\nyPos:%d", zDelta, xPos, yPos);
            MessageBox(NULL, buffer, L"Debug", NULL);
            break;
        }
        default:
            return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
        }

        return TRUE;
    }

    void MainWindow::Paint(HDC hDC, RECT* rcDirty, BOOL bErase)
    {
        FillRect(hDC, rcDirty, CreateSolidBrush(RGB(35, 38, 40)));
    }
}


