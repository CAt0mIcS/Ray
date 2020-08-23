#include "MainWindow.h"


#define NPE_BTN1_ID 0x1

#define NPE_WM_ZOOM (WM_USER + 1)

namespace NPE
{

    struct MousePos
    {
        int x, y;
    };
    MousePos mousePos{};


    LRESULT CALLBACK CtrlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    MainWindow::MainWindow() 
        : m_Windows{}
    {
        // WS_CLIPCHILDREN for flickering child windows
        CreateNativeWindow(L"NodePlanningEditor", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
    }

    void MainWindow::Show(_In_ int nCmdShow)
    {
        ShowWindow(m_hWnd, nCmdShow);

        WNDCLASS wc{};
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = CtrlProc;
        wc.lpszClassName = L"CUSTOMBUTTON";

        RegisterClass(&wc);

        HWND hWndButton = CreateWindow(L"CUSTOMBUTTON", L"Test", WS_CHILD | WS_VISIBLE | WS_BORDER, 200, 200, 100, 100, m_hWnd, (HMENU)NPE_BTN1_ID, NULL, NULL);
        ShowWindow(hWndButton, 1);
        m_Windows.emplace_back(hWndButton);
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
        case WM_MOUSEMOVE:
        {
            mousePos.x = GET_X_LPARAM(lParam);
            mousePos.y = GET_Y_LPARAM(lParam);
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
            //MessageBox(NULL, buffer, L"Debug", NULL);

            #define NEGATIVE 0
            #define POSITIVE 1
            if (zDelta > 0)
            {
                SendMessage(m_Windows[0], NPE_WM_ZOOM, NEGATIVE, 655370);
            }
            else
            {
                SendMessage(m_Windows[0], NPE_WM_ZOOM, POSITIVE, 655370);
            }

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

    

    struct WindowPos
    {
        int x, y, width, height;
    };
    WindowPos winPos{ 200, 200, 100, 100 };
    WindowPos CalculateNewWindowPos(HWND hWnd, WindowPos oldWindowPos, MousePos mousePos, bool scrollUp);

    LRESULT CALLBACK CtrlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc;
            RECT rect;

            GetClientRect(hWnd, &rect);

            hdc = BeginPaint(hWnd, &ps);
            SetTextColor(hdc, RGB(35, 38, 40));
            FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(255, 255, 255)));
            DrawText(hdc, L"Hello World!", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

            ReleaseDC(hWnd, hdc);
            EndPaint(hWnd, &ps);
            break;
        }
        case NPE_WM_ZOOM:
        {
            //winPos.x = winPos.x - (int)wParam / 2;
            //winPos.y = winPos.y - lParam / 2;
            //winPos.width = winPos.width - (int)wParam / 2;
            //winPos.height = winPos.height - lParam / 2;
            //
            //LPARAM lParamTest = 0;
            //WORD hiWord;
            //WORD loWord;
            //while (HIWORD(lParamTest) != -10 || LOWORD(lParamTest) != -10)
            //{
            //    //Both HIWORD and LOWORD are  10 if lParam is 655370
            //    ++lParamTest;
            //    hiWord = HIWORD(lParamTest);
            //    loWord = LOWORD(lParamTest);
            //}

            /*
            if x == 200 && y = 200 && width == 100 && height == 100 and mouse cursor on the center of the widget
                *********
                *       *
                *       *
                *********
                
                Resize each corner by 5 -> x = 195, y = 195, width = 110, height = 110
               ***********
               *         *
               *         *
               *         *
               ***********
            */

            if (wParam == 1)
            {
                //winPos.x        -= (int)HIWORD(lParam) / 2;
                //winPos.y        -= (int)LOWORD(lParam) / 2;
                //winPos.width    -= (int)HIWORD(lParam) / 2;
                //winPos.height   -= (int)LOWORD(lParam) / 2;
                winPos = CalculateNewWindowPos(hWnd, winPos, mousePos, false);
            }
            else if(wParam == 0)
            {
                //winPos.x        += (int)HIWORD(lParam) / 2;
                //winPos.y        += (int)LOWORD(lParam) / 2;
                //winPos.width    += (int)HIWORD(lParam) / 2;
                //winPos.height   += (int)LOWORD(lParam) / 2;
                winPos = CalculateNewWindowPos(hWnd, winPos, mousePos, true);
            }
            //wchar_t buffer[256];
            //wsprintfW(buffer, L"Delta:%d\nxPos:%d\nyPos:%d", 0, mousePos.x, mousePos.y);
            //MessageBox(NULL, buffer, buffer, NULL);
            

            MoveWindow(hWnd, winPos.x, winPos.y, winPos.width, winPos.height, TRUE);
            break;
        }
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }

        return TRUE;
    }

    WindowPos CalculateNewWindowPos(HWND hWnd, WindowPos oldWindowPos, MousePos mousePos, bool scrollUp)
    {
        //Check if mouse is on the left, right, bottom, top side of the widget

        WindowPos pos{};

        bool onRight = false;
        bool onLeft = false;
        bool bellow = false;
        bool above = false;

        int widgetCenterX = (winPos.x + winPos.x + winPos.width) / 2;
        int widgetCenterY = (winPos.y + winPos.y + winPos.height) / 2;

        //if (mousePos.x > oldWindowPos.x)
        //{
        //    onRight = true;
        //}
        //else if (mousePos.x < oldWindowPos.x)
        //{
        //    onLeft = true;
        //}
        //if (mousePos.y > oldWindowPos.y)
        //{
        //    bellow = true;
        //}
        //else if (mousePos.y < oldWindowPos.y)
        //{
        //    above = true;
        //}

        if (mousePos.x > widgetCenterX)
        {
            onRight = true;
        }
        else if (mousePos.x < widgetCenterX)
        {
            onLeft = true;
        }
        if (mousePos.y > widgetCenterY)
        {
            bellow = true;
        }
        else if (mousePos.y < widgetCenterY)
        {
            above = true;
        }


        if (onRight && above)
        {
            if (scrollUp)
            {
                pos.x = oldWindowPos.x - (50 * (mousePos.x - widgetCenterX) / 500);
                pos.y = oldWindowPos.y - (50 * (mousePos.y - widgetCenterY) / 500);

                pos.width = oldWindowPos.width + (50 * (mousePos.x - widgetCenterX) / 500);
                pos.height = oldWindowPos.height - (50 * (mousePos.y - widgetCenterY) / 500);
            }
            else
            {
                pos.x = oldWindowPos.x + (50 * (mousePos.x - widgetCenterX) / 500);
                pos.y = oldWindowPos.y + (50 * (mousePos.y - widgetCenterY) / 500);

                pos.width = oldWindowPos.width - (50 * (mousePos.x - widgetCenterX) / 500);
                pos.height = oldWindowPos.height + (50 * (mousePos.y - widgetCenterY) / 500);
            }
        }
        else if (onRight && bellow)
        {
            if (scrollUp)
            {
                pos.x = oldWindowPos.x - (50 * (mousePos.x - widgetCenterX) / 500);
                pos.y = oldWindowPos.y - (50 * (mousePos.y - widgetCenterY) / 500);

                pos.width = oldWindowPos.width + (50 * (mousePos.x - widgetCenterX) / 500);
                pos.height = oldWindowPos.height + (50 * (mousePos.y - widgetCenterY) / 500);
            }
            else
            {
                pos.x = oldWindowPos.x + (50 * (mousePos.x - widgetCenterX) / 500);
                pos.y = oldWindowPos.y + (50 * (mousePos.y - widgetCenterY) / 500);

                pos.width = oldWindowPos.width - (50 * (mousePos.x - widgetCenterX) / 500);
                pos.height = oldWindowPos.height - (50 * (mousePos.y - widgetCenterY) / 500);
            }
        }
        else if (onLeft && above)
        {
            if (scrollUp)
            {
                pos.x = oldWindowPos.x + (50 * 100 / 500);
                pos.y = oldWindowPos.y + (50 * 100 / 500);
            }
            else
            {
                pos.x = oldWindowPos.x - (50 * 100 / 500);
                pos.y = oldWindowPos.y - (50 * 100 / 500);
            }
        }
        else if (onLeft && bellow)
        {
            if (scrollUp)
            {
                pos.x = oldWindowPos.x + (50 * 100 / 500);
                pos.y = oldWindowPos.y - (50 * 100 / 500);
            }
            else
            {
                pos.x = oldWindowPos.x - (50 * 100 / 500);
                pos.y = oldWindowPos.y + (50 * 100 / 500);
            }
        }


        //if (scrollUp)
        //{
        //    pos.width = oldWindowPos.width + (25 * 100 / 500);
        //    pos.height = oldWindowPos.height + (25 * 100 / 500);
        //}
        //else
        //{
        //    pos.width = oldWindowPos.width - (25 * 100 / 500);
        //    pos.height = oldWindowPos.height - (25 * 100 / 500);
        //}

        return pos;
    }

}


