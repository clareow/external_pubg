#pragma once
#include "include.h"

bool DisableProxies(void)
{
	HKEY hKey;
	DWORD data = 0;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return false; // Key could not be opened

	if (RegSetValueExW(hKey, L"ProxyEnable", 0, REG_DWORD, (LPBYTE)&data, sizeof(data)) != ERROR_SUCCESS)
		return false; // Failed to set the value

	if (RegCloseKey(hKey) != ERROR_SUCCESS)
		return false; // Could not close key.

	return true;
}