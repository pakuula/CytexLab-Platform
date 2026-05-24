//
// Created by Вадим on 24.05.2026.
//

#include "KernelLinuxCalls.h"

INT64 syscall6(INT64 n, INT64 a1, INT64 a2, INT64 a3, INT64 a4, INT64 a5, INT64 a6) {
    register INT64 rax __asm__("rax") = n;
    register INT64 rdi __asm__("rdi") = a1;
    register INT64 rsi __asm__("rsi") = a2;
    register INT64 rdx __asm__("rdx") = a3;
    register INT64 r10 __asm__("r10") = a4;
    register INT64 r8  __asm__("r8")  = a5;
    register INT64 r9  __asm__("r9")  = a6;
    register INT64 ret __asm__("rax");

    __asm__ volatile (
            "syscall"
            : "=r"(ret)
            : "r"(rax), "r"(rdi), "r"(rsi), "r"(rdx), "r"(r10), "r"(r8), "r"(r9)
            : "rcx", "r11", "memory"
            );
    return ret;
}

INT64 syscall3(INT64 Instruction, INT64 arg1, INT64 arg2, INT64 arg3) {
    return syscall6(Instruction, arg1, arg2, arg3, 0, 0, 0);
}

INT64 syscall2(INT64 Instruction, INT64 arg1, INT64 arg2) {
    return syscall6(Instruction, arg1, arg2, 0, 0, 0, 0);
}

INT64 syscall1(INT64 Instruction, INT64 arg1) {
    return syscall6(Instruction, arg1, 0, 0, 0, 0, 0);
}