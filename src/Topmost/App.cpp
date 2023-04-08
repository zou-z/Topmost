#include <Windows.h>
#include "MainWindow.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 初始化主窗口
    DWORD errorCode = S_OK;
    auto mainWindow = new App::MainWindow();
    if ((errorCode = mainWindow->Initialization(hInstance)) != S_OK)
    {
        OutputDebugString((L"Main Window Initialization Failed, Error Code:" + std::to_wstring(errorCode)).c_str());
        return FALSE;
    }

    // 主消息循环
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    delete mainWindow;
    return (int)msg.wParam;
}
