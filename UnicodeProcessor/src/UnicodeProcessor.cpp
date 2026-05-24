//
// Created by Вадим on 21.05.2026.
//

#include <CytexLab-Platform.Types.h>
#include <CytexLab-Platform.UnicodeProcessor.h>

INT8 CytexLab::UnicodeProcessor::Size(LPCUINT8 Sym)
{
    if (Sym == NULLPTR) return -1;
    UINT8 byte = *Sym;

    if ((byte & 0x80) == 0)
    {
        return 1;
    }
    else if ((byte & 0xE0) == 0xC0)
    {
        return 2;
    }
    else if ((byte & 0xF0) == 0xE0)
    {
        return 3;
    }
    else if ((byte & 0xF8) == 0xF0)
    {
        return 4;
    }
    else {
        return -2;
    }
}

INT8 CytexLab::UnicodeProcessor::Size(LPCUINT16 Sym) {
    if (Sym == NULLPTR) return -1;
    UINT16 byte = *Sym;

    if ((byte & 0xFC00) == 0xD800) {
        return 2;
    }
    else if ((byte & 0xFC00) == 0xDC00) {
        return -2;
    }
    else {
        return 1;
    }
}

BOOL isCorrectContinueUTF8(UINT8 Sym) {
    if ((Sym & 0xC0) == 0x80) return TRUE;
    return FALSE;
}

INT8 CytexLab::UnicodeProcessor::ToUTF32(LPCUINT8 From, LPUINT32 To) {
    if (From == NULLPTR) return -1;
    if (To == NULLPTR) return -2;

    INT8 size = Size(From);
    if (size == -1) return -3;

    UINT8 firstByte = *From;

    switch (size) {
        case 1:
            *To = firstByte;
            break;
        case 2: {
            UINT8 secondByte = *(From + 1);
            if (!isCorrectContinueUTF8(secondByte)) return -4;
            *To = ((firstByte & 0x1F) << 6) | (secondByte & 0x3F);
            break;
        }
        case 3: {
            UINT8 secondByte = *(From + 1);
            UINT8 thirdByte = *(From + 2);
            if (!isCorrectContinueUTF8(secondByte) || !isCorrectContinueUTF8(thirdByte)) return -4;
            *To = ((firstByte & 0x0F) << 12) |
                  ((secondByte & 0x3F) << 6) |
                  (thirdByte & 0x3F);
            break;
        }

        case 4: {
            UINT8 secondByte = *(From + 1);
            UINT8 thirdByte = *(From + 2);
            UINT8 fourthByte = *(From + 3);
            if (!isCorrectContinueUTF8(secondByte) ||
                !isCorrectContinueUTF8(thirdByte) ||
                !isCorrectContinueUTF8(fourthByte)) return -4;
            *To =  ((firstByte & 0x07) << 18) |
                   ((secondByte & 0x3F) << 12) |
                   ((thirdByte & 0x3F) << 6) |
                   (fourthByte & 0x3F);
            break;
        }

        default:
            return -3;
    }

    return size;
}

INT8 CytexLab::UnicodeProcessor::ToUTF32(LPCUINT16 From, LPUINT32 To)
{
    if (From == NULLPTR) return -1;
    if (To == NULLPTR) return -2;

    UINT16 firstCode = *From;

    if ((firstCode & 0xFC00) == 0xD800)
    {
        UINT16 secondCode = *(From + 1);
        if ((secondCode & 0xFC00) != 0xDC00) return -3;
        *To = ((static_cast<UINT32>(firstCode & 0x03FF) << 10) |
               (static_cast<UINT32>(secondCode & 0x03FF))) + 0x10000;
        return 2;
    }
    else if ((firstCode & 0xFC00) == 0xDC00)
    {
        return -3;
    }
    else
    {
        *To = firstCode;
        return 1;
    }
}

INT8 CytexLab::UnicodeProcessor::ToUTF16(LPCUINT32 From, LPUINT16 To)
{
    if (From == NULLPTR) return -1;
    if (To == NULLPTR) return -2;

    UINT32 codepoint = *From;

    if (codepoint > 0x10FFFF) return -3;

    if (codepoint >= 0x10000)
    {
        UINT32 offset = codepoint - 0x10000;
        *To = static_cast<UINT16>((offset >> 10) | 0xD800);
        *(To + 1) = static_cast<UINT16>((offset & 0x03FF) | 0xDC00);
        return 2;
    }
    else
    {
        *To = static_cast<UINT16>(codepoint);
        return 1;
    }
}

INT8 CytexLab::UnicodeProcessor::ToUTF8(LPCUINT32 From, LPUINT8 To)
{
    if (From == NULLPTR) return -1;
    if (To == NULLPTR) return -2;

    UINT32 codepoint = *From;

    if (codepoint > 0x10FFFF) return -3;

    if (codepoint < 0x80)
    {
        *To = static_cast<UINT8>(codepoint);
        return 1;
    }
    else if (codepoint < 0x800)
    {
        *To = static_cast<UINT8>(0xC0 | (codepoint >> 6));
        *(To + 1) = static_cast<UINT8>(0x80 | (codepoint & 0x3F));
        return 2;
    }
    else if (codepoint < 0x10000)
    {
        if (codepoint >= 0xD800 && codepoint <= 0xDFFF) return -3;
        *To = static_cast<UINT8>(0xE0 | (codepoint >> 12));
        *(To + 1) = static_cast<UINT8>(0x80 | ((codepoint >> 6) & 0x3F));
        *(To + 2) = static_cast<UINT8>(0x80 | (codepoint & 0x3F));
        return 3;
    }
    else
    {
        *To = static_cast<UINT8>(0xF0 | (codepoint >> 18));
        *(To + 1) = static_cast<UINT8>(0x80 | ((codepoint >> 12) & 0x3F));
        *(To + 2) = static_cast<UINT8>(0x80 | ((codepoint >> 6) & 0x3F));
        *(To + 3) = static_cast<UINT8>(0x80 | (codepoint & 0x3F));
        return 4;
    }
}

CytexLab::UnlimetedUnicodeProcessor::ReturnData CytexLab::UnlimetedUnicodeProcessor::ToUTF32(LPCUINT8 From, LPUINT32 To) {
    if (From == NULLPTR) return {-1, 0};
    if (To == NULLPTR) return {-2, 0};

    INT64 size = 0;
    INT64 bytes = 0;

    while (*(From + bytes) != 0) {
        INT8 s = UnicodeProcessor::ToUTF32(From + bytes, To + size);
        if (s == -3) return {-3, 0};
        else if (s == -4) return {-4, 0};
        else {
            size++;
            bytes += s;
        }
    }

    return {size, bytes};
}

CytexLab::UnlimetedUnicodeProcessor::ReturnData CytexLab::UnlimetedUnicodeProcessor::ToUTF32(LPCUINT16 From, LPUINT32 To) {
    if (From == NULLPTR) return {-1, 0};
    if (To == NULLPTR) return {-2, 0};

    INT64 size = 0;
    INT64 bytes = 0;

    while (*(From + bytes) != 0) {
        INT8 s = UnicodeProcessor::ToUTF32(From + bytes, To + size);
        if (s == -3) return {-3, 0};
        else {
            size++;
            bytes += s;
        }
    }

    return {size, bytes};
}

CytexLab::UnlimetedUnicodeProcessor::ReturnData CytexLab::UnlimetedUnicodeProcessor::ToUTF16(LPCUINT32 From, LPUINT16 To) {
    if (From == NULLPTR) return {-1, 0};
    if (To == NULLPTR) return {-2, 0};

    INT64 size = 0;
    INT64 bytes = 0;

    while (*(From + size) != 0) {
        INT8 s = UnicodeProcessor::ToUTF16(From + size, To + bytes);

        if (s == -3) return {-3, 0};
        else {
            size++;
            bytes += s;
        }
    }

    return {size, bytes};
}

CytexLab::UnlimetedUnicodeProcessor::ReturnData CytexLab::UnlimetedUnicodeProcessor::ToUTF8(LPCUINT32 From, LPUINT8 To) {
    if (From == NULLPTR) return {-1, 0};
    if (To == NULLPTR) return {-2, 0};

    INT64 size = 0;
    INT64 bytes = 0;

    while (*(From + size) != 0) {
        INT8 s = UnicodeProcessor::ToUTF8(From + size, To + bytes);

        if (s == -3) return {-3, 0};
        else {
            size++;
            bytes += s;
        }
    }

    return {size, bytes};
}