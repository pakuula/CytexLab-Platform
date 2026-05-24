//
// Created by Вадим on 24.05.2026.
//

#include "SysCallsImpl.h"
#include "FileImpl.h"
#include "CytexLab-Platform.UnicodeProcessor.h"
#include "CytexLab-Platfrom.Operators.h"
#include "KernelLinuxCalls.h"

void SysCallsLinuxImpl::DeInit() {
    this->allacator.DeInit();
}

void SysCallsLinuxImpl::Init() {
    this->hStdIn = reinterpret_cast<HANDLE>(0);
    this->hStdOut = reinterpret_cast<HANDLE>(1);
    this->hStdErr = reinterpret_cast<HANDLE>(2);
    this->Inited = TRUE;

    this->allacator.Init();
}

HANDLE SysCallsLinuxImpl::GetStdHandle(INT64 Handle) {
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

void SysCallsLinuxImpl::Close(INT64 Code) {
    syscall1(SYS_exit, Code);
}

CytexLab::SysCalls::Interface::IMemBlock SysCallsLinuxImpl::Heap(UINT64 Size) {
    return this->allacator.Heap(Size);
}

LPVOID SysCallsLinuxImpl::GetMemory(CytexLab::SysCalls::Interface::IMemBlock &MemBlock) {
    return this->allacator.GetMem(MemBlock);
}

void SysCallsLinuxImpl::Free(CytexLab::SysCalls::Interface::IMemBlock &MemBlock) {
    this->allacator.Free(MemBlock);
}

INT64 SysCallsLinuxImpl::WriteConsole(HANDLE hOut, LPCECHAR Buffer) {
    if (hOut == NULLPTR) return -1;
    if (Buffer == NULLPTR) return -2;

    UINT32 written = 0;

    while (*Buffer != 0) {
        UINT8 chr[4];
        INT8 num = CytexLab::UnicodeProcessor::ToUTF8(Buffer, reinterpret_cast<LPUINT8>(&chr));

        if (num <= 0) return -3;

        //BOOL success = ::WriteConsoleW(hOut, &chr[0], num, 0, NULLPTR);
        INT64 cW = syscall3(SYS_write, reinterpret_cast<INT64>(hOut), reinterpret_cast<INT64>(&chr), num);

        if (cW < 0) {
            return -4;
        }

        written++;
        Buffer++;
    }

    return written;
}

INT64 SysCallsLinuxImpl::ReadConsole(HANDLE hIn, LPECHAR Buffer, INT64 BufferSize) {
    if (hIn == NULLPTR) return -1;
    if (Buffer == NULLPTR) return -2;

    CytexLab::SysCalls::Interface::IMemBlock mem = this->Heap(BufferSize * 4);
    LPUINT8 buffer = static_cast<LPUINT8>(this->GetMemory(mem));

    INT64 readed;
    readed = syscall3(SYS_read, reinterpret_cast<INT64>(hIn), reinterpret_cast<INT64>(buffer), BufferSize*4);

    if (readed >= 0) {
        while (readed > 0 && buffer[readed-1] == L'\n' || buffer[readed-1] == L'\r') readed--;
        buffer[readed] = L'\0';

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

CytexLab::SysCalls::Interface::IFile* SysCallsLinuxImpl::OpenFile(LPCECHAR FileName, INT64 Mode) {
    // 1. Конвертируем режим доступа в Linux-флаги
    int flags = 0;

    if (Mode == CytexLab::SysCalls::Interface::IFileOpenMode::Read) {
        flags = O_RDONLY;
    }
    else if (Mode == CytexLab::SysCalls::Interface::IFileOpenMode::Write) {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    }
    else if (Mode == CytexLab::SysCalls::Interface::IFileOpenMode::ReadWrite) {
        flags = O_RDWR | O_CREAT;
    }
    else {
        return nullptr;
    }

    // 2. Конвертируем UTF-32 → UTF-8 (путь к файлу)
    char utf8_path[4096];
    auto result = CytexLab::UnlimetedUnicodeProcessor::ToUTF8(FileName, reinterpret_cast<LPUINT8>(utf8_path));

    if (result.SymbolsCount == 0 || result.BytesCount == 0) {
        return static_cast<CytexLab::SysCalls::Interface::IFile*>(NULLPTR);
    }
    utf8_path[result.BytesCount] = '\0';

    // 3. Системный вызов open
    int fd = syscall3(SYS_open, (INT64)utf8_path, flags, 0644);
    if (fd < 0) {
        return nullptr;
    }

    // 4. Выделяем память под объект через mmap (как аналог HeapAlloc в Windows)
    void* mem = (void*)syscall6(SYS_mmap, 0, sizeof(FileLinuxImpl),
                                PROT_READ | PROT_WRITE,
                                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == (void*)-1) {
        syscall1(SYS_close, fd);
        return static_cast<CytexLab::SysCalls::Interface::IFile*>(NULLPTR);
    }

    // 5. Placement new и возврат
    FileLinuxImpl* file = new (mem) FileLinuxImpl(fd);
    return file;
}

void SysCallsLinuxImpl::DestroyFile(CytexLab::SysCalls::Interface::IFile *File) {
    syscall2(SYS_munmap, reinterpret_cast<INT64>(File), sizeof(FileLinuxImpl));
}