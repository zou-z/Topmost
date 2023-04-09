#include "StartMenuUtil.h"
#include <ShlObj.h>
#include <Shlwapi.h>
#include <string>
#include "resource.h"
#pragma comment(lib, "shlwapi.lib")

namespace Util
{
	StartMenuUtil::StartMenuUtil()
	{
	}

	StartMenuUtil::~StartMenuUtil()
	{
	}

	bool StartMenuUtil::AddToStartMenu()
	{
		HRESULT hResult = NULL;
		// 获取开始菜单路径
		LPWSTR startMenuPath;
		if ((hResult = SHGetKnownFolderPath(FOLDERID_StartMenu, 0, NULL, &startMenuPath)) != S_OK)
		{
			OutputDebugString((L"Start Menu Add, SHGetKnownFolderPath Failed, HRESULT: " + std::to_wstring(hResult)).c_str());
			return false;
		}

		// 构造 .lnk 文件的绝对路径
		LPWSTR shortcutPath = (LPWSTR)CoTaskMemAlloc(MAX_PATH * sizeof(WCHAR));
		if (shortcutPath == nullptr)
		{
			OutputDebugString(L"Start Menu Add, CoTaskMemAlloc Failed");
			CoTaskMemFree(startMenuPath);
			return false;
		}
		swprintf_s(shortcutPath, MAX_PATH, L"%s\\%s.lnk", startMenuPath, AppName);

		// 判断快捷方式是否已存在
		if (PathFileExists(shortcutPath) == TRUE)
		{
			OutputDebugString(L"Start Menu Add, Shortcut Already Exist");
			CoTaskMemFree(startMenuPath);
			CoTaskMemFree(shortcutPath);
			return true;
		}

		// 获取当前exe路径
		WCHAR exePath[MAX_PATH + 1] = { 0 };
		if (GetModuleFileName(NULL, exePath, MAX_PATH) == FALSE)
		{
			OutputDebugString((L"Start Menu Add, GetModuleFileName Failed, Error Code: " + std::to_wstring(GetLastError())).c_str());
			CoTaskMemFree(startMenuPath);
			CoTaskMemFree(shortcutPath);
			return false;
		}

		// 创建 .lnk 文件并设置其属性
		IShellLink* shortcut;
		if ((hResult = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&shortcut))) != S_OK)
		{
			OutputDebugString((L"Start Menu Add, CoCreateInstance Failed, HRESULT: " + std::to_wstring(hResult)).c_str());
			CoTaskMemFree(startMenuPath);
			CoTaskMemFree(shortcutPath);
			return false;
		}
		if ((hResult = shortcut->SetPath(exePath)) != S_OK)
		{
			OutputDebugString((L"Start Menu Add, SetPath Failed, HRESULT: " + std::to_wstring(hResult)).c_str());
			shortcut->Release();
			CoTaskMemFree(startMenuPath);
			CoTaskMemFree(shortcutPath);
			return false;
		}
		IPersistFile* persistFile = nullptr;
		if ((hResult = shortcut->QueryInterface(IID_IPersistFile, (void**)&persistFile)) != S_OK)
		{
			OutputDebugString((L"Start Menu Add, QueryInterface Failed, HRESULT: " + std::to_wstring(hResult)).c_str());
			shortcut->Release();
			CoTaskMemFree(startMenuPath);
			CoTaskMemFree(shortcutPath);
			return false;
		}
		if ((hResult = persistFile->Save(shortcutPath, TRUE)) != S_OK)
		{
			OutputDebugString((L"Start Menu Add, Save Failed, HRESULT: " + std::to_wstring(hResult)).c_str());
		}

		// 释放内存
		persistFile->Release();
		shortcut->Release();
		CoTaskMemFree(startMenuPath);
		CoTaskMemFree(shortcutPath);
		return true;
	}

	bool StartMenuUtil::RemoveFromStartMenu()
	{
		HRESULT hResult = NULL;
		// 获取开始菜单路径
		LPWSTR startMenuPath;
		if ((hResult = SHGetKnownFolderPath(FOLDERID_StartMenu, 0, NULL, &startMenuPath)) != S_OK)
		{
			OutputDebugString((L"Start Menu Remove, SHGetKnownFolderPath Failed, HRESULT: " + std::to_wstring(hResult)).c_str());
			return false;
		}

		// 构造 .lnk 文件的绝对路径
		LPWSTR shortcutPath = (LPWSTR)CoTaskMemAlloc(MAX_PATH * sizeof(WCHAR));
		if (shortcutPath == nullptr)
		{
			OutputDebugString(L"Start Menu Remove, CoTaskMemAlloc Failed");
			CoTaskMemFree(startMenuPath);
			return false;
		}
		swprintf_s(shortcutPath, MAX_PATH, L"%s\\%s.lnk", startMenuPath, AppName);

		// 判断快捷方式是否存在
		if (PathFileExists(shortcutPath) == FALSE)
		{
			OutputDebugString(L"Start Menu Remove, Shortcut Not Exist");
			CoTaskMemFree(startMenuPath);
			CoTaskMemFree(shortcutPath);
			return true;
		}

		// 删除快捷方式
		if (DeleteFile(shortcutPath) == FALSE)
		{
			OutputDebugString((L"Start Menu Remove, Delete Shortcut Failed, Error Code: " + std::to_wstring(GetLastError())).c_str());
		}
	
		// 释放内存
		CoTaskMemFree(startMenuPath);
		CoTaskMemFree(shortcutPath);
		return true;
	}
}
