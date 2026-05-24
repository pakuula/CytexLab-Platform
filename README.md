# CytexLab Platform

**Кроссплатформенный C++ фреймворк для embedded и системного программирования без стандартной библиотеки.**

[![License: MPL 2.0](https://img.shields.io/badge/License-MPL%202.0-brightgreen.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20STM32-blue)]()
[![C++](https://img.shields.io/badge/C%2B%2B-14-blue.svg)]()
[![Size](https://img.shields.io/badge/Hello%20World-11KB-green)]()

---

## 🔥 Что это и зачем?

CytexLab Platform — это моя замена стандартной библиотеки для ситуаций, где STL не завезти, а libc — роскошь.

- **Freestanding C++**: `-nostdlib`, свои операторы `new`/`delete`, никакого RTTI, исключений и stack-protector'а.
- **Свой аллокатор**: с дефрагментацией, выравниванием и произвольным освобождением.
- **Unicode-first**: внутри — UTF-32 (O(1) индексация!), снаружи — UTF-8/UTF-16.
- **Кроссплатформенность**: один интерфейс — разные реализации под Windows/Linux/STM32.
- **Минимальный размер**: Hello World с аллокатором, файлами и Unicode — **11 KB** (O3 не может ужать сильнее).

---

## 🎯 Поддерживаемые платформы

| Статус | Платформа | Размер Hello World | Особенности |
|--------|-----------|-------------------|-------------|
| ✅ **Готово** | Windows (AMD64) | **11 KB** | WinAPI через импорты, UTF-16 консоль |
| 🚧 **В разработке** | Linux (AMD64) | — | Прямые syscall'ы, без libc |
| 📋 **Планируется** | STM32H7S7 | — | Портативная консоль, внешняя SDRAM, ChromART (DMA2D) |
| 📋 **Планируется** | AVR (ATmega328P) | — | Статический пуловый аллокатор, работа с UART |

---

## 🧠 Архитектура

```cpp
namespace CytexLab::SysCalls::Interface {

    class ISysCalls {
    public:
        virtual IMemBlock Heap(UINT64 Size) = 0;
        virtual LPVOID GetMemory(IMemBlock& MemBlock) = 0;
        virtual void Free(IMemBlock& MemBlock) = 0;
        
        virtual INT64 WriteConsole(HANDLE hOut, LPCECHAR Buffer) = 0;
        virtual INT64 ReadConsole(HANDLE hIn, LPECHAR Buffer, INT64 BufferSize) = 0;
        
        virtual IFile* OpenFile(LPCECHAR FileName, INT64 Mode) = 0;
        
        virtual void Init() = 0;
        virtual void Close(INT64 Code) = 0;
    };
    
    class IFile {
    public:
        virtual INT64 Read(LPVOID Buffer, INT64 BufferSize) = 0;      // binary
        virtual INT64 Read(LPECHAR Buffer, INT64 BufferSize) = 0;     // text (UTF-32)
        virtual INT64 Write(LPCVOID Buffer, INT64 BufferSize) = 0;    // binary
        virtual INT64 Write(LPCECHAR Buffer, INT64 BufferSize) = 0;   // text (UTF-32)
        virtual INT64 Seek(INT64 Interrupt, INT64 Pos) = 0;
        virtual void Close() = 0;
        virtual INT64 GetSize() = 0;
        virtual BOOL IsEOF() = 0;
        virtual void Flush() = 0;
    };
    
}
```

Реализации для конкретных платформ лежат в `SysCalls/lib/<platform>/src/`. Сейчас полностью готова **Windows** (9 файлов).

---

## 📂 Структура проекта (актуальная)

```
CytexLab-Platform/
├── include/                      # Публичные заголовки
│   ├── CytexLab-Platform.Types.h
│   ├── CytexLab-Platform.SysCalls.Interface.h
│   ├── CytexLab-Platform.SysCalls.Windows.h
│   ├── CytexLab-Platform.UnicodeProcessor.h
│   ├── CytexLab-Platform.MemFunctions.h
│   └── CytexLab-Platfrom.Operators.h          # placement new
│
├── SysCalls/
│   ├── lib/
│   │   └── windows/              # ✅ Готовая реализация для Windows
│   │       ├── CMakeLists.txt
│   │       └── src/
│   │           ├── Fabric.cpp               # CreateInterface/DestroyInterface
│   │           ├── SysCallsImpl.cpp/h       # главный класс SysCallsWindowsImpl
│   │           ├── FileImpl.cpp/h           # FileWindowsImpl (Read/Write/Seek...)
│   │           ├── HeapWindows.cpp/h        # аллокатор (Heap/Free/GetMemory)
│   │           ├── WindowsImports.h         # все __declspec(dllimport)
│   │           └── Placeholders.cpp         # заглушки для линковки
│   └── test/windows/             # Тестовый пример
│       ├── CMakeLists.txt
│       └── src/main.cpp
│
├── MemFunctions/                 # memcpy, memset (freestanding)
│   └── src/MemFunctions.cpp
│
├── UnicodeProcessor/             # конвертация UTF-32 ↔ UTF-8/UTF-16
│   └── src/UnicodeProcessor.cpp
│
├── build/                        # сборочные директории (clang/debug, release)
└── CMakeLists.txt (корневой, если есть – не показан, но подразумевается)
```

---

## 📦 Пример: Hello World с записью в файл

```cpp
#include <CytexLab-Platform.SysCalls.Windows.h>

using namespace CytexLab::SysCalls;

extern "C" void startup() {
    Interface::ISysCalls* sysCalls = Windows::CreateInterface();
    sysCalls->Init();

    HANDLE hOut = sysCalls->GetStdHandle(Interface::IStdHandles::Output);

    // Запись в файл
    Interface::IFile* file = sysCalls->OpenFile(
        reinterpret_cast<LPCECHAR>(U"test.txt"), 
        Interface::IFileOpenMode::Write
    );
    file->Write(U"Hello, World! Это работает! 😊\r\n", 32);
    file->Close();

    sysCalls->WriteConsole(hOut, U"File written!\r\n");
    sysCalls->Close(0);
    
    Windows::DestroyInterface(sysCalls);
}
```

**Результат:** исполняемый файл **11 KB** (Release, O2). Умеет выделять память, выводить в консоль (UTF-8 → UTF-16), создавать и писать в файл (UTF-32 → UTF-8).

---

## 🔧 Сборка (Windows, MSYS2 + Clang)

```bash
# Установка окружения (MSYS2 CLANG64)
pacman -S mingw-w64-clang-x86_64-clang mingw-w64-clang-x86_64-cmake

# Клонирование
git clone https://github.com/CytexLab/CytexLab-Platform
cd CytexLab-Platform

# Сборка теста (например, SysCalls/test/windows)
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -G "Ninja"
cmake --build . --target CytexLab.CytexLab-Platform.SysCalls.Test.Windows
```

### Требования

| Платформа | Компилятор | Инструменты |
|-----------|-----------|-------------|
| **Windows** | Clang 18+ (MSYS2 CLANG64) | CMake 3.20+, Ninja |
| **Linux** | Clang/GCC (в будущем) | — |
| **STM32** | ARM-none-eabi-gcc | STM32CubeMX |

---

## 📊 Размеры (Windows, AMD64, Release O2)

| Компонент | Байт |
|-----------|------|
| **Hello World (аллокатор + файлы)** | **11 KB (exe)** |
| — чистый код (.text) | ~4.8 KB |
| — данные (.rdata) | ~0.9 KB |
| — заголовки + выравнивание | ~5.3 KB |

O3 не может ужать сильнее — упёрлись в формат PE.

---

## 🛣️ Дорожная карта

- **[x] v0.2** — Windows: файлы (текст/бинарные), консоль, аллокатор
- **[ ] v0.3** — Linux: прямые syscall'ы (write, read, open, mmap)
- **[ ] v0.4** — STM32H7S7: двойная буферизация, DMA2D, свой GUI
- **[ ] v0.5** — Воксельный движок для портативной консоли

---

## 🤝 Вклад

Проект открыт для обсуждений. Если вы тоже считаете, что C++ без стандартной библиотеки — это круто, форкайте, предлагайте изменения.

Особенно нужна помощь с:
- Linux портом (прямые syscall'ы, таблица системных вызовов)
- Оптимизацией аллокатора под ARM Cortex-M
- Документацией и тестами

---

## 📜 Лицензия

Mozilla Public License Version 2.0 (MPL-2.0). См. файл [LICENSE](LICENSE).

---

## ⭐ P.S.

Проект в активной разработке. Код местами ещё «alpha», но базовые принципы уже устоялись. Если вы хотите **написать портативную консоль на STM32**, запустить **воксельный движок на 600 MHz Cortex-M7** или просто **понять, как работает C++ без libc** — вам сюда.
