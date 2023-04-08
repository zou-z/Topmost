#include "MainWindow.h"
#include "resource.h"

namespace App
{
    MainWindow* MainWindow::instance;

    MainWindow::MainWindow()
    {
        instance = this;
        className = L"Topmost";
        title = L"Topmost Indication Window";
        hWnd = NULL;
        windowListUtil = new Util::WindowListUtil();
    }

    MainWindow::~MainWindow()
    {
        UnregisterKey();
        instance = nullptr;
        delete windowListUtil;
    }

    DWORD MainWindow::Initialization(HINSTANCE hInstance)
    {
        DWORD errorCode = S_OK;
        if ((errorCode = RegisterWindowClass(hInstance)) != S_OK) return errorCode;
        if ((errorCode = CreateInstance(hInstance)) != S_OK) return errorCode;
        if ((errorCode = RegisterKey()) != S_OK)return errorCode;
        return S_OK;
    }

    DWORD MainWindow::RegisterWindowClass(HINSTANCE hInstance)
    {
        WNDCLASSEXW wcex{};
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = MainWindow::WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_ICON1);
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = className.c_str();
        wcex.hIconSm = LoadIcon(hInstance, (LPCTSTR)IDI_ICON1);

        if (RegisterClassExW(&wcex) == FALSE)
        {
            return GetLastError();
        }
        return S_OK;
    }

    DWORD MainWindow::CreateInstance(HINSTANCE hInstance)
    {
        hWnd = CreateWindowEx(WS_EX_LAYERED, className.c_str(), title.c_str(), WS_POPUP, 0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);
        if (hWnd == NULL)
        {
            return GetLastError();
        }

        //ShowWindow(hWnd, SW_HIDE);
        ShowWindow(hWnd, SW_NORMAL);
        UpdateWindow(hWnd);
        SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255), 0, LWA_COLORKEY);
        return S_OK;
    }

    LRESULT MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
            //     case WM_COMMAND:
                     //int wmId = LOWORD(wParam);
                     //// 分析菜单选择:
                     //switch (wmId)
                     //{
                     //case IDM_ABOUT:
                     //	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                     //	break;
                     //case IDM_EXIT:
                     //	DestroyWindow(hWnd);
                     //	break;
                     //default:
                     //	return DefWindowProc(hWnd, message, wParam, lParam);
                     //}
                     //break;
        case WM_HOTKEY:
            if (wParam == instance->hotKeyId)
            {
                OutputDebugString(L"-- Pressed Hot Key--\r\n");
                if (instance != nullptr)
                {
                    POINT point;
                    if (GetCursorPos(&point) == FALSE)
                    {
                        instance->ShowErrorMessage(L"Get Cursor Position Failed", GetLastError());
                        return 0;
                    }

                    DWORD result = S_OK;
                    if ((result = instance->windowListUtil->RefreshWindowList()) != S_OK)
                    {
                        instance->ShowErrorMessage(L"Refresh Window List Failed", result);
                        return 0;
                    }

                    Util::WindowInfo windowInfo;
                    if (instance->windowListUtil->FindWindowByCursorPosition(point.x, point.y, windowInfo))
                    {
                        if ((result = instance->windowListUtil->SetWindowTopmost(windowInfo.Handle)) != S_OK)
                        {
                            instance->ShowErrorMessage(L"Set Window Topmost Failed", result);
                            return 0;
                        }

                        auto width = windowInfo.Right - windowInfo.Left;
                        auto height = windowInfo.Bottom - windowInfo.Top;
                        // 清除绘制
                        InvalidateRect(hWnd, NULL, true);
                        // 重置窗口位置
                        SetWindowPos(hWnd, HWND_TOPMOST, windowInfo.Left, windowInfo.Top, width, height, SWP_SHOWWINDOW);
                        // draw main window border with corresponding color
                        instance->RedrawWindowBorder(width, height, RGB(128, 128, 128));
                        // start hide window timer


                    }
                }
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }

    DWORD MainWindow::RegisterKey()
    {
        // Ctrl + Shift + `
        if (RegisterHotKey(hWnd, hotKeyId, MOD_CONTROL | MOD_SHIFT, VK_OEM_3) == FALSE)
        {
            return GetLastError();
        }
        return S_OK;
    }

    DWORD MainWindow::UnregisterKey()
    {
        if (UnregisterHotKey(hWnd, hotKeyId) == FALSE)
        {
            return GetLastError();
        }
        return S_OK;
    }

    void MainWindow::RedrawWindowBorder(long width, long height, COLORREF color)
    {
        int thickness = 5;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HPEN pen = CreatePen(PS_SOLID, thickness, color);
        SelectObject(hdc, pen);
        Rectangle(hdc, thickness / 2, thickness / 2, width - thickness / 2, height - thickness / 2);
        DeleteObject(pen);
        EndPaint(hWnd, &ps);
        DeleteDC(hdc);
    }

    void MainWindow::ShowErrorMessage(std::wstring message, DWORD errorCode)
    {
        std::wstring text = message + std::to_wstring(errorCode);
        OutputDebugString((text + L"\r\n").c_str());



    }
}
