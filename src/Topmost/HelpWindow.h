#pragma once
#include <Windows.h>
#include <string>

namespace View
{
	class HelpWindow
	{
	public:
		HelpWindow();
		~HelpWindow();
		DWORD Show(HINSTANCE hInstance);

	private:
		DWORD RegisterWindowClass(HINSTANCE hInstance);
		DWORD CreateInstance(HINSTANCE hInstance);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		static HelpWindow* instance;
		std::wstring className;
		std::wstring title;
		HWND hWnd;
		bool isRegisterWindowClass;
		bool isWindowOpened;
	};
}
