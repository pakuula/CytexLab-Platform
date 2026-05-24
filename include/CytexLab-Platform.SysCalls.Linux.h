//
// Created by Вадим on 24.05.2026.
//

#ifndef CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_LIB_WINDOWS_CYTEXLAB_PLATFORM_SYSCALLS_LINUX_H
#define CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_LIB_WINDOWS_CYTEXLAB_PLATFORM_SYSCALLS_LINUX_H

#include <CytexLab-Platform.SysCalls.Interface.h>

namespace CytexLab {
    namespace SysCalls {
        namespace Linux {
            Interface::ISysCalls* CreateInterface();
            void DestroyInterface(Interface::ISysCalls* Interface);
        }
    }
}

#endif //CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_LIB_WINDOWS_CYTEXLAB_PLATFORM_SYSCALLS_LINUX_H
