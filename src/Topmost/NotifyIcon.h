#pragma once
#include <Windows.h>

#define NotifyMessageId                0x1234                   // ��������Ϣid
#define NotifyStartMenuAddId           0x1235                   // ��ӵ���ʼ�˵�id
#define NotifyStartMenuRemoveId        0x1236                   // �ӿ�ʼ�˵����Ƴ�id
#define NotifyHelpId                   0x1237                   // �˵��İ�����Ϣid
#define NotifyExitId                   0x1238                   // �˵����˳���Ϣid
#define NotifyStartMenuAddText         L"��ӵ���ʼ�˵�"           // ��ӵ���ʼ�˵��ı�
#define NotifyStartMenuRemoveText      L"�ӿ�ʼ�˵����Ƴ�"         // �ӿ�ʼ�˵����Ƴ��ı�
#define NotifyHelpText                 L"����"                   // �˵��İ����ı�
#define NotifyExitText                 L"�˳�"                   // �˵����˳��ı�

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
