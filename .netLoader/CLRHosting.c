#include <windows.h>
#include <metahost.h>
#include <mscoree.h>
#include <stdio.h>

// Import mscorlib to handle AppDomain and Assembly interfaces
#import "mscorlib.tlb" raw_interfaces_only \
    high_property_prefixes("_get","_put","_putref") \
    rename("ReportEvent", "ClrReportEvent")

using namespace mscorlib;

void ExecuteAssemblyFromHeap(unsigned char* rawData, size_t dataSize) {
    HRESULT hr;
    ICLRMetaHost* pMetaHost = NULL;
    ICLRRuntimeInfo* pRuntimeInfo = NULL;
    ICorRuntimeHost* pCorHost = NULL;
    IUnknown* pAppDomainUnk = NULL;
    _AppDomain* pAppDomain = NULL;
    _Assembly* pAssembly = NULL;
    _MethodInfo* pEntryPoint = NULL;

    // 1. Get the MetaHost and Runtime Info
    hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_PPV_ARGS(&pMetaHost));
    hr = pMetaHost->GetRuntime(L"v4.0.30319", IID_PPV_ARGS(&pRuntimeInfo));

    // 2. Get the ICorRuntimeHost interface and start the CLR
    hr = pRuntimeInfo->GetInterface(CLSID_CorRuntimeHost, IID_PPV_ARGS(&pCorHost));
    hr = pCorHost->Start();

    // 3. Create a new AppDomain
    hr = pCorHost->CreateDomain(L"SandboxDomain", NULL, &pAppDomainUnk);
    hr = pAppDomainUnk->QueryInterface(IID_PPV_ARGS(&pAppDomain));

    // 4. Create a SAFEARRAY from the heap buffer
    SAFEARRAYBOUND bounds = { (ULONG)dataSize, 0 };
    SAFEARRAY* pSafeArray = SafeArrayCreate(VT_UI1, 1, &bounds);
    void* ppvData;
    SafeArrayAccessData(pSafeArray, &ppvData);
    memcpy(ppvData, rawData, dataSize);
    SafeArrayUnaccessData(pSafeArray);

    // 5. Load the assembly into the new domain
    hr = pAppDomain->Load_3(pSafeArray, &pAssembly);

    // 6. Execute the entry point (static Main)
    hr = pAssembly->get_EntryPoint(&pEntryPoint);
    VARIANT vRet, vObj;
    VariantInit(&vRet);
    VariantInit(&vObj); // Null object for static methods
    hr = pEntryPoint->Invoke_3(vObj, NULL, &vRet);

    // Cleanup (simplified)
    if (pSafeArray) SafeArrayDestroy(pSafeArray);
    if (pCorHost) pCorHost->Release();
    if (pMetaHost) pMetaHost->Release();
}
