#pragma once
#include <Windows.h>
#include <string>
#include "WindowListUtil.h"
#include "NotifyIcon.h"
#include "HelpWindow.h"
#include "StartMenuUtil.h"

namespace View
{
	class MainWindow
	{
	public:
		MainWindow();
		~MainWindow();
		DWORD Initialization(HINSTANCE hInstance);

	private:
		DWORD RegisterWindowClass(HINSTANCE hInstance);
		DWORD CreateInstance(HINSTANCE hInstance);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		DWORD RegisterKey();
		DWORD UnregisterKey();
		void SetWindowTopmost();
		void ShowNotifyMenu(LPARAM lParam, HWND hWnd);
		void RedrawWindowBorder(long width, long height, COLORREF color);
		void ShowErrorMessage(std::wstring message, DWORD errorCode);

	private:
		static MainWindow* instance;
		std::wstring className;
		std::wstring title;
		HINSTANCE hInstance;
		HWND hWnd;
		const int hotKeyId = 1;         // �ȼ�id
		const int timerId = 1;          // �رմ��ڶ�ʱ��id
		const int timerInterval = 200;  // �رմ��ڶ�ʱ����ʱʱ�䣨���룩
		Util::WindowListUtil* windowListUtil;
		Util::StartMenuUtil* startMenuUtil;
		View::NotifyIcon* notifyIcon;
		View::HelpWindow* helpWindow;
	};
}
