//
// Created by Вадим on 24.05.2026.
//

#include "FileImpl.h"
#include "WindowsImports.h"
#include "CytexLab-Platform.UnicodeProcessor.h"

INT64 FileWindowsImpl::Read(LPVOID Buffer, INT64 BufferSize) {
    if (Buffer == NULLPTR) return -1;

    UINT32 readed = 0;
    BOOL success = ::ReadFile(this->nativeHandle, Buffer, BufferSize, &readed, 0);

    if (success == TRUE) return readed;
    else return -2;
}

INT64 FileWindowsImpl::Read(LPECHAR Buffer, INT64 BufferSize) {
    if (Buffer == NULLPTR) return -1;

    HANDLE heap = ::GetProcessHeap();
    LPVOID mem = ::HeapAlloc(heap, 0, BufferSize * 2);

    BOOL success = ::ReadFile(this->nativeHandle, mem, BufferSize * 2, 0, 0);

    if (success == FALSE) {
        ::HeapFree(heap, 0, mem);
        return -2;
    }

    CytexLab::UnlimetedUnicodeProcessor::ReturnData result = CytexLab::UnlimetedUnicodeProcessor::ToUTF32(static_cast<LPCUINT8>(mem), Buffer);

    ::HeapFree(heap, 0, mem);
    return result.SymbolsCount;
}

INT64 FileWindowsImpl::Write(LPCVOID Buffer, INT64 BufferSize) {
    if (Buffer == NULLPTR) return -1;

    UINT32 written = 0;
    BOOL success = ::WriteFile(this->nativeHandle, Buffer, BufferSize, &written, 0);

    if (success == TRUE) return written;
    else return -2;
}

INT64 FileWindowsImpl::Write(LPCECHAR Buffer, INT64 BufferSize) {
    if (Buffer == NULLPTR) return -1;

    HANDLE heap = ::GetProcessHeap();
    LPUINT8 buf = static_cast<LPUINT8>(::HeapAlloc(heap, 0, BufferSize * 4));

    CytexLab::UnlimetedUnicodeProcessor::ReturnData result = CytexLab::UnlimetedUnicodeProcessor::ToUTF8(Buffer, buf);

    if (result.SymbolsCount == 0 || result.BytesCount == 0) {
        ::HeapFree(heap, 0, buf);
        return -2;
    }

    UINT32 written = 0;
    BOOL success = ::WriteFile(this->nativeHandle, buf, result.BytesCount, &written, 0);

    ::HeapFree(heap, 0, buf);

    if (success == FALSE) return -3;
    else return written;
}

INT64 FileWindowsImpl::Seek(INT64 Interrupt, INT64 Pos) {
    UINT32 type = -1;

    if (Interrupt == CytexLab::SysCalls::Interface::IFileSeekInterrupt::Start) type = FILE_BEGIN;
    else if (Interrupt == CytexLab::SysCalls::Interface::IFileSeekInterrupt::Current) type = FILE_CURRENT;
    else if (Interrupt == CytexLab::SysCalls::Interface::IFileSeekInterrupt::End) type = FILE_END;
    else return -1;

    LARGE_INTEGER li;
    LARGE_INTEGER nli;
    li.QuadPart = Pos;
    ::SetFilePointerEx(this->nativeHandle, li, &nli, type);

    return nli.QuadPart;
}

void FileWindowsImpl::Close() {
    ::CloseHandle(this->nativeHandle);
}

INT64 FileWindowsImpl::GetSize() {
    INT64 curPos = this->Seek(CytexLab::SysCalls::Interface::IFileSeekInterrupt::Current, 0);
    INT64 size = this->Seek(CytexLab::SysCalls::Interface::IFileSeekInterrupt::End, 0);
    this->Seek(CytexLab::SysCalls::Interface::IFileSeekInterrupt::Start, curPos);
    return size;
}

BOOL FileWindowsImpl::IsEOF() {
    INT64 curPos = this->Seek(CytexLab::SysCalls::Interface::IFileSeekInterrupt::Current, 0);
    INT64 size = this->Seek(CytexLab::SysCalls::Interface::IFileSeekInterrupt::End, 0);
    this->Seek(CytexLab::SysCalls::Interface::IFileSeekInterrupt::Start, curPos);
    return curPos == size;
}

void FileWindowsImpl::Flush() {
    ::FlushFileBuffers(this->nativeHandle);
}