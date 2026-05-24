//
// Created by Вадим on 24.05.2026.
//

#include "CytexLab-Platfrom.Operators.h"
#include "CytexLab-Platform.SysCalls.Linux.h"
#include "SysCallsImpl.h"
#include "KernelLinuxCalls.h"

CytexLab::SysCalls::Interface::ISysCalls *CytexLab::SysCalls::Linux::CreateInterface() {
    LPVOID mem = reinterpret_cast<LPVOID>(syscall6(SYS_mmap, 0, sizeof(SysCallsLinuxImpl), PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    return new (mem) SysCallsLinuxImpl();
}

void CytexLab::SysCalls::Linux::DestroyInterface(Interface::ISysCalls *Interface) {
    Interface->DeInit();
    syscall2(SYS_munmap, reinterpret_cast<INT64>(Interface), sizeof(SysCallsLinuxImpl));
}