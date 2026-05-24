//
// Created by Вадим on 24.05.2026.
//

#ifndef CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_LIB_WINDOWS_HEAPWINDOWS_H
#define CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_LIB_WINDOWS_HEAPWINDOWS_H

#include "WindowsImports.h"
#include "CytexLab-Platform.SysCalls.Interface.h"

#define MAX_ALLOC_BLOCK 256
#define MAX_MEM_BLOCK 128
#define START_MEM_BLOCK_SIZE 2048

typedef struct {
    UINT64 Offset;
    UINT64 Size;
} MemBlockAlloc;

typedef struct {
    LPVOID Pointer;
    UINT64 Size;
    UINT64 Allocated;
    MemBlockAlloc Blocks[MAX_ALLOC_BLOCK];
} MemBlock;

class Allacator {
private:
    MemBlock memBlocks[MAX_MEM_BLOCK];

public:
    void Init();
    void DeInit();

    CytexLab::SysCalls::Interface::IMemBlock Heap(UINT64 Size);
    LPVOID GetMem(CytexLab::SysCalls::Interface::IMemBlock& memBlock);
    void Free(CytexLab::SysCalls::Interface::IMemBlock& memBlock);
};

#endif //CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_LIB_WINDOWS_HEAPWINDOWS_H
