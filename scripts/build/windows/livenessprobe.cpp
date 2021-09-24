// OMSProbeExe.cpp : This file contains the 'main' function. Program execution begins and ends there.

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <Windows.h>
#include <tlhelp32.h>
#include <tchar.h>

#define SUCCESS                            0x00000000
#define NO_FLUENT_BIT_PROCESS              0x00000001
#define FILESYSTEM_WATCHER_FILE_EXISTS     0x00000002
#define CERTIFICATE_RENEWAL_REQUIRED       0x00000003
#define FLUENTDWINAKS_SERVICE_NOT_RUNNING  0x00000004
#define UNEXPECTED_ERROR                   0xFFFFFFFF


bool IsProcessRunning(const wchar_t * const executableName) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (!Process32First(snapshot, &entry)) {
        CloseHandle(snapshot);
        return false;
    }

    do {
        if (!_wcsicmp(entry.szExeFile, executableName)) {
            CloseHandle(snapshot);
            return true;
        }
    } while (Process32Next(snapshot, &entry));

    CloseHandle(snapshot);
    return false;
}


bool IsFileExists(const wchar_t* const fileName)
{
    DWORD dwAttrib = GetFileAttributes(fileName);
    return dwAttrib != INVALID_FILE_SIZE;
}

int GetServiceStatus(const wchar_t * const serivceName)
{
    SC_HANDLE theService, scm;
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwBytesNeeded;


    scm = OpenSCManager(nullptr, nullptr, SC_MANAGER_ENUMERATE_SERVICE);
    if (!scm) {
        return UNEXPECTED_ERROR;
    }

    theService = OpenService(scm, serivceName, SERVICE_QUERY_STATUS);
    if (!theService) {
        CloseServiceHandle(scm);
        return UNEXPECTED_ERROR;
    }

    auto result = QueryServiceStatusEx(theService, SC_STATUS_PROCESS_INFO,
        reinterpret_cast<LPBYTE>(&ssStatus), sizeof(SERVICE_STATUS_PROCESS),
        &dwBytesNeeded);

    CloseServiceHandle(theService);
    CloseServiceHandle(scm);

    if (result == 0) {
        return UNEXPECTED_ERROR;
    }

    return ssStatus.dwCurrentState;
}

int _tmain(int argc, wchar_t * argv[])
{
    wprintf_s(L"INFO:number of passed arguments - %d \n", argc);
    if( argc < 5) {
        wprintf_s(L"ERROR:unexpected number arguments and expected is 5");
        return UNEXPECTED_ERROR;
    }

    if (IsProcessRunning(argv[1]))
    {
        wprintf_s(L"INFO:%s is running\n", argv[1]);
    }
    else
    {
        wprintf_s(L"ERROR: %s is not running\n", argv[1]);
        return NO_FLUENT_BIT_PROCESS;
    }

    DWORD dwStatus = GetServiceStatus(argv[2]);

    if (dwStatus == SERVICE_RUNNING)
    {
        wprintf_s(L"INFO:%s is running\n", argv[2]);
    }
    else
    {
        wprintf_s(L"ERROR: %s is running\n", argv[2]);
        return FLUENTDWINAKS_SERVICE_NOT_RUNNING;
    }

    if (IsFileExists(argv[3]))
    {
        wprintf_s(L"INFO:%s exists which indicates Config Map Updated since agent started.\n", argv[3]);
        return FILESYSTEM_WATCHER_FILE_EXISTS;
    }

    if (IsFileExists(argv[4]))
    {
        wprintf_s(L"INFO:%s exists indicates Certificate needs to be renewed.\n", argv[4]);
        return CERTIFICATE_RENEWAL_REQUIRED;
    }

    return SUCCESS;
}
