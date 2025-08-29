/*
    CONTRIBUTORS:
        Sean Pesce

*/

#include "d3d11/main.h"
#include <string>


namespace d3d11 {

    HMODULE chain = NULL;
    FARPROC functions[func_count];
    BOOL IsWow64()
    {
        typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
        LPFN_ISWOW64PROCESS fnIsWow64Process = nullptr;
        BOOL bIsWow64 = FALSE;
        //IsWow64Process is not available on all supported versions of Windows.
        //Use GetModuleHandle to get a handle to the DLL that contains the function
        //and GetProcAddress to get a pointer to the function if available.
        fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
        if (NULL != fnIsWow64Process)
        {
            if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
            {
                //handle error
            }
        }
        return bIsWow64;
    }
    bool FileExists(const std::string& path) {
        DWORD attrib = GetFileAttributesA(path.c_str());
        return (attrib != INVALID_FILE_ATTRIBUTES &&
            !(attrib & FILE_ATTRIBUTE_DIRECTORY));
    }
    void hook_exports()
    {
        if (IsWow64())
        {
            chain = LoadLibrary("C:\\Windows\\SysWOW64\\d3d11.dll");
        }
        else
        {
            chain = LoadLibrary("C:\\Windows\\System32\\d3d11.dll");
        }
        //chain = LoadLibrary("bd3d11.dll");
        int count = 0;
        for (int i = 0; i < d3d11::func_count; i++)
        {
            FARPROC func = GetProcAddress(chain, func_names[i]);
            if (func)
            {
                count++;
            }
            functions[i] = func;
        }
        //std::string rdcpath = "C:\\Program Files\\RenderDoc\\renderdoc.dll";
        std::string rdcpath = "E:\\gpa\\shimd3d64.dll";
        std::string rdcpath2 = "E:\\gpa\\shimloader64.dll";
        HMODULE renderdoc = LoadLibrary(rdcpath.c_str());
        HMODULE renderdoc2 = LoadLibrary(rdcpath2.c_str());
        if (!FileExists(rdcpath)) {
            char msg[512];
            sprintf_s(msg, "File does not exist: %s", rdcpath.c_str());
            MessageBoxA(NULL, msg, "File Not Found", MB_ICONERROR);
            return;
        }
        if (!renderdoc) {
            DWORD error = GetLastError();
            char errorMsg[512];

            // 格式化错误信息
            sprintf_s(errorMsg, "Failed to load: %s\nError code: %d", rdcpath.c_str(), error);
            MessageBoxA(NULL, errorMsg, "LoadLibrary Error", MB_ICONERROR);

            // 也可以输出到控制台
            printf("LoadLibrary failed with error %d\n", error);
            exit(0);
        }
    }
}




