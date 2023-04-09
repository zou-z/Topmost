#include "MainWindow.h"
#include "resource.h"

namespace View
{
    MainWindow* MainWindow::instance;

    MainWindow::MainWindow()
    {
        instance = this;
        className = L"Topmost";
        title = L"Topmost Indication Window";
        hInstance = NULL;
        hWnd = NULL;
        windowListUtil = new Util::WindowListUtil();
        startMenuUtil = new Util::StartMenuUtil();
        notifyIcon = new View::NotifyIcon();
        helpWindow = new View::HelpWindow();
    }

    MainWindow::~MainWindow()
    {
        UnregisterKey();
        instance = nullptr;
        hInstance = NULL;
        hWnd = NULL;
        delete windowListUtil;
        delete startMenuUtil;
        delete notifyIcon;
        delete helpWindow;
    }

    DWORD MainWindow::Initialization(HINSTANCE hInstance)
    {
        this->hInstance = hInstance;
        DWORD errorCode = S_OK;
        if ((errorCode = RegisterWindowClass(hInstance)) != S_OK) return errorCode;
        if ((errorCode = CreateInstance(hInstance)) != S_OK) return errorCode;
        if ((errorCode = RegisterKey()) != S_OK)return errorCode;
        notifyIcon->ShowNotifyIcon(hInstance, hWnd);
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
        hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOOLWINDOW, className.c_str(), title.c_str(), WS_POPUP, 0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);
        if (hWnd == NULL)
        {
            return GetLastError();
        }
        ShowWindow(hWnd, SW_HIDE);
        UpdateWindow(hWnd);
        SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255), 0, LWA_COLORKEY);
        return S_OK;
    }

    LRESULT MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_COMMAND:
            switch (wParam)
            {
            case NotifyStartMenuAddId:
                if (instance->startMenuUtil->AddToStartMenu())
                {
                    MessageBox(NULL, L"添加成功", AppName, MB_ICONINFORMATION | MB_OK);
                }
                else
                {
                    MessageBox(NULL, L"添加失败", AppName, MB_ICONERROR | MB_OK);
                }
                break;
            case NotifyStartMenuRemoveId:
                if (instance->startMenuUtil->RemoveFromStartMenu())
                {
                    MessageBox(NULL, L"移除成功", AppName, MB_ICONINFORMATION | MB_OK);
                }
                else
                {
                    MessageBox(NULL, L"移除失败", AppName, MB_ICONERROR | MB_OK);
                }
                break;
            case NotifyHelpId:
            {
                DWORD result = S_OK;
                if ((result = instance->helpWindow->Show(instance->hInstance)) != S_OK)
                {
                    instance->ShowErrorMessage(L"Open Help Window Failed", result);
                }
                break;
            }
            case NotifyExitId:
            {
                instance->notifyIcon->RemoveNotifyIcon(hWnd);
                DestroyWindow(hWnd);
                break;
            }
            default:
                break;
            }
            break;
        case WM_HOTKEY:
			if (instance != nullptr && wParam == instance->hotKeyId)
			{
                OutputDebugString(((std::wstring)AppName + L": Pressed Hot Key\r\n").c_str());
                instance->SetWindowTopmost();
			}
            break;
        case WM_TIMER:
            ShowWindow(hWnd, SW_HIDE);
            break;
        case NotifyMessageId:
            instance->ShowNotifyMenu(lParam, hWnd);
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

    void MainWindow::SetWindowTopmost()
    {
        // 获取鼠标位置
        POINT point;
        if (GetCursorPos(&point) == FALSE)
        {
            instance->ShowErrorMessage(L"Get Cursor Position Failed", GetLastError());
            return;
        }

        // 刷新窗口列表
        DWORD result = S_OK;
        if ((result = instance->windowListUtil->RefreshWindowList()) != S_OK)
        {
            instance->ShowErrorMessage(L"Refresh Window List Failed", result);
            return;
        }

        // 找到鼠标位置处的窗口，置顶/取消置顶窗口
        Util::WindowInfo windowInfo;
        if (instance->windowListUtil->FindWindowByCursorPosition(point.x, point.y, windowInfo))
        {
            if ((result = instance->windowListUtil->SetWindowTopmost(windowInfo.Handle, windowInfo.IsTopMost)) != S_OK)
            {
                instance->ShowErrorMessage(L"Set Window Topmost Failed", result);
                return;
            }

            auto width = windowInfo.Right - windowInfo.Left;
            auto height = windowInfo.Bottom - windowInfo.Top;
            // 清除绘制
            InvalidateRect(hWnd, NULL, true);
            // 重置窗口位置
            if ((result = SetWindowPos(hWnd, HWND_TOPMOST, windowInfo.Left, windowInfo.Top, width, height, SWP_SHOWWINDOW)) == FALSE)
            {
                instance->ShowErrorMessage(L"Set Window Position Failed", GetLastError());
            }
            ShowWindow(hWnd, SW_SHOW);
            SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            // 绘制窗口边框
            if (windowInfo.IsTopMost)
            {
                instance->RedrawWindowBorder(width, height, RGB(127, 127, 127));
            }
            else
            {
                instance->RedrawWindowBorder(width, height, RGB(255, 97, 0));
            }
            // 启动定时器
            if (SetTimer(hWnd, instance->timerId, instance->timerInterval, NULL) == FALSE)
            {
                ShowWindow(hWnd, SW_HIDE);
                instance->ShowErrorMessage(L"Set Timer Failed", GetLastError());
            }
        }
    }

    void MainWindow::ShowNotifyMenu(LPARAM lParam, HWND hWnd)
    {
        DWORD result = S_OK;
        switch (lParam)
        {
        case WM_LBUTTONDOWN:
            break;
        case WM_RBUTTONDOWN:
            if ((result = notifyIcon->ShowNotifyMenu(hWnd)) != S_OK)
            {
                ShowErrorMessage(L"Show Notify Menu Failed", result);
            }
            break;
        default:
            break;
        }
    }

    void MainWindow::RedrawWindowBorder(long width, long height, COLORREF color)
    {
        int thickness = 3;
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
        std::wstring text = message + L", Error Code: " + std::to_wstring(errorCode);
        OutputDebugString(((std::wstring)AppName + text + L"\r\n").c_str());
        MessageBox(NULL, text.c_str(), AppName, MB_ICONERROR | MB_OK);
    }
}
