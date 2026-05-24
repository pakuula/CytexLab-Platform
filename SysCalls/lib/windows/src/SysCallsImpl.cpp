//
// Created by Вадим on 24.05.2026.
//

#include "SysCallsImpl.h"
#include "FileImpl.h"
#include "CytexLab-Platform.UnicodeProcessor.h"
#include "CytexLab-Platfrom.Operators.h"

void SysCallsWindowsImpl::DeInit() {
    this->allacator.DeInit();
}

void SysCallsWindowsImpl::Init() {
    this->hStdIn = ::GetStdHandle(STD_INPUT_HANDLE);
    this->hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
    this->hStdErr = ::GetStdHandle(STD_ERROR_HANDLE);
    this->Inited = TRUE;

    this->allacator.Init();
}

HANDLE SysCallsWindowsImpl::GetStdHandle(INT64 Handle) {
    if (!this->Inited) return static_cast<HANDLE>(NULLPTR);

    if (Handle == CytexLab::SysCalls::Interface::IStdHandles::Output)
        return this->hStdOut;
    else if (Handle == CytexLab::SysCalls::Interface::IStdHandles::Input)
        return this->hStdIn;
    else if (Handle == CytexLab::SysCalls::Interface::IStdHandles::Error)
        return this->hStdErr;
    else
        return static_cast<HANDLE>(NULLPTR);
}

void SysCallsWindowsImpl::Close(INT64 Code) {
    ::ExitProcess(Code);
}

CytexLab::SysCalls::Interface::IMemBlock SysCallsWindowsImpl::Heap(UINT64 Size) {
    return this->allacator.Heap(Size);
}

LPVOID SysCallsWindowsImpl::GetMemory(CytexLab::SysCalls::Interface::IMemBlock &MemBlock) {
    return this->allacator.GetMem(MemBlock);
}

void SysCallsWindowsImpl::Free(CytexLab::SysCalls::Interface::IMemBlock &MemBlock) {
    this->allacator.Free(MemBlock);
}

INT64 SysCallsWindowsImpl::WriteConsole(HANDLE hOut, LPCECHAR Buffer) {
    if (hOut == NULLPTR) return -1;
    if (Buffer == NULLPTR) return -2;

    UINT32 written = 0;

    while (*Buffer != 0) {
        UINT16 chr[2];
        INT8 num = CytexLab::UnicodeProcessor::ToUTF16(Buffer, reinterpret_cast<LPUINT16>(&chr));

        if (num <= 0) return -3;

        BOOL success = ::WriteConsoleW(hOut, &chr[0], num, 0, NULLPTR);

        if (success == FALSE) {
            return -4;
        }

        written++;
        Buffer++;
    }

    return written;
}

INT64 SysCallsWindowsImpl::ReadConsole(HANDLE hIn, LPECHAR Buffer, INT64 BufferSize) {
    if (hIn == NULLPTR) return -1;
    if (Buffer == NULLPTR) return -2;

    CytexLab::SysCalls::Interface::IMemBlock mem = this->Heap(BufferSize * 2);
    WCHAR* buffer = static_cast<WCHAR*>(this->GetMemory(mem));

    UINT32 readed = 0;
    BOOL success = ::ReadConsoleW(hIn, buffer, BufferSize, &readed, 0);

    while (readed > 0 && buffer[readed-1] == L'\n' || buffer[readed-1] == L'\r') readed--;
    buffer[readed] = L'\0';

    if (success) {
        CytexLab::UnlimetedUnicodeProcessor::ReturnData result = CytexLab::UnlimetedUnicodeProcessor::ToUTF32(buffer, Buffer);

        if (result.SymbolsCount == 0 || result.BytesCount == 0) {
            this->Free(mem);
            return -3;
        } else {
            this->Free(mem);
            return result.SymbolsCount;
        }
    } else {
        this->Free(mem);
        return -4;
    }
}

CytexLab::SysCalls::Interface::IFile *SysCallsWindowsImpl::OpenFile(LPCECHAR FileName, INT64 Mode) {
    UINT32 dwDesiredAccess = 0;
    UINT32 dwCreationDisposition = 0;

    if (Mode == CytexLab::SysCalls::Interface::IFileOpenMode::Read) {
        dwDesiredAccess = GENERIC_READ;
        dwCreationDisposition = OPEN_EXISTING;
    }
    else if (Mode == CytexLab::SysCalls::Interface::IFileOpenMode::Write) {
        dwDesiredAccess = GENERIC_WRITE;
        dwCreationDisposition = CREATE_ALWAYS;
    }
    else if (Mode == CytexLab::SysCalls::Interface::IFileOpenMode::ReadWrite) {
        dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
        dwCreationDisposition = OPEN_ALWAYS;
    }
    else return (CytexLab::SysCalls::Interface::IFile*)NULLPTR;

    UINT32 dwShareMode = FILE_SHARE_READ;
    UINT32 dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;

    HANDLE heap = ::GetProcessHeap();
    LPUINT16 mem = static_cast<LPUINT16>(::HeapAlloc(heap, 0, 260 * sizeof(UINT16)));

    CytexLab::UnlimetedUnicodeProcessor::ReturnData result = CytexLab::UnlimetedUnicodeProcessor::ToUTF16(FileName, mem);

    if (result.SymbolsCount == 0 || result.BytesCount == 0) {
        ::HeapFree(heap, 0, mem);
        return (CytexLab::SysCalls::Interface::IFile*)-1;
    }

    HANDLE hFile = ::CreateFileW(mem, dwDesiredAccess, dwShareMode, 0, dwCreationDisposition, dwFlagsAndAttributes, 0);

    if (hFile == (HANDLE)-1) {
        ::HeapFree(heap, 0, mem);
        return (CytexLab::SysCalls::Interface::IFile*)-1;
    }

    LPVOID memFile = ::HeapAlloc(heap, 0, sizeof(FileWindowsImpl));
    FileWindowsImpl* file = new (memFile) FileWindowsImpl(hFile);

    ::HeapFree(heap, 0, mem);
    return static_cast<CytexLab::SysCalls::Interface::IFile*>(file);
}

void SysCallsWindowsImpl::DestroyFile(CytexLab::SysCalls::Interface::IFile *File) {
    if (File == NULLPTR) return;

    File->Close();

    HANDLE heap = ::GetProcessHeap();
    ::HeapFree(heap, 0, File);
}