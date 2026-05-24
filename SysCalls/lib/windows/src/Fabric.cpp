//
// Created by Вадим on 24.05.2026.
//

#include "CytexLab-Platform.SysCalls.Windows.h"
#include "CytexLab-Platfrom.Operators.h"
#include "SysCallsImpl.h"

CytexLab::SysCalls::Interface::ISysCalls *CytexLab::SysCalls::Windows::CreateInterface() {
    HANDLE heap = ::GetProcessHeap();
    LPVOID mem = ::HeapAlloc(heap, 0, sizeof(SysCallsWindowsImpl));
    return new (mem) SysCallsWindowsImpl();
}

void CytexLab::SysCalls::Windows::DestroyInterface(Interface::ISysCalls *Interface) {
    Interface->DeInit();
    HANDLE heap = ::GetProcessHeap();
    ::HeapFree(heap, 0, Interface);
}