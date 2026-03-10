#include <windows.h>
#include <stdio.h>
//#include <MetaHost.h>
#include "peheader.h"
#include "clr.h"
//#pragma comment(lib, "mscoree.lib")
//#pragma comment(lib, "oleaut32.lib")


// ============================================================
// YOUR EXACT RVAtoOffset
// ============================================================
DWORD RVAtoOffset(DWORD rva, PDIMAGE_NT_HEADERS32 nt)
{
    PDIMAGE_SECTION_HEADER section = (PDIMAGE_SECTION_HEADER)(
        (unsigned char*)nt + 0x18 + nt->FileHeader.SizeOfOptionalHeader);

    for (int i = 0; i < nt->FileHeader.NumberOfSections; i++)
    {
        DWORD sectionStart = section[i].VirtualAddress;
        DWORD sectionEnd = sectionStart + section[i].Misc.VirtualSize;
        if (rva >= sectionStart && rva < sectionEnd)
            return rva - sectionStart + section[i].PointerToRawData;
    }
    return rva;
}

// ============================================================
// YOUR EXACT parseDotnetForNetVersion
// ============================================================
widecharbuffer* parseDotnetForNetVersion(CHAR* address)
{
    PDIMAGE_DOS_HEADER assemblyFile = (PDIMAGE_DOS_HEADER)address;
    PDIMAGE_NT_HEADERS32 nt = (PDIMAGE_NT_HEADERS32)((BYTE*)address + assemblyFile->e_lfanew);

    unsigned short magic = nt->OptionalHeader.Magic;
    if (magic != 0x010b)
    {
        PDIMAGE_NT_HEADERS64 nt64 = (PDIMAGE_NT_HEADERS64)(address + assemblyFile->e_lfanew);
        (void)nt64;
    }

    DIMAGE_DATA_DIRECTORY clrDir = nt->OptionalHeader.DataDirectory[14];
    if (!clrDir.VirtualAddress) {
        printf("not a dotnet file.\n");
        exit(0);
    }

    unsigned long clrOffset = RVAtoOffset(clrDir.VirtualAddress, nt);
    PIMAGE_COR20_HEADER clr = (PIMAGE_COR20_HEADER)((BYTE*)address + clrOffset);

    printf("CLR Header Flags: 0x%08X\n", clr->Flags);
    printf("CLR Major Runtime Version: %d\n", clr->MajorRuntimeVersion);
    printf("CLR Minor Runtime Version: %d\n", clr->MinorRuntimeVersion);
    printf("Entry Point Token: 0x%08X\n", clr->EntryPointToken);

    unsigned long metadataOffset = RVAtoOffset(clr->MetaData.VirtualAddress, nt);
    unsigned char* metadata = (BYTE*)address + metadataOffset;

    METADATA_HEADER* header = (METADATA_HEADER*)metadata;
    char* version = (char*)(metadata + sizeof(METADATA_HEADER));
    unsigned long netversionlength = header->VersionLength;

    char* netversion = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, netversionlength);
    if (!netversion) printf("cannot allocate heap to store dotnet version\n");
    memcpy(netversion, version, netversionlength);

    unsigned short sizeRequired = (unsigned short)MultiByteToWideChar(
        CP_ACP, 0, netversion, (int)strlen(netversion), NULL, 0);

    wchar_t* netversionWideChar = (wchar_t*)HeapAlloc(
        GetProcessHeap(), HEAP_ZERO_MEMORY, (sizeRequired + 1) * sizeof(wchar_t));
    if (!netversionWideChar) printf("cannot allocate heap to store dotnet version\n");

    MultiByteToWideChar(CP_ACP, 0, netversion, (int)strlen(netversion),
        netversionWideChar, sizeRequired);

    wprintf(L"CLR Version: %s\n", netversionWideChar);

    // Enumerate installed runtimes
    ICLRMetaHost* pMetaHost = NULL;
    IEnumUnknown* pRuntimeEnum = NULL;
    IUnknown* pRuntimeUnk = NULL;

    CLRCreateInstanceFnPtr xCLRCreateInstance = (CLRCreateInstanceFnPtr)GetProcAddress(LoadLibraryA("MSCOREE.DLL"), "CLRCreateInstance");
    HRESULT hr = 0;
    hr = xCLRCreateInstance(&xCLSID_CLRMetaHost, &xIID_ICLRMetaHost, (LPVOID*)&pMetaHost);
    hr = pMetaHost->lpVtbl->EnumerateInstalledRuntimes(pMetaHost, &pRuntimeEnum);



    ULONG fetched = 0;
    while (pRuntimeEnum->lpVtbl->Next(pRuntimeEnum, 1, &pRuntimeUnk, &fetched) == S_OK)
    {
        ICLRRuntimeInfo* pRuntimeInfo = NULL;
        if (SUCCEEDED(pRuntimeUnk->lpVtbl->QueryInterface(
            pRuntimeUnk, &xIID_ICLRRuntimeInfo, (LPVOID*)&pRuntimeInfo)))
        {
            WCHAR ver[MAX_PATH];
            DWORD length = MAX_PATH;
            if (SUCCEEDED(pRuntimeInfo->lpVtbl->GetVersionString(pRuntimeInfo, ver, &length)))
            {
                wprintf(L"Found CLR Version: %s\n", ver);
                if (wcscmp(ver, netversionWideChar) == 0)
                {
                    // Pass the address of your interface pointer
                    wprintf(L"Found CLR Version matching with application: %s\n", ver);

                }
                else {
                    printf("version not found\n");
                }
            }
            pRuntimeInfo->lpVtbl->Release(pRuntimeInfo);
        }
        pRuntimeUnk->lpVtbl->Release(pRuntimeUnk);
    }

    pRuntimeEnum->lpVtbl->Release(pRuntimeEnum);
    pMetaHost->lpVtbl->Release(pMetaHost);

    widecharbuffer* netversionstring = (widecharbuffer*)HeapAlloc(
        GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(widecharbuffer));
    netversionstring->Buffer = netversionWideChar;
    netversionstring->Length = sizeRequired;
    return netversionstring;
}

// ============================================================
// YOUR EXACT localDotnetExecute
// ============================================================
void localDotnetExecute(widecharbuffer* DotNetVersion, ascicharbuffer dotnetbinary)
{
    HRESULT          hr = S_OK;
    ICLRMetaHost* pMetaHost = NULL;
    ICLRRuntimeInfo* CLRRuntimeInfo = NULL;
    ICorRuntimeHost* pCorRuntimeHost = NULL;
    IUnknown* AppDomainUnk = NULL;
    IAppDomain* pAppDomain = NULL;
    IAssembly* pAssembly = NULL;
    IMethodInfo* pEntryPoint = NULL;

    CLRCreateInstanceFnPtr xCLRCreateInstance = (CLRCreateInstanceFnPtr)GetProcAddress(LoadLibraryA("MSCOREE.DLL"), "CLRCreateInstance");
    hr = xCLRCreateInstance(&xCLSID_CLRMetaHost, &xIID_ICLRMetaHost, (LPVOID*)&pMetaHost);
    printf("CLRCreateInstance:   0x%08X | ptr: %p\n", hr, pMetaHost);

    hr = pMetaHost->lpVtbl->GetRuntime(
        pMetaHost, DotNetVersion->Buffer, &xIID_ICLRRuntimeInfo, (LPVOID*)&CLRRuntimeInfo);
    printf("GetRuntime:          0x%08X | ptr: %p\n", hr, CLRRuntimeInfo);

    BOOL CLRLoadable = FALSE;
    CLRRuntimeInfo->lpVtbl->IsLoadable(CLRRuntimeInfo, &CLRLoadable);
    if (CLRLoadable)
    {
        printf("IsLoadable:          0x%08X | %d\n", hr, CLRLoadable);
        printf("CLR is loadable\n");
    }

    hr = CLRRuntimeInfo->lpVtbl->GetInterface(
        CLRRuntimeInfo, &xCLSID_CorRuntimeHost, &xIID_ICorRuntimeHost, (LPVOID*)&pCorRuntimeHost);
    printf("GetInterface:        0x%08X | ptr: %p\n", hr, pCorRuntimeHost);

    hr = pCorRuntimeHost->lpVtbl->Start(pCorRuntimeHost);
    printf("Start:               0x%08X\n", hr);

    hr = pCorRuntimeHost->lpVtbl->CreateDomain(pCorRuntimeHost, L"GUIDomain", NULL, &AppDomainUnk);
    //hr = pCorRuntimeHost->lpVtbl->GetDefaultDomain(pCorRuntimeHost, &AppDomainUnk);
    printf("GetDefaultDomain:    0x%08X | ptr: %p\n", hr, AppDomainUnk);

    hr = AppDomainUnk->lpVtbl->QueryInterface(AppDomainUnk, &xIID_AppDomain, (void**)&pAppDomain);
    printf("QueryInterface:      0x%08X | ptr: %p\n", hr, pAppDomain);

    // Build SAFEARRAY
    SAFEARRAY* pSafeArray = SafeArrayCreateVector(VT_UI1, 0, dotnetbinary.Length);
    void HUGEP* pDest = NULL;
    SafeArrayAccessData(pSafeArray, &pDest);
    memcpy(pDest, dotnetbinary.Buffer, dotnetbinary.Length);
    SafeArrayUnaccessData(pSafeArray);




    /*setup AMSI bypass using hardware breakpoint*/



    // Load_3
    hr = pAppDomain->lpVtbl->Load_3(pAppDomain, pSafeArray, &pAssembly);
    printf("Load_3 hr: 0x%08X\n", hr);
    SafeArrayDestroy(pSafeArray);

    if (SUCCEEDED(hr))
    {
        printf("Assembly loaded from memory successfully!\n");

        hr = pAssembly->lpVtbl->EntryPoint(pAssembly, &pEntryPoint);
        printf("got entry point hr: 0x%08X\n", hr);

        VARIANT vRet, vObj;
        VariantInit(&vRet);
        VariantInit(&vObj);

        SAFEARRAY* pMethodArgs = SafeArrayCreateVector(VT_VARIANT, 0, 1);
        VARIANT vtPsa;
        VariantInit(&vtPsa);
        vtPsa.vt = VT_ARRAY | VT_BSTR;
        vtPsa.parray = SafeArrayCreateVector(VT_BSTR, 0, 0);
        LONG idx = 0;
        SafeArrayPutElement(pMethodArgs, &idx, &vtPsa);

        hr = pEntryPoint->lpVtbl->Invoke_3(pEntryPoint, vObj, pMethodArgs, &vRet);
        printf("Invoke_3 hr: 0x%08X\n", hr);

        SafeArrayDestroy(vtPsa.parray);
        SafeArrayDestroy(pMethodArgs);
    }

    // Cleanup
    if (pEntryPoint)     pEntryPoint->lpVtbl->Release(pEntryPoint);
    if (pAssembly)       pAssembly->lpVtbl->Release(pAssembly);
    if (pAppDomain)      pAppDomain->lpVtbl->Release(pAppDomain);
    if (AppDomainUnk)    AppDomainUnk->lpVtbl->Release(AppDomainUnk);
    if (pCorRuntimeHost) {
        pCorRuntimeHost->lpVtbl->Stop(pCorRuntimeHost);
        pCorRuntimeHost->lpVtbl->Release(pCorRuntimeHost);
    }
    if (CLRRuntimeInfo)  CLRRuntimeInfo->lpVtbl->Release(CLRRuntimeInfo);
    if (pMetaHost)       pMetaHost->lpVtbl->Release(pMetaHost);
}

// ============================================================
// YOUR EXACT main
// ============================================================
int main(void)
{
    printf("Pointer size: %zu\n", sizeof(void*));

    HANDLE fhandle = CreateFileW(
        L"C:\\Users\\outlook\\source\\repos\\testing\\bin\\Debug\\testing.exe",
        GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (fhandle == INVALID_HANDLE_VALUE) {
        printf("Cannot open dotnet binary\n");
        return 1;
    }

    LARGE_INTEGER fileSize = { 0 };
    if (!GetFileSizeEx(fhandle, &fileSize)) {
        printf("Cannot determine dotnet binary file size\n");
        return 1;
    }

    LPVOID dotnetBinaryAddress = VirtualAlloc(NULL, (SIZE_T)fileSize.QuadPart,
        MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!dotnetBinaryAddress) {
        printf("Allocation for dotnet binary failed\n");
        return 1;
    }

    DWORD bytesRead = 0;
    if (!ReadFile(fhandle, dotnetBinaryAddress, (DWORD)fileSize.QuadPart, &bytesRead, NULL)) {
        printf("Error in reading file\n");
        return 1;
    }

    printf("File size: %lld, Bytes read: %lu\n", fileSize.QuadPart, bytesRead);

    widecharbuffer* NetVersion = parseDotnetForNetVersion((char*)dotnetBinaryAddress);
    wprintf(L"CLR version required: %s\n", NetVersion->Buffer);

    ascicharbuffer dotnetBinary;
    dotnetBinary.Buffer = (unsigned char*)dotnetBinaryAddress;
    dotnetBinary.Length = bytesRead;

    localDotnetExecute(NetVersion, dotnetBinary);

    CloseHandle(fhandle);
    VirtualFree(dotnetBinaryAddress, 0, MEM_RELEASE);
    return 0;
}