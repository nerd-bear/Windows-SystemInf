#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <io.h>
#include <fcntl.h>
#include <wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

// Helper function to execute WMI queries
std::vector<IWbemClassObject *> executeWMIQuery(IWbemServices *pSvc, const wchar_t *query)
{
    std::vector<IWbemClassObject *> results;
    IEnumWbemClassObject *pEnumerator = NULL;
    HRESULT hres = pSvc->ExecQuery(SysAllocString(L"WQL"), SysAllocString(query),
                                   WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);

    if (FAILED(hres))
    {
        std::wcout << L"Query failed. Error code = 0x" << std::hex << hres << std::endl;
        return results;
    }

    IWbemClassObject *pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator)
    {
        hres = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (0 == uReturn)
            break;
        results.push_back(pclsObj);
    }

    pEnumerator->Release();
    return results;
}

// Helper function to get property value as wstring
std::wstring getWMIProperty(IWbemClassObject *obj, const wchar_t *property)
{
    VARIANT vtProp;
    HRESULT hr = obj->Get(property, 0, &vtProp, 0, 0);
    if (SUCCEEDED(hr) && vtProp.vt == VT_BSTR)
    {
        std::wstring result = vtProp.bstrVal;
        VariantClear(&vtProp);
        return result;
    }
    VariantClear(&vtProp);
    return L"N/A";
}

void getSystemInfo(IWbemServices *pSvc)
{
    std::wcout << L"\n=== System Information ===" << std::endl;
    auto results = executeWMIQuery(pSvc, L"SELECT * FROM Win32_ComputerSystem");
    if (!results.empty())
    {
        std::wcout << L"Manufacturer: " << getWMIProperty(results[0], L"Manufacturer") << std::endl;
        std::wcout << L"Model: " << getWMIProperty(results[0], L"Model") << std::endl;
        std::wcout << L"Total Physical Memory: " << getWMIProperty(results[0], L"TotalPhysicalMemory") << L" bytes" << std::endl;
    }
    for (auto obj : results)
        obj->Release();
}

void getProcessorInfo(IWbemServices *pSvc)
{
    std::wcout << L"\n=== Processor Information ===" << std::endl;
    auto results = executeWMIQuery(pSvc, L"SELECT * FROM Win32_Processor");
    for (auto obj : results)
    {
        std::wcout << L"Name: " << getWMIProperty(obj, L"Name") << std::endl;
        std::wcout << L"Manufacturer: " << getWMIProperty(obj, L"Manufacturer") << std::endl;
        std::wcout << L"Max Clock Speed: " << getWMIProperty(obj, L"MaxClockSpeed") << L" MHz" << std::endl;
        std::wcout << L"Number of Cores: " << getWMIProperty(obj, L"NumberOfCores") << std::endl;
        std::wcout << L"Number of Logical Processors: " << getWMIProperty(obj, L"NumberOfLogicalProcessors") << std::endl;
        std::wcout << L"---" << std::endl;
    }
    for (auto obj : results)
        obj->Release();
}

void getMemoryInfo(IWbemServices *pSvc)
{
    std::wcout << L"\n=== Memory Information ===" << std::endl;
    auto results = executeWMIQuery(pSvc, L"SELECT * FROM Win32_PhysicalMemory");
    for (auto obj : results)
    {
        std::wcout << L"Manufacturer: " << getWMIProperty(obj, L"Manufacturer") << std::endl;
        std::wcout << L"Capacity: " << getWMIProperty(obj, L"Capacity") << L" bytes" << std::endl;
        std::wcout << L"Speed: " << getWMIProperty(obj, L"Speed") << L" MHz" << std::endl;
        std::wcout << L"---" << std::endl;
    }
    for (auto obj : results)
        obj->Release();
}

void getDiskInfo(IWbemServices *pSvc)
{
    std::wcout << L"\n=== Disk Information ===" << std::endl;
    auto results = executeWMIQuery(pSvc, L"SELECT * FROM Win32_DiskDrive");
    for (auto obj : results)
    {
        std::wcout << L"Model: " << getWMIProperty(obj, L"Model") << std::endl;
        std::wcout << L"Size: " << getWMIProperty(obj, L"Size") << L" bytes" << std::endl;
        std::wcout << L"Interface Type: " << getWMIProperty(obj, L"InterfaceType") << std::endl;
        std::wcout << L"---" << std::endl;
    }
    for (auto obj : results)
        obj->Release();
}

void getGraphicsInfo(IWbemServices *pSvc)
{
    std::wcout << L"\n=== Graphics Information ===" << std::endl;
    auto results = executeWMIQuery(pSvc, L"SELECT * FROM Win32_VideoController");
    for (auto obj : results)
    {
        std::wcout << L"Name: " << getWMIProperty(obj, L"Name") << std::endl;
        std::wcout << L"Adapter RAM: " << getWMIProperty(obj, L"AdapterRAM") << L" bytes" << std::endl;
        std::wcout << L"Driver Version: " << getWMIProperty(obj, L"DriverVersion") << std::endl;
        std::wcout << L"---" << std::endl;
    }
    for (auto obj : results)
        obj->Release();
}

void getNetworkInfo(IWbemServices *pSvc)
{
    std::wcout << L"\n=== Network Adapter Information ===" << std::endl;
    auto results = executeWMIQuery(pSvc, L"SELECT * FROM Win32_NetworkAdapter WHERE PhysicalAdapter=True");
    for (auto obj : results)
    {
        std::wcout << L"Name: " << getWMIProperty(obj, L"Name") << std::endl;
        std::wcout << L"Manufacturer: " << getWMIProperty(obj, L"Manufacturer") << std::endl;
        std::wcout << L"MAC Address: " << getWMIProperty(obj, L"MACAddress") << std::endl;
        std::wcout << L"---" << std::endl;
    }
    for (auto obj : results)
        obj->Release();
}

void getBIOSInfo(IWbemServices *pSvc)
{
    std::wcout << L"\n=== BIOS Information ===" << std::endl;
    auto results = executeWMIQuery(pSvc, L"SELECT * FROM Win32_BIOS");
    if (!results.empty())
    {
        std::wcout << L"Manufacturer: " << getWMIProperty(results[0], L"Manufacturer") << std::endl;
        std::wcout << L"Version: " << getWMIProperty(results[0], L"Version") << std::endl;
        std::wcout << L"Release Date: " << getWMIProperty(results[0], L"ReleaseDate") << std::endl;
    }
    for (auto obj : results)
        obj->Release();
}

void getMotherboardInfo(IWbemServices *pSvc)
{
    std::wcout << L"\n=== Motherboard Information ===" << std::endl;
    auto results = executeWMIQuery(pSvc, L"SELECT * FROM Win32_BaseBoard");
    if (!results.empty())
    {
        std::wcout << L"Manufacturer: " << getWMIProperty(results[0], L"Manufacturer") << std::endl;
        std::wcout << L"Product: " << getWMIProperty(results[0], L"Product") << std::endl;
        std::wcout << L"Version: " << getWMIProperty(results[0], L"Version") << std::endl;
    }
    for (auto obj : results)
        obj->Release();
}

void getSoundInfo(IWbemServices *pSvc)
{
    std::wcout << L"\n=== Sound Device Information ===" << std::endl;
    auto results = executeWMIQuery(pSvc, L"SELECT * FROM Win32_SoundDevice");
    for (auto obj : results)
    {
        std::wcout << L"Name: " << getWMIProperty(obj, L"Name") << std::endl;
        std::wcout << L"Manufacturer: " << getWMIProperty(obj, L"Manufacturer") << std::endl;
        std::wcout << L"---" << std::endl;
    }
    for (auto obj : results)
        obj->Release();
}

void getUSBInfo(IWbemServices *pSvc)
{
    std::wcout << L"\n=== USB Controller Information ===" << std::endl;
    auto results = executeWMIQuery(pSvc, L"SELECT * FROM Win32_USBController");
    for (auto obj : results)
    {
        std::wcout << L"Name: " << getWMIProperty(obj, L"Name") << std::endl;
        std::wcout << L"Manufacturer: " << getWMIProperty(obj, L"Manufacturer") << std::endl;
        std::wcout << L"---" << std::endl;
    }
    for (auto obj : results)
        obj->Release();
}

// Global variables
HWND hEdit;
std::wstringstream outputStream;

// Forward declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Function to create the main window
HWND CreateMainWindow(HINSTANCE hInstance)
{
    const wchar_t CLASS_NAME[] = L"Hardware Info Window Class";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Hardware Information",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL,
        NULL,
        hInstance,
        NULL);

    return hwnd;
}

// Function to create the edit control (text area)
HWND CreateEditControl(HWND hwndParent)
{
    HWND hEdit = CreateWindowExW(
        WS_EX_CLIENTEDGE,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_READONLY,
        0, 0, 0, 0,
        hwndParent,
        NULL,
        (HINSTANCE)GetWindowLongPtrW(hwndParent, GWLP_HINSTANCE),
        NULL);

    // Set a monospaced font for better formatting
    HFONT hFont = CreateFontW(0, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
                              OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                              DEFAULT_PITCH | FF_DONTCARE, L"Consolas");
    SendMessageW(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

    return hEdit;
}

// Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        hEdit = CreateEditControl(hwnd);
        return 0;

    case WM_SIZE:
        // Resize the edit control to fill the client area
        if (hEdit)
        {
            RECT rcClient;
            GetClientRect(hwnd, &rcClient);
            MoveWindow(hEdit, 0, 0, rcClient.right, rcClient.bottom, TRUE);
        }
        return 0;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

// Custom wcout that writes to our stringstream
class custom_wbuf : public std::wstreambuf
{
public:
    custom_wbuf(std::wstringstream &stream) : output_stream(stream) {}

protected:
    virtual std::streamsize xsputn(const wchar_t *s, std::streamsize n)
    {
        output_stream.write(s, n);
        return n;
    }

    virtual int_type overflow(int_type c)
    {
        if (c != EOF)
        {
            output_stream.put(wchar_t(c));
        }
        return c;
    }

private:
    std::wstringstream &output_stream;
};

// Function to replace '\n' with '\r\n' for proper display in the Edit control
std::wstring FormatForEditControl(const std::wstring &input)
{
    std::wstring output;
    for (wchar_t c : input)
    {
        if (c == L'\n')
        {
            output += L"\r\n";
        }
        else
        {
            output += c;
        }
    }
    return output;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Redirect wcout to our stringstream
    custom_wbuf custom_buf(outputStream);
    std::wstreambuf *old_buf = std::wcout.rdbuf(&custom_buf);

    HWND hwnd = CreateMainWindow(hInstance);
    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Initialize COM and WMI
    HRESULT hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres))
    {
        std::wcout << L"Failed to initialize COM library. Error code = 0x" << std::hex << hres << std::endl;
        return 1;
    }

    hres = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL);

    if (FAILED(hres))
    {
        std::wcout << L"Failed to initialize security. Error code = 0x" << std::hex << hres << std::endl;
        CoUninitialize();
        return 1;
    }

    IWbemLocator *pLoc = NULL;
    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&pLoc);
    if (FAILED(hres))
    {
        std::wcout << L"Failed to create IWbemLocator object. Error code = 0x" << std::hex << hres << std::endl;
        CoUninitialize();
        return 1;
    }

    IWbemServices *pSvc = NULL;
    hres = pLoc->ConnectServer(
        SysAllocString(L"ROOT\\CIMV2"),
        NULL,
        NULL,
        0, // Changed from NULL to 0
        NULL,
        0,
        0,
        &pSvc);

    if (FAILED(hres))
    {
        std::wcout << L"Could not connect to WMI. Error code = 0x" << std::hex << hres << std::endl;
        pLoc->Release();
        CoUninitialize();
        return 1;
    }

    hres = CoSetProxyBlanket(
        pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE);

    if (FAILED(hres))
    {
        std::wcout << L"Could not set proxy blanket. Error code = 0x" << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1;
    }

    // Get hardware information
    getSystemInfo(pSvc);
    getProcessorInfo(pSvc);
    getMemoryInfo(pSvc);
    getDiskInfo(pSvc);
    getGraphicsInfo(pSvc);
    getNetworkInfo(pSvc);
    getBIOSInfo(pSvc);
    getMotherboardInfo(pSvc);
    getSoundInfo(pSvc);
    getUSBInfo(pSvc);

    // Format the text for proper display in the Edit control
    std::wstring formattedText = FormatForEditControl(outputStream.str());

    // Set the text of the edit control
    SetWindowTextW(hEdit, formattedText.c_str());

    // Cleanup
    pSvc->Release();
    pLoc->Release();
    CoUninitialize();

    // Restore the original wcout buffer
    std::wcout.rdbuf(old_buf);

    // Message loop
    MSG msg = {};
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
