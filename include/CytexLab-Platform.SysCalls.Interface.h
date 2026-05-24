//
// Created by Вадим on 21.05.2026.
//

#ifndef CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_CYTEXLAB_PLATFORM_SYSCALLS_INTERFACE_H
#define CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_CYTEXLAB_PLATFORM_SYSCALLS_INTERFACE_H

#include <CytexLab-Platform.Types.h>

namespace CytexLab {
    namespace SysCalls {
        namespace Interface {
            class IStdHandles {
            public:
                const static INT64 Input = 0;
                const static INT64 Output = 1;
                const static INT64 Error = 2;
            };

            typedef struct {
                INT64 BlockID;
                UINT64 Offset;
                UINT64 Size;
            } IMemBlock;

            class IFileSeekInterrupt {
            public:
                const static INT64 Start = 0;
                const static INT64 Current = 1;
                const static INT64 End = 2;
            };

            class IFile {
            public:
                virtual INT64 Read(LPECHAR Buffer, INT64 BufferSize) = 0;
                virtual INT64 Read(LPVOID Buffer, INT64 BufferSize) = 0;
                virtual INT64 Write(LPCECHAR Buffer, INT64 BufferSize) = 0;
                virtual INT64 Write(LPCVOID Buffer, INT64 BufferSize) = 0;
                virtual INT64 Seek(INT64 Interrupt, INT64 Pos) = 0;
                virtual void Close() = 0;
                virtual INT64 GetSize() = 0;
                virtual BOOL IsEOF() = 0;
                virtual void Flush() = 0;
            };

            class IFileOpenMode {
            public:
                const static INT64 Read = 0;
                const static INT64 Write = 1;
                const static INT64 ReadWrite = 2;
            };

            class ISysCalls {
            public:
                virtual void Init() = 0;
                virtual void DeInit() = 0;
                virtual void Close(INT64 Code) = 0;
                virtual HANDLE GetStdHandle(INT64 Handle) = 0;
                virtual IMemBlock Heap(UINT64 Size) = 0;
                virtual LPVOID GetMemory(IMemBlock& MemBlock) = 0;
                virtual void Free(IMemBlock& MemBlock) = 0;
                virtual INT64 WriteConsole(HANDLE hOut, LPCECHAR Buffer) = 0;
                virtual INT64 ReadConsole(HANDLE hIn, LPECHAR Buffer, INT64 BufferSize) = 0;
                virtual IFile* OpenFile(LPCECHAR FileName, INT64 Mode) = 0;
                virtual void DestroyFile(IFile* File) = 0;
            };
        }
    }
}

#endif //CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_CYTEXLAB_PLATFORM_SYSCALLS_INTERFACE_H
