//
// Created by Вадим on 24.05.2026.
//

#ifndef CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_LIB_WINDOWS_SYSCALLSIMPL_H
#define CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_LIB_WINDOWS_SYSCALLSIMPL_H

#include "CytexLab-Platform.Types.h"
#include "CytexLab-Platform.SysCalls.Interface.h"
#include "HeapWindows.h"

class SysCallsWindowsImpl : public CytexLab::SysCalls::Interface::ISysCalls {
private:
    HANDLE hStdIn = static_cast<HANDLE>(NULLPTR);
    HANDLE hStdOut = static_cast<HANDLE>(NULLPTR);
    HANDLE hStdErr = static_cast<HANDLE>(NULLPTR);
    BOOL Inited = FALSE;

    Allacator allacator;

public:
    void Init() override;
    void DeInit() override;
    HANDLE GetStdHandle(INT64 Handle) override;
    void Close(INT64 Code) override;
    CytexLab::SysCalls::Interface::IMemBlock Heap(UINT64 Size) override;
    LPVOID GetMemory(CytexLab::SysCalls::Interface::IMemBlock& MemBlock) override;
    void Free(CytexLab::SysCalls::Interface::IMemBlock& MemBlock) override;
    INT64 WriteConsole(HANDLE hOut, LPCECHAR Buffer) override;
    INT64 ReadConsole(HANDLE hIn, LPECHAR Buffer, INT64 BufferSize) override;
    CytexLab::SysCalls::Interface::IFile* OpenFile(LPCECHAR FileName, INT64 Mode) override;
};

#endif //CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_LIB_WINDOWS_SYSCALLSIMPL_H
