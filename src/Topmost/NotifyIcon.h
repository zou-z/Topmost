#pragma once
#include <Windows.h>

#define NotifyMessageId                0x1234                   // 任务栏消息id
#define NotifyStartMenuAddId           0x1235                   // 添加到开始菜单id
#define NotifyStartMenuRemoveId        0x1236                   // 从开始菜单中移除id
#define NotifyHelpId                   0x1237                   // 菜单的帮助消息id
#define NotifyExitId                   0x1238                   // 菜单的退出消息id
#define NotifyStartMenuAddText         L"添加到开始菜单"           // 添加到开始菜单文本
#define NotifyStartMenuRemoveText      L"从开始菜单中移除"         // 从开始菜单中移除文本
#define NotifyHelpText                 L"帮助"                   // 菜单的帮助文本
#define NotifyExitText                 L"退出"                   // 菜单的退出文本

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
