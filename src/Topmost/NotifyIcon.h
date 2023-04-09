#pragma once
#include <Windows.h>

#define NotifyMessageId    0x1234    // ��������Ϣid
#define NotifyHelpId       0x1235    // �˵��İ�����Ϣid
#define NotifyExitId       0x1236    // �˵����˳���Ϣid
#define NotifyHelpText     L"����"    // �˵��İ����ı�
#define NotifyExitText     L"�˳�"    // �˵����˳��ı�

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
