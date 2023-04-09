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
		// ��ȡ��ʼ�˵�·��
		LPWSTR startMenuPath;
		if ((hResult = SHGetKnownFolderPath(FOLDERID_StartMenu, 0, NULL, &startMenuPath)) != S_OK)
		{
			OutputDebugString((L"Start Menu Add, SHGetKnownFolderPath Failed, HRESULT: " + std::to_wstring(hResult)).c_str());
			return false;
		}

		// ���� .lnk �ļ��ľ���·��
		LPWSTR shortcutPath = (LPWSTR)CoTaskMemAlloc(MAX_PATH * sizeof(WCHAR));
		if (shortcutPath == nullptr)
		{
			OutputDebugString(L"Start Menu Add, CoTaskMemAlloc Failed");
			CoTaskMemFree(startMenuPath);
			return false;
		}
		swprintf_s(shortcutPath, MAX_PATH, L"%s\\%s.lnk", startMenuPath, AppName);

		// �жϿ�ݷ�ʽ�Ƿ��Ѵ���
		if (PathFileExists(shortcutPath) == TRUE)
		{
			OutputDebugString(L"Start Menu Add, Shortcut Already Exist");
			CoTaskMemFree(startMenuPath);
			CoTaskMemFree(shortcutPath);
			return true;
		}

		// ��ȡ��ǰexe·��
		WCHAR exePath[MAX_PATH + 1] = { 0 };
		if (GetModuleFileName(NULL, exePath, MAX_PATH) == FALSE)
		{
			OutputDebugString((L"Start Menu Add, GetModuleFileName Failed, Error Code: " + std::to_wstring(GetLastError())).c_str());
			CoTaskMemFree(startMenuPath);
			CoTaskMemFree(shortcutPath);
			return false;
		}

		// ���� .lnk �ļ�������������
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

		// �ͷ��ڴ�
		persistFile->Release();
		shortcut->Release();
		CoTaskMemFree(startMenuPath);
		CoTaskMemFree(shortcutPath);
		return true;
	}

	bool StartMenuUtil::RemoveFromStartMenu()
	{
		HRESULT hResult = NULL;
		// ��ȡ��ʼ�˵�·��
		LPWSTR startMenuPath;
		if ((hResult = SHGetKnownFolderPath(FOLDERID_StartMenu, 0, NULL, &startMenuPath)) != S_OK)
		{
			OutputDebugString((L"Start Menu Remove, SHGetKnownFolderPath Failed, HRESULT: " + std::to_wstring(hResult)).c_str());
			return false;
		}

		// ���� .lnk �ļ��ľ���·��
		LPWSTR shortcutPath = (LPWSTR)CoTaskMemAlloc(MAX_PATH * sizeof(WCHAR));
		if (shortcutPath == nullptr)
		{
			OutputDebugString(L"Start Menu Remove, CoTaskMemAlloc Failed");
			CoTaskMemFree(startMenuPath);
			return false;
		}
		swprintf_s(shortcutPath, MAX_PATH, L"%s\\%s.lnk", startMenuPath, AppName);

		// �жϿ�ݷ�ʽ�Ƿ����
		if (PathFileExists(shortcutPath) == FALSE)
		{
			OutputDebugString(L"Start Menu Remove, Shortcut Not Exist");
			CoTaskMemFree(startMenuPath);
			CoTaskMemFree(shortcutPath);
			return true;
		}

		// ɾ����ݷ�ʽ
		if (DeleteFile(shortcutPath) == FALSE)
		{
			OutputDebugString((L"Start Menu Remove, Delete Shortcut Failed, Error Code: " + std::to_wstring(GetLastError())).c_str());
		}
	
		// �ͷ��ڴ�
		CoTaskMemFree(startMenuPath);
		CoTaskMemFree(shortcutPath);
		return true;
	}
}
