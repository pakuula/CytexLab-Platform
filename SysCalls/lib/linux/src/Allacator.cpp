//
// Created by Вадим on 24.05.2026.
//

#include "Allacator.h"
#include "KernelLinuxCalls.h"

void Allacator::Init() {
    this->memBlocks[0].Pointer = reinterpret_cast<LPVOID>(syscall6(SYS_mmap, 0, START_MEM_BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    this->memBlocks[0].Size = START_MEM_BLOCK_SIZE;

    for (UINT64 i = 1; i < MAX_MEM_BLOCK; i++) this->memBlocks[i].Pointer = NULLPTR;
}

void Allacator::DeInit() {
    for (UINT64 i = 0; i < MAX_MEM_BLOCK; i++) {
        if (this->memBlocks[i].Pointer != NULLPTR) {
            syscall2(SYS_munmap, reinterpret_cast<INT64>(this->memBlocks[i].Pointer), this->memBlocks[i].Size);
        } else {
            continue;
        }
    }
}

inline UINT64 AlignUp(UINT64 Value) {
    return (Value + 7) & ~static_cast<UINT64>(7);
}

CytexLab::SysCalls::Interface::IMemBlock Allacator::Heap(UINT64 Size) {
    // Сразу выравниваем размер запрашиваемой памяти
    UINT64 alignedSize = AlignUp(Size);

    INT64 targetBlockID = -1;
    UINT64 targetOffset = 0;
    UINT64 insertAtIndex = 0;

    // Шаг 1. Ищем место в уже существующих системных блоках
    for (INT64 i = 0; i < MAX_MEM_BLOCK; i++) {
        if (this->memBlocks[i].Pointer == NULLPTR) continue;

        MemBlock& currentMem = this->memBlocks[i];

        // Если в системном блоке исчерпан лимит на количество аллокаций
        if (currentMem.Allocated >= MAX_ALLOC_BLOCK) continue;

        // Случай А: Блок абсолютно пустой
        if (currentMem.Allocated == 0) {
            if (currentMem.Size >= alignedSize) {
                targetBlockID = i;
                targetOffset = 0;
                insertAtIndex = 0;
                break;
            }
            continue;
        }

        // Случай Б: Проверяем место ПЕРЕД первым выделенным подблоком
        // Здесь смещение 0, оно уже идеально выровнено
        if (currentMem.Blocks[0].Offset >= alignedSize) {
            targetBlockID = i;
            targetOffset = 0;
            insertAtIndex = 0;
            break;
        }

        // Случай В: Ищем "дыры" между уже существующими подблоками
        bool foundSpace = false;
        for (UINT64 j = 0; j < currentMem.Allocated - 1; j++) {
            // Выравниваем гипотетическое начало нового блока
            UINT64 currentBlockEnd = currentMem.Blocks[j].Offset + currentMem.Blocks[j].Size;
            UINT64 alignedStart = AlignUp(currentBlockEnd);
            UINT64 nextBlockStart = currentMem.Blocks[j + 1].Offset;

            // Проверяем, влезет ли наш выровненный блок в эту дыру
            if (nextBlockStart >= alignedStart && (nextBlockStart - alignedStart) >= alignedSize) {
                targetBlockID = i;
                targetOffset = alignedStart;
                insertAtIndex = j + 1; // Вставим между j и j+1
                foundSpace = true;
                break;
            }
        }
        if (foundSpace) break;

        // Случай Г: Проверяем место ПОСЛЕ самого последнего подблока
        UINT64 lastBlockEnd = currentMem.Blocks[currentMem.Allocated - 1].Offset +
                              currentMem.Blocks[currentMem.Allocated - 1].Size;
        UINT64 alignedLastBlockEnd = AlignUp(lastBlockEnd);

        if (currentMem.Size >= alignedLastBlockEnd && (currentMem.Size - alignedLastBlockEnd) >= alignedSize) {
            targetBlockID = i;
            targetOffset = alignedLastBlockEnd;
            insertAtIndex = currentMem.Allocated; // Вставим в самый конец
            break;
        }
    }

    // Шаг 2. Если места не нашлось, выделяем ОДИН новый системный блок
    if (targetBlockID == -1) {
        for (INT64 i = 0; i < MAX_MEM_BLOCK; i++) {
            if (this->memBlocks[i].Pointer == NULLPTR) {
                // Используем alignedSize для расчета размера нового большого блока
                UINT64 newBlockSize = (alignedSize * 2 > START_MEM_BLOCK_SIZE) ? alignedSize * 2 : START_MEM_BLOCK_SIZE;

                this->memBlocks[i].Pointer = reinterpret_cast<LPVOID>(syscall6(SYS_mmap, 0, newBlockSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
                if (this->memBlocks[i].Pointer == NULLPTR) {
                    // Системная куча переполнена
                    CytexLab::SysCalls::Interface::IMemBlock failedResult;
                    failedResult.BlockID = -1;
                    return failedResult;
                }

                this->memBlocks[i].Size = newBlockSize;
                this->memBlocks[i].Allocated = 0;

                targetBlockID = i;
                targetOffset = 0;
                insertAtIndex = 0;
                break; // Важно! Выходим сразу после выделения одного блока
            }
        }
    }

    // Формируем результат, если нашли/выделили место
    CytexLab::SysCalls::Interface::IMemBlock result;

    if (targetBlockID == -1) {
        result.BlockID = -1;
        return result;
    }

    // Шаг 3. Записываем информацию о подблоке со сдвигом остальных (сохраняем сортировку!)
    MemBlock& finalMem = this->memBlocks[targetBlockID];

    // Сдвигаем элементы вправо, чтобы освободить место для insertAtIndex
    for (UINT64 k = finalMem.Allocated; k > insertAtIndex; k--) {
        finalMem.Blocks[k] = finalMem.Blocks[k - 1];
    }

    // Записываем данные нового подблока (сохраняем именно alignedSize!)
    finalMem.Blocks[insertAtIndex].Offset = targetOffset;
    finalMem.Blocks[insertAtIndex].Size = alignedSize;
    finalMem.Allocated += 1;

    result.BlockID = targetBlockID;
    result.Offset = targetOffset;
    result.Size = alignedSize; // Возвращаем клиенту выровненный размер

    return result;
}

LPVOID Allacator::GetMem(CytexLab::SysCalls::Interface::IMemBlock &memBlock) {
    if (memBlock.BlockID != -1) {
        return static_cast<LPUINT8>(this->memBlocks[memBlock.BlockID].Pointer) + memBlock.Offset;
    }
    return NULLPTR;
}

void Allacator::Free(CytexLab::SysCalls::Interface::IMemBlock &memBlock) {
    // 1. Защита от некорректного или уже освобожденного ID
    if (memBlock.BlockID == -1 || memBlock.BlockID >= MAX_MEM_BLOCK) {
        return;
    }

    MemBlock& currentMem = this->memBlocks[memBlock.BlockID];
    INT64 foundIndex = -1;

    // 2. Ищем индекс удаляемого подблока по его смещению
    for (UINT64 i = 0; i < currentMem.Allocated; i++) {
        if (currentMem.Blocks[i].Offset == memBlock.Offset) {
            foundIndex = static_cast<INT64>(i);
            break;
        }
    }

    // Если такой блок не найден (например, повторный вызов Free для того же адреса)
    if (foundIndex == -1) {
        return;
    }

    // 3. Смещаем все подблоки после удаленного влево, убирая "дыру" в массиве метаданных
    for (UINT64 k = static_cast<UINT64>(foundIndex); k < currentMem.Allocated - 1; k++) {
        currentMem.Blocks[k] = currentMem.Blocks[k + 1];
    }

    // 4. Очищаем теперь уже неиспользуемый последний элемент для безопасности
    currentMem.Blocks[currentMem.Allocated - 1].Offset = 0;
    currentMem.Blocks[currentMem.Allocated - 1].Size = 0;

    // 5. Уменьшаем счетчик аллокаций
    currentMem.Allocated -= 1;

    // Optional: Если очень хочется экономить память системной кучи Windows,
    // здесь можно добавить проверку: если currentMem.Allocated == 0,
    // то можно вызвать ::HeapFree и полностью освободить системный блок memBlocks[BlockID].
    // Но для производительности лучше оставить его выделенным под будущие Heap().

    // Маркируем переданную структуру как освобожденную
    memBlock.BlockID = -1;
    memBlock.Offset = 0;
    memBlock.Size = 0;
}
