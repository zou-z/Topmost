#include <Windows.h>
#include "MainWindow.h"
#include "resource.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // ����Ƿ������������ʵ��
    HANDLE mutex = CreateMutex(NULL, TRUE, ((std::wstring)AppName + L"_SingleInstance").c_str());
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBox(NULL, L"���������У������ٴ�����", AppName, MB_ICONINFORMATION | MB_OK);
        if (mutex != NULL)
        {
            CloseHandle(mutex);
        }
        return 0;
    }

    // ��ʼ��������
    DWORD errorCode = S_OK;
    auto mainWindow = new View::MainWindow();
    if ((errorCode = mainWindow->Initialization(hInstance)) != S_OK)
    {
        OutputDebugString(((std::wstring)AppName + L": Main Window Initialization Failed, Error Code:" + std::to_wstring(errorCode)).c_str());
        return FALSE;
    }

    // ����Ϣѭ��
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // �ͷ��ڴ�
    delete mainWindow;
    if (mutex != NULL)
    {
        CloseHandle(mutex);
    }
    return (int)msg.wParam;
}
