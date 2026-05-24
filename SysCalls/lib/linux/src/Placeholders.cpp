//
// Created by Вадим on 24.05.2026.
//

#include "KernelLinuxCalls.h"

extern "C" void __cxa_pure_virtual() {
    syscall1(SYS_exit, -1);
}