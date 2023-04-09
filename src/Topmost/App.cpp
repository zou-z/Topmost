#include <Windows.h>
#include "MainWindow.h"
#include "resource.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 检查是否存在其他程序实例
    HANDLE mutex = CreateMutex(NULL, TRUE, ((std::wstring)AppName + L"_SingleInstance").c_str());
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBox(NULL, L"程序已运行，无需再次启动", AppName, MB_ICONINFORMATION | MB_OK);
        if (mutex != NULL)
        {
            CloseHandle(mutex);
        }
        return 0;
    }

    // 初始化主窗口
    DWORD errorCode = S_OK;
    auto mainWindow = new View::MainWindow();
    if ((errorCode = mainWindow->Initialization(hInstance)) != S_OK)
    {
        OutputDebugString(((std::wstring)AppName + L": Main Window Initialization Failed, Error Code:" + std::to_wstring(errorCode)).c_str());
        return FALSE;
    }

    // 主消息循环
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 释放内存
    delete mainWindow;
    if (mutex != NULL)
    {
        CloseHandle(mutex);
    }
    return (int)msg.wParam;
}
