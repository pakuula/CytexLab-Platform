//
// Created by Вадим on 24.05.2026.
//

#ifndef CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_LIB_WINDOWS_FILEIMPL_H
#define CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_LIB_WINDOWS_FILEIMPL_H

#include "CytexLab-Platform.Types.h"
#include "CytexLab-Platform.SysCalls.Interface.h"

class FileWindowsImpl : public CytexLab::SysCalls::Interface::IFile {
private:
    HANDLE nativeHandle;

public:
    FileWindowsImpl(HANDLE NativeHandle) : nativeHandle(NativeHandle) {}
    INT64 Read(LPECHAR Buffer, INT64 BufferSize) override;
    INT64 Read(LPVOID Buffer, INT64 BufferSize) override;
    INT64 Write(LPCECHAR Buffer, INT64 BufferSize) override;
    INT64 Write(LPCVOID Buffer, INT64 BufferSize) override;
    INT64 Seek(INT64 Interrupt, INT64 Pos) override;
    void Close() override;
    INT64 GetSize() override;
    BOOL IsEOF() override;
    void Flush() override;
};

#endif //CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_LIB_WINDOWS_FILEIMPL_H
