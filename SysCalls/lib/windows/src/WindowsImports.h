//
// Created by Вадим on 24.05.2026.
//

#ifndef CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_LIB_WINDOWS_WINDOWSIMPORTS_H
#define CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_LIB_WINDOWS_WINDOWSIMPORTS_H

#include "CytexLab-Platform.Types.h"

#define WINAPI __stdcall

#define STD_OUTPUT_HANDLE ((INT32)-11)
#define STD_INPUT_HANDLE ((INT32)-10)
#define STD_ERROR_HANDLE ((INT32)-12)
#define GENERIC_READ 0x80000000
#define GENERIC_WRITE 0x40000000
#define FILE_SHARE_READ 0x00000001
#define CREATE_ALWAYS 2
#define OPEN_EXISTING 3
#define OPEN_ALWAYS 4
#define FILE_ATTRIBUTE_NORMAL 0x80
#define FILE_BEGIN 0
#define FILE_CURRENT 1
#define FILE_END 2

typedef union _LARGE_INTEGER {
    struct {
        INT32 LowPart;
        INT32 HighPart;
    } DUMMYSTRUCTNAME;
    struct {
        INT32 LowPart;
        INT32 HighPart;
    } u;
    INT64 QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

extern "C" {
__declspec(dllimport) HANDLE WINAPI GetStdHandle(INT32 nStdHandle);
__declspec(dllimport) LPVOID WINAPI HeapAlloc(HANDLE hHeap, INT32 dwFlags, INT64 dwBytes);
__declspec(dllimport) HANDLE WINAPI GetProcessHeap();
__declspec(dllimport) BOOL WINAPI HeapFree(HANDLE hHeap, INT32 dwFlags, LPVOID lpMem);
__declspec(dllimport) void WINAPI ExitProcess(INT64 uExitCode);
__declspec(dllimport) BOOL WINAPI WriteConsoleW(HANDLE  hConsoleOutput, LPCUINT16 lpBuffer, UINT32 nNumberOfCharsToWrite, LPUINT32 lpNumberOfCharsWritten, LPVOID lpReserved);
__declspec(dllimport) BOOL WINAPI ReadConsoleW(HANDLE hConsoleInput, LPVOID lpBuffer, UINT32 nNumberOfCharsToRead, LPUINT32 lpNumberOfCharsRead, LPVOID pInputControl);
__declspec(dllimport) HANDLE WINAPI CreateFileW(const WCHAR* lpFileName, UINT32 dwDesiredAccess, UINT32 dwShareMode, LPVOID lpSecurityAttributes, UINT32 dwCreationDisposition, UINT32 dwFlagsAndAttributes, HANDLE hTemplateFile);
__declspec(dllimport) BOOL WINAPI ReadFile(HANDLE hFile, LPVOID lpBuffer, UINT32 nNumberOfBytesToRead, LPUINT32 lpNumberOfBytesRead, LPVOID lpOverlapped);
__declspec(dllimport) BOOL WINAPI WriteFile(HANDLE hFile, LPCVOID lpBuffer, UINT32 nNumberOfBytesToWrite, LPUINT32 lpNumberOfBytesWritten, LPVOID lpOverlapped);
__declspec(dllimport) BOOL WINAPI SetFilePointerEx(HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER lpNewFilePointer, UINT32 dwMoveMethod);
__declspec(dllimport) BOOL WINAPI CloseHandle(HANDLE hObject);
__declspec(dllimport) BOOL WINAPI FlushFileBuffers(HANDLE hFile);
}

#endif //CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_LIB_WINDOWS_WINDOWSIMPORTS_H
