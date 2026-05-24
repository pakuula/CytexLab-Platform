# CytexLab Platform

**Кроссплатформенный C++ фреймворк для embedded и системного программирования без стандартной библиотеки.**

[![License: MPL2](https://img.shields.io/badge/License-MPL-yellow.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20STM32-blue)]()
[![C++](https://img.shields.io/badge/C%2B%2B-14-blue.svg)]()
[![Size](https://img.shields.io/badge/Hello%20World-11KB-green)]()

---

## 🔥 Что это и зачем?

CytexLab Platform — это моя замена стандартной библиотеки для ситуаций, где STL не завезешь, а libc — роскошь.

- **Freestanding C++**: `-nostdlib`, свои операторы `new`/`delete`, никакого RTTI, исключений и stack-protector'а
- **Свой аллокатор**: с дефрагментацией, выравниванием и произвольным освобождением
- **Unicode-first**: внутри — UTF-32 (O(1) индексация!), снаружи — UTF-8/UTF-16
- **Кроссплатформенность**: один интерфейс — разные реализации под Windows/Linux/STM32
- **Минимальный размер**: Hello World с аллокатором, файлами и Unicode — **11 KB** (O3 не может ужать сильнее)

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
        virtual INT64 Read(LPVOID Buffer, INT64 BufferSize) = 0;      // бинарное
        virtual INT64 Read(LPECHAR Buffer, INT64 BufferSize) = 0;     // текстовое (UTF-32)
        virtual INT64 Write(LPCVOID Buffer, INT64 BufferSize) = 0;    // бинарное
        virtual INT64 Write(LPCECHAR Buffer, INT64 BufferSize) = 0;   // текстовое (UTF-32)
        virtual INT64 Seek(INT64 Interrupt, INT64 Pos) = 0;
        virtual void Close() = 0;
        virtual INT64 GetSize() = 0;
        virtual BOOL IsEOF() = 0;
        virtual void Flush() = 0;
    };
    
}
```

**Платформо-зависимые реализации** лежат в отдельных папках:
- `SysCalls/windows/` — 9 файлов, CLion не тормозит
- `SysCalls/linux/` — скоро
- `SysCalls/stm32/` — потом

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

**Результат:**
- Исполняемый файл: **11 KB** (Release, O2)
- Умеет: выделять память, писать в консоль (UTF-8 → UTF-16), создавать и писать в файл (UTF-32 → UTF-8)

---

## 🔧 Сборка

### Windows (MSYS2 + Clang)

```bash
# Установка окружения (MSYS2 CLANG64)
pacman -S mingw-w64-clang-x86_64-clang mingw-w64-clang-x86_64-cmake

# Клонирование
git clone https://github.com/yourusername/CytexLab
cd CytexLab

# Сборка
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

### Требования

| Платформа | Компилятор | Инструменты |
|-----------|-----------|-------------|
| **Windows** | Clang 18+ (MSYS2) | CMake 3.20+, Ninja |
| **Linux** | Clang/GCC (скоро) | — |
| **STM32** | ARM-none-eabi-gcc | STM32CubeMX |

---

## 📂 Структура проекта

```
CytexLab/
├── include/                    # Публичные заголовки
│   ├── CytexLab-Platform.Types.h
│   ├── CytexLab-Platform.ISysCalls.h
│   └── CytexLab-Platform.UnicodeProcessor.h
│
├── SysCalls/                   # Системные вызовы (HAL)
│   ├── windows/                # ✅ Windows (9 файлов, рабочий)
│   │   ├── Fabric.cpp          # CreateInterface/DestroyInterface
│   │   ├── SysCallsImpl.cpp    # ISysCalls реализация
│   │   ├── FileImpl.cpp        # IFile реализация (Read/Write/Seek)
│   │   ├── ConsoleWindows.cpp  # WriteConsole/ReadConsole
│   │   ├── HeapWindows.cpp     # Аллокатор (Heap/Free/GetMemory)
│   │   └── WindowsImports.h    # WinAPI импорты
│   ├── linux/                  # 🚧 В разработке
│   └── stm32/                  # 📋 Планируется
│
├── UnicodeProcessor/           # UTF-32 ↔ UTF-8/UTF-16 конвертация
├── MemFunctions/               # memcpy/memset (freestanding)
├── tests/                      # Тесты
└── CMakeLists.txt
```

---

## 📊 Размеры (Windows, AMD64, Release O2)

| Компонент | Байт |
|-----------|------|
| **Исходный код платформы** | ~500 строк |
| **Hello World (с аллокатором + файлами)** | **11 KB (exe)** |
| — чистый код (.text) | ~4.8 KB |
| — данные (.rdata) | ~0.9 KB |
| — заголовки + выравнивание | ~5.3 KB |

**Вывод:** O3 не может ужать сильнее — упёрлись в формат PE.

---

## 🛣️ Дорожная карта

- **[x] v0.2** — Windows: файлы (текст/бинарные), консоль, аллокатор
- **[ ] v0.3** — Linux: прямые syscall'ы (write, read, open, mmap)
- **[ ] v0.4** — STM32H7S7: двойная буферизация, DMA2D, свой GUI
- **[ ] v0.5** — Воксельный движок для портативной консоли

---

## 🤝 Вклад

Проект открыт для обсуждений. Если ты тоже считаешь, что C++ без стандартной библиотеки — это круто, пиши, форкай, предлагай.

Особенно нужна помощь с:
- Linux портом (прямые syscall'ы, таблица системных вызовов)
- Оптимизацией аллокатора под ARM Cortex-M
- Документацией и тестами

---

## 📜 Лицензия

MPL 2 © [Вадим](https://github.com/CytexLab)

---

## ⭐ P.S.

Проект в активной разработке. Код местами ещё "alpha", но базовые принципы уже устоялись. Если хочешь **написать портативную консоль на STM32**, запустить **воксельный движок на 600 MHz Cortex-M7** или просто **понять, как работает C++ без libc** — тебе сюда.
