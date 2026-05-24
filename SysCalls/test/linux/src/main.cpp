//
// Created by Вадим on 24.05.2026.
//

#include "CytexLab-Platform.SysCalls.Linux.h"

extern "C" void startup() {
    using namespace CytexLab::SysCalls;

    Interface::ISysCalls* sysCalls = Linux::CreateInterface();
    sysCalls->Init();

    HANDLE hOut = sysCalls->GetStdHandle(Interface::IStdHandles::Output);
    HANDLE hIn = sysCalls->GetStdHandle(Interface::IStdHandles::Input);

    Interface::IMemBlock memBlock = sysCalls->Heap(512);
    LPECHAR buf = static_cast<LPECHAR>(sysCalls->GetMemory(memBlock));

    sysCalls->WriteConsole(hOut, (LPCECHAR)U"--- START ---\n");
    sysCalls->WriteConsole(hOut, (LPCECHAR)U"Hello, World!\n");
    sysCalls->WriteConsole(hOut, (LPCECHAR)U"--- AFTER HELLO ---\n");

    INT64 readed = sysCalls->ReadConsole(hIn, buf, 511);
    sysCalls->WriteConsole(hOut, (LPCECHAR)U"--- AFTER READ ---\n");
    buf[readed] = U'\0';

    sysCalls->WriteConsole(hOut, (LPCECHAR)U"--- BEFORE OUTPUT ---\n");
    sysCalls->WriteConsole(hOut, &buf[0]);

    sysCalls->Free(memBlock);
    sysCalls->Close(0);
}