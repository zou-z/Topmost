#include "HelpWindow.h"
#include "resource.h"

namespace View
{
	HelpWindow* HelpWindow::instance;

	HelpWindow::HelpWindow()
	{
		instance = this;
		className = L"Topmost_Help";
		title = L"Topmost - °ïÖú";
		hWnd = NULL;
		isRegisterWindowClass = false;
		isWindowOpened = false;
	}

	HelpWindow::~HelpWindow()
	{
		instance = nullptr;
		hWnd = NULL;
	}

	DWORD HelpWindow::Show(HINSTANCE hInstance)
	{
		if (isWindowOpened)
		{
			if (SetActiveWindow(hWnd) == NULL)
			{
				return GetLastError();
			}
		}
		else
		{
			DWORD errorCode = S_OK;
			if (!isRegisterWindowClass)
			{
				isRegisterWindowClass = true;
				if ((errorCode = RegisterWindowClass(hInstance)) != S_OK) return errorCode;
			}
			if ((errorCode = CreateInstance(hInstance)) != S_OK) return errorCode;
			isWindowOpened = true;
		}
		return S_OK;
	}

	DWORD HelpWindow::RegisterWindowClass(HINSTANCE hInstance)
	{
		WNDCLASSEXW wcex{};
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = HelpWindow::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_ICON1);
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = className.c_str();
		wcex.hIconSm = LoadIcon(hInstance, (LPCTSTR)IDI_ICON1);

		if (RegisterClassExW(&wcex) == FALSE)
		{
			return GetLastError();
		}
		return S_OK;
	}

	DWORD HelpWindow::CreateInstance(HINSTANCE hInstance)
	{
		hWnd = CreateWindowEx(NULL, className.c_str(), title.c_str(), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, nullptr, nullptr, hInstance, nullptr);
		if (hWnd == NULL)
		{
			return GetLastError();
		}
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
		return S_OK;
	}

	LRESULT HelpWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			instance->isWindowOpened = false;
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
}
