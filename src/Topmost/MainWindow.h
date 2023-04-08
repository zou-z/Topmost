#pragma once
#include <Windows.h>
#include <string>
#include "WindowListUtil.h"

namespace App
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
		void RedrawWindowBorder(long width, long height, COLORREF color);
		void ShowErrorMessage(std::wstring message, DWORD errorCode);

	private:
		static MainWindow* instance;
		std::wstring className;
		std::wstring title;
		HWND hWnd;
		const int hotKeyId = 1;
		Util::WindowListUtil* windowListUtil;
	};
}