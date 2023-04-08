#pragma once
#include <Windows.h>
#include <string>
#include <vector>

namespace Util
{
	struct WindowInfo
	{
		HWND Handle;
		std::wstring Title;
		long Left;
		long Top;
		long Right;
		long Bottom;
		bool IsTopMost;
	};

	class WindowListUtil
	{
	public:
		WindowListUtil();
		DWORD RefreshWindowList();
		bool FindWindowByCursorPosition(long mouseX, long mouseY, WindowInfo& position);
		DWORD SetWindowTopmost(HWND hWnd, bool isWindowTopmost);

	private:
		DWORD GetTopMostWindowHandle(HWND& hWnd);
		DWORD IsCapturableWindow(const HWND& hWnd, const std::wstring& title, bool& isCapturable, bool& isTopMost);
		std::wstring GetWindowTitle(const HWND& hWnd);

	private:
		std::vector<WindowInfo> windowList;
		bool isRefreshingWindowList;
	};
}
