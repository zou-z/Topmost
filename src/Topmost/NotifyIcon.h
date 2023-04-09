#pragma once
#include <Windows.h>

#define NotifyMessageId    0x1234    // 任务栏消息id
#define NotifyHelpId       0x1235    // 菜单的帮助消息id
#define NotifyExitId       0x1236    // 菜单的退出消息id
#define NotifyHelpText     L"帮助"    // 菜单的帮助文本
#define NotifyExitText     L"退出"    // 菜单的退出文本

namespace View
{
	class NotifyIcon
	{
	public:
		NotifyIcon();
		~NotifyIcon();
		void ShowNotifyIcon(HINSTANCE hInstance, HWND hWnd);
		DWORD ShowNotifyMenu(HWND hWnd);
		void RemoveNotifyIcon(HWND hWnd);

	private:
		const int notifyId = 1;
	};
}
