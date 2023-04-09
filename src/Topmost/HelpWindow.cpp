#include "HelpWindow.h"
#include "resource.h"
#pragma comment(lib, "version.lib")

namespace View
{
	HelpWindow* HelpWindow::instance;

	HelpWindow::HelpWindow()
	{
		instance = this;
		className = L"Topmost_Help";
		title = L"Topmost - 帮助";
		hWnd = NULL;
		isRegisterWindowClass = false;
		isWindowOpened = false;
		version = {};
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
		hWnd = CreateWindowEx(NULL, className.c_str(), title.c_str(), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 300, 170, nullptr, nullptr, hInstance, nullptr);
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
		case WM_PAINT:
		{
			instance->RenderText(hWnd);
			break;
		}
		case WM_DESTROY:
			instance->isWindowOpened = false;
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	void HelpWindow::RenderText(HWND hWnd)
	{
		int thickness = 1;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		// 设置并选中字体
		HFONT hFont = CreateFont(
			-MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 96), // 字号
			0,                                               // 宽度
			0,                                               // 倾斜度
			0,                                               // 方向
			FW_NORMAL,                                       // 粗细
			FALSE,                                           // 斜体
			FALSE,                                           // 下划线
			FALSE,                                           // 删除线
			DEFAULT_CHARSET,                                 // 字符集
			OUT_DEFAULT_PRECIS,                              // 输出精度
			CLIP_DEFAULT_PRECIS,                             // 裁剪精度
			DEFAULT_QUALITY,                                 // 质量
			DEFAULT_PITCH | FF_SWISS,                        // 间距和族
			L"微软雅黑");
		HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

		// 设置文本颜色
		SetTextColor(hdc, RGB(0, 0, 0));

		// 获取窗口区域
		RECT rect;
		GetClientRect(hWnd, &rect);
		rect.top = 30;

		// 获取程序版本
		if (version.length() <= 0)
		{
			GetAppVersion(version);
		}

		// 绘制文本
		std::wstring text =
			std::wstring(L"快捷键: Ctrl + Shift + `\r\n") +
			std::wstring(L"https://github.com/zou-z/Topmost\r\n") +
			std::wstring(L"v" + version + L"\r\n") +
			std::wstring(L"© zzh");
		DrawText(hdc, text.c_str(), -1, &rect, DT_WORDBREAK | DT_CENTER);

		// 回收资源
		EndPaint(hWnd, &ps);
		DeleteObject(hFont);
		DeleteDC(hdc);
	}

	DWORD HelpWindow::GetAppVersion(std::wstring& version)
	{
		// 加载rc资源
		HRSRC hResInfo = FindResource(NULL, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
		if (hResInfo == NULL)
		{
			return GetLastError();
		}
		HGLOBAL hRes = LoadResource(NULL, hResInfo);
		if (hRes == NULL)
		{
			return GetLastError();
		}
		LPVOID lpRes = LockResource(hRes);
		if (lpRes == NULL)
		{
			return GetLastError();
		}

		// 查找版本号
		UINT length;
		VS_FIXEDFILEINFO* fileInfo = nullptr;
		VerQueryValue(lpRes, L"\\", (LPVOID*)&fileInfo, &length);
		DWORD fileVersionMS = fileInfo->dwFileVersionMS;
		DWORD fileVersionLS = fileInfo->dwFileVersionLS;

		// 格式化版本号
		version =
			std::to_wstring(HIWORD(fileVersionMS)) + L"." +
			std::to_wstring(LOWORD(fileVersionMS)) + L"." +
			std::to_wstring(HIWORD(fileVersionLS)) + L"." +
			std::to_wstring(LOWORD(fileVersionLS));

		// 释放资源
		FreeResource(hRes);
	}
}
