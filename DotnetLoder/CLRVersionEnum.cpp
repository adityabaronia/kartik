// CLRVersionEnum.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <windows.h>
#include <iostream>
#include <MetaHost.h>
#pragma comment(lib, "mscoree.lib")

int main()
{
	ICLRMetaHost* pMetaHost = NULL;
	IEnumUnknown* CLRRuntineEnum = NULL;
	

	IUnknown* pRuntimeUnk = NULL;

	HRESULT hr = CLRCreateInstance(CLSID_CLRMetaHost,
	IID_ICLRMetaHost, (LPVOID*)&pMetaHost);

	hr = pMetaHost->EnumerateInstalledRuntimes(&CLRRuntineEnum);
	if (SUCCEEDED(hr))
	{
		ULONG fetched = 0;
		while (!CLRRuntineEnum->Next(1, &pRuntimeUnk, &fetched)) {
			ICLRRuntimeInfo* pRuntimeInfo = NULL;
			hr = pRuntimeUnk->QueryInterface(IID_ICLRRuntimeInfo, (LPVOID*)&pRuntimeInfo);

			if (SUCCEEDED(hr))
			{
				WCHAR version[MAX_PATH];
				DWORD length = MAX_PATH;
				//pRuntimeInfo->GetVersionString(version, &length);
				if (SUCCEEDED(pRuntimeInfo->GetVersionString(version, &length))) {
					std::wcout << L"Found CLR Version: " << version << std::endl;
				}
				pRuntimeInfo->Release();
			}
			pRuntimeUnk->Release();
		}
	}
	pMetaHost->Release();


}

