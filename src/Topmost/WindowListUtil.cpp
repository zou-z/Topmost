#include "WindowListUtil.h"
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

namespace Util
{
	WindowListUtil::WindowListUtil()
	{
		windowList = {};
		isRefreshingWindowList = false;
	}

	DWORD WindowListUtil::RefreshWindowList()
	{
		if (isRefreshingWindowList) return S_OK;
		isRefreshingWindowList = true;

		DWORD result = S_OK;
		HWND hWnd = NULL;
		windowList.clear();
		if ((result = GetTopMostWindowHandle(hWnd)) != S_OK)
		{
			isRefreshingWindowList = false;
			return result;
		}
		while ((hWnd = GetWindow(hWnd, GW_HWNDNEXT)) != NULL)
		{
			std::wstring title = GetWindowTitle(hWnd);
			bool isCapturable = false;
			bool isTopMost = false;
			if ((result = IsCapturableWindow(hWnd, title, isCapturable, isTopMost)) != S_OK) return result;
			if (isCapturable)
			{
				WindowInfo windowInfo{};
				windowInfo.Handle = hWnd;
				windowInfo.Title = title;
				windowInfo.IsTopMost = isTopMost;
				RECT rect{};
				if (GetWindowRect(hWnd, &rect) == FALSE)
				{
					isRefreshingWindowList = false;
					return GetLastError();
				}
				windowInfo.Left = rect.left;
				windowInfo.Top = rect.top;
				windowInfo.Right = rect.right;
				windowInfo.Bottom = rect.bottom;
				windowList.push_back(windowInfo);
			}
		}
		isRefreshingWindowList = false;
		return S_OK;
	}

	bool WindowListUtil::FindWindowByCursorPosition(long mouseX, long mouseY, WindowInfo& position)
	{
		if (windowList.size() <= 0) return false;
		int i = 0;
		for (; i < windowList.size(); ++i)
		{
			if (windowList[i].Left < mouseX && mouseX < windowList[i].Right &&
				windowList[i].Top < mouseY && mouseY < windowList[i].Bottom)
			{
				position = windowList[i];
				return true;
			}
		}
		return false;
	}

	DWORD WindowListUtil::SetWindowTopmost(HWND hWnd)
	{
		return S_OK;
	}

	DWORD WindowListUtil::GetTopMostWindowHandle(HWND& hWnd)
	{
		HWND desktopHandle = GetDesktopWindow();
		hWnd = GetTopWindow(desktopHandle);
		if (hWnd == NULL)
		{
			return GetLastError();
		}
		return S_OK;
	}

	DWORD WindowListUtil::IsCapturableWindow(const HWND& hWnd, const std::wstring& title, bool& isCapturable, bool& isTopMost)
	{
		if (title.length() <= 0 ||
			hWnd == GetShellWindow() ||
			!IsWindowVisible(hWnd) ||
			GetAncestor(hWnd, GA_ROOT) != hWnd)
		{
			isCapturable = false;
			return S_OK;
		}

		auto style = GetWindowLong(hWnd, GWL_STYLE);
		if ((style & WS_DISABLED) == WS_DISABLED)
		{
			isCapturable = false;
			return S_OK;
		}

		style = GetWindowLong(hWnd, GWL_EXSTYLE);
		if ((style & WS_EX_TOOLWINDOW) == WS_EX_TOOLWINDOW)
		{
			isCapturable = false;
			return S_OK;
		}

		if (IsIconic(hWnd))
		{
			isCapturable = false;
			return S_OK;
		}

		int classNameMaxLength = 256;
		WCHAR* className = new WCHAR[classNameMaxLength];
		if (GetClassName(hWnd, className, classNameMaxLength) == FALSE)
		{
			isCapturable = false;
			return GetLastError();
		}
		std::wstring classNameString = std::wstring(className);
		delete[] className;
		if (classNameString == L"Windows.UI.Core.CoreWindow" || classNameString == L"ApplicationFrameWindow")
		{
			DWORD cloaked = FALSE;
			if (DwmGetWindowAttribute(hWnd, DWMWA_CLOAKED, &cloaked, sizeof(int)) == S_OK)
			{
				if (cloaked == DWM_CLOAKED_SHELL)
				{
					isCapturable = false;
					return S_OK;
				}
			}
		}

		if (title == L"Task View" && classNameString == L"Windows.UI.Core.CoreWindow" ||
			title == L"DesktopWindowXamlSource" && classNameString == L"Windows.UI.Core.CoreWindow" ||
			title == L"PopupHost" && classNameString == L"Xaml_WindowedPopupClass")
		{
			isCapturable = false;
			return S_OK;
		}

		isTopMost = (style & WS_EX_TOPMOST) == WS_EX_TOPMOST;
		isCapturable = true;
		return S_OK;
	}

	std::wstring WindowListUtil::GetWindowTitle(const HWND& hWnd)
	{
		int length = GetWindowTextLength(hWnd);
		if (length <= 0)
		{
			return L"";
		}
		++length;
		WCHAR* title = new WCHAR[length]{ 0 };
		GetWindowText(hWnd, title, length);
		std::wstring titleString = std::wstring(title);
		delete[] title;
		return titleString;
	}
}
