//
//
//

#include <CytexLab-Platform.SysCalls.Windows.h>
#include <CytexLab-Platform.UnicodeProcessor.h>

using namespace CytexLab::SysCalls;

extern "C" void startup() {
    Interface::ISysCalls* sysCalls = Windows::CreateInterface();
    sysCalls->Init();

    HANDLE hOut = sysCalls->GetStdHandle(Interface::IStdHandles::Output);

    sysCalls->WriteConsole(hOut, (LPCECHAR)U"Hello, World!\r\n\0");

    sysCalls->Close(0);
    Windows::DestroyInterface(sysCalls);
}