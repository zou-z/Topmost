#include "NotifyIcon.h"
#include "resource.h"
#include <strsafe.h>

namespace View
{
	NotifyIcon::NotifyIcon()
	{
	}

	NotifyIcon::~NotifyIcon()
	{
	}

	void NotifyIcon::ShowNotifyIcon(HINSTANCE hInstance, HWND hWnd)
	{
		NOTIFYICONDATA data = {};
		data.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_ICON1);  // 设置图标
		StringCchCopy(data.szTip, ARRAYSIZE(data.szTip), L"Test application");  // 设置提示文本
		data.hWnd = hWnd;  // 设置关联的窗口
		data.uID = notifyId;  // 设置id
		data.uFlags = NIF_GUID | NIF_ICON | NIF_MESSAGE | NIF_TIP;  // 设置类型
		data.uCallbackMessage = NotifyMessageId;  // 设置关联的消息id
		Shell_NotifyIcon(NIM_ADD, &data);
	}

    DWORD NotifyIcon::ShowNotifyMenu(HWND hWnd)
	{
        POINT point;
        if (GetCursorPos(&point) == FALSE)
        {
            return GetLastError();
        }
        HMENU hMenu;
        hMenu = CreatePopupMenu();
        if (hMenu == NULL)
        {
            return GetLastError();
        }
        if (AppendMenu(hMenu, MF_STRING, NotifyHelpId, NotifyHelpText) == FALSE)
        {
            return GetLastError();
        }
        if (AppendMenu(hMenu, MF_STRING, NotifyExitId, NotifyExitText) == FALSE)
        {
            return GetLastError();
        }
        SetForegroundWindow(hWnd);  // 让HMENU自动关闭
        if (TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, NULL, hWnd, NULL) == FALSE)
        {
            return GetLastError();
        }
        DestroyMenu(hMenu);
        return S_OK;
	}

    void NotifyIcon::RemoveNotifyIcon(HWND hWnd)
    {
        NOTIFYICONDATA data = {};
        data.hWnd = hWnd;
        data.uID = notifyId;
        Shell_NotifyIcon(NIM_DELETE, &data);
    }
}
