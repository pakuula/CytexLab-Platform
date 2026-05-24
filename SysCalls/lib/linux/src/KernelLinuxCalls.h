//
// Created by Вадим on 24.05.2026.
//

#ifndef CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_LIB_WINDOWS_KERNELLINUXCALLS_H
#define CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_LIB_WINDOWS_KERNELLINUXCALLS_H

#include "CytexLab-Platform.Types.h"

// Системные вызовы (из syscall_64.tbl)
#define SYS_read        0
#define SYS_write       1
#define SYS_open        2
#define SYS_close       3
#define SYS_lseek       8
#define SYS_mmap        9
#define SYS_munmap      11
#define SYS_brk         12
#define SYS_exit        60
#define SYS_fsync 74

// Флаги для open (из fcntl.h и sys/stat.h)
#define O_RDONLY        0
#define O_WRONLY        1
#define O_RDWR          2
#define O_CREAT         64
#define O_TRUNC         512

// Режимы для open (права доступа)
#define S_IRWXU         448     // 0700
#define S_IRUSR         256     // 0400
#define S_IWUSR         128     // 0200
#define S_IRGRP         32      // 0040

// Протекты для mmap (из sys/mman.h)
#define PROT_NONE       0
#define PROT_READ       1
#define PROT_WRITE      2
#define PROT_EXEC       4

// Флаги для mmap
#define MAP_PRIVATE     2
#define MAP_ANONYMOUS   32

// whence для lseek
#define SEEK_SET        0
#define SEEK_CUR        1
#define SEEK_END        2

INT64 syscall6(INT64 Instruction, INT64 arg1, INT64 arg2, INT64 arg3, INT64 arg4, INT64 arg5, INT64 arg6);
INT64 syscall3(INT64 Instruction, INT64 arg1, INT64 arg2, INT64 arg3);
INT64 syscall2(INT64 Instruction, INT64 arg1, INT64 arg2);
INT64 syscall1(INT64 Instruction, INT64 arg1);

#endif //CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_LIB_WINDOWS_KERNELLINUXCALLS_H
