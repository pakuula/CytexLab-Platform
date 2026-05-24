//
// Created by Вадим on 22.05.2026.
//

#ifndef CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_TEST_WINDOWS_CYTEXLAB_PLATFORM_MEMFUNCTIONS_H
#define CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_TEST_WINDOWS_CYTEXLAB_PLATFORM_MEMFUNCTIONS_H

extern "C" void* memset(void* dest, int c, unsigned long long count);
extern "C" void  memcpy(void* dest, const void* source, unsigned long long count);

#endif //CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_TEST_WINDOWS_CYTEXLAB_PLATFORM_MEMFUNCTIONS_H
