//
// Created by Вадим on 24.05.2026.
//

#include "FileImpl.h"
#include "KernelLinuxCalls.h"
#include "CytexLab-Platform.UnicodeProcessor.h"

INT64 FileLinuxImpl::Read(LPVOID Buffer, INT64 BufferSize) {
    if (Buffer == NULLPTR) return -1;

    INT64 readed;
    readed = syscall3(SYS_read, this->nativeHandle, reinterpret_cast<INT64>(Buffer), BufferSize);

    if (readed >= 0) return readed;
    else return -2;
}

INT64 FileLinuxImpl::Read(LPECHAR Buffer, INT64 BufferSize) {
    if (Buffer == NULLPTR) return -1;

    LPVOID mem = reinterpret_cast<LPVOID>(syscall6(SYS_mmap, 0, BufferSize * 4, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));

    BOOL success = syscall3(SYS_read, this->nativeHandle, reinterpret_cast<INT64>(mem), BufferSize * 4);

    if (success == FALSE) {
        syscall2(SYS_munmap, reinterpret_cast<INT64>(mem), BufferSize * 4);
        return -2;
    }

    CytexLab::UnlimetedUnicodeProcessor::ReturnData result = CytexLab::UnlimetedUnicodeProcessor::ToUTF32(static_cast<LPCUINT8>(mem), Buffer);

    syscall2(SYS_munmap, reinterpret_cast<INT64>(mem), BufferSize * 4);
    return result.SymbolsCount;
}

INT64 FileLinuxImpl::Write(LPCVOID Buffer, INT64 BufferSize) {
    if (Buffer == NULLPTR) return -1;

    UINT32 written;
    written = syscall3(SYS_write, this->nativeHandle, reinterpret_cast<INT64>(Buffer), BufferSize);

    if (written >= 0) return written;
    else return -2;
}

INT64 FileLinuxImpl::Write(LPCECHAR Buffer, INT64 BufferSize) {
    if (Buffer == NULLPTR) return -1;

    //LPUINT8 buf = static_cast<LPUINT8>(::HeapAlloc(heap, 0, BufferSize * 4));
    LPUINT8 buf = reinterpret_cast<LPUINT8>(syscall6(SYS_mmap, 0, BufferSize * 4, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0));

    CytexLab::UnlimetedUnicodeProcessor::ReturnData result = CytexLab::UnlimetedUnicodeProcessor::ToUTF8(Buffer, buf);

    if (result.SymbolsCount == 0 || result.BytesCount == 0) {
        syscall2(SYS_munmap, reinterpret_cast<INT64>(buf), BufferSize * 4);
        return -2;
    }

    UINT32 written = 0;
    BOOL success = syscall3(SYS_write, this->nativeHandle, reinterpret_cast<INT64>(buf), result.BytesCount);

    syscall2(SYS_munmap, reinterpret_cast<INT64>(buf), BufferSize * 4);

    if (success == FALSE) return -3;
    else return written;
}

INT64 FileLinuxImpl::Seek(INT64 Interrupt, INT64 Pos) {
    UINT32 type = -1;

    if (Interrupt == CytexLab::SysCalls::Interface::IFileSeekInterrupt::Start) type = SEEK_SET;
    else if (Interrupt == CytexLab::SysCalls::Interface::IFileSeekInterrupt::Current) type = SEEK_CUR;
    else if (Interrupt == CytexLab::SysCalls::Interface::IFileSeekInterrupt::End) type = SEEK_END;
    else return -1;

    INT64 newPos = syscall3(SYS_lseek, this->nativeHandle, Pos, type);

    return newPos;
}

void FileLinuxImpl::Close() {
    syscall1(SYS_close, this->nativeHandle);
}

INT64 FileLinuxImpl::GetSize() {
    INT64 curPos = this->Seek(CytexLab::SysCalls::Interface::IFileSeekInterrupt::Current, 0);
    INT64 size = this->Seek(CytexLab::SysCalls::Interface::IFileSeekInterrupt::End, 0);
    this->Seek(CytexLab::SysCalls::Interface::IFileSeekInterrupt::Start, curPos);
    return size;
}

BOOL FileLinuxImpl::IsEOF() {
    INT64 curPos = this->Seek(CytexLab::SysCalls::Interface::IFileSeekInterrupt::Current, 0);
    INT64 size = this->Seek(CytexLab::SysCalls::Interface::IFileSeekInterrupt::End, 0);
    this->Seek(CytexLab::SysCalls::Interface::IFileSeekInterrupt::Start, curPos);
    return curPos == size;
}

void FileLinuxImpl::Flush() {
    syscall1(SYS_fsync, this->nativeHandle);
}