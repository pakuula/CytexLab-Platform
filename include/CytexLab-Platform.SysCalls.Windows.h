//
// Created by Вадим on 21.05.2026.
//

#ifndef CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_CYTEXLAB_PLATFORM_SYSCALLS_WINDOWS_H
#define CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_CYTEXLAB_PLATFORM_SYSCALLS_WINDOWS_H

#include <CytexLab-Platform.SysCalls.Interface.h>

namespace CytexLab {
    namespace SysCalls {
        namespace Windows {
            Interface::ISysCalls* CreateInterface();
            void DestroyInterface(Interface::ISysCalls* Interface);
        }
    }
}

#endif //CYTEXLAB_CYTEXLAB_PLATFORM_SYSCALLS_CYTEXLAB_PLATFORM_SYSCALLS_WINDOWS_H
