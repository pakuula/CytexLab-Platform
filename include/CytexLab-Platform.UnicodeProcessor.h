//
// Created by Вадим on 21.05.2026.
//

#ifndef CYTEXLAB_CYTEXLAB_PLATFORM_UNICODEPROCESSOR_CYTEXLAB_PLATFORM_UNICODEPROCESSOR_HPP
#define CYTEXLAB_CYTEXLAB_PLATFORM_UNICODEPROCESSOR_CYTEXLAB_PLATFORM_UNICODEPROCESSOR_HPP

namespace CytexLab
{
    namespace UnicodeProcessor
    {
        INT8 ToUTF32(LPCUINT8 From, LPUINT32 To);
        INT8 ToUTF32(LPCUINT16 From, LPUINT32 To);
        INT8 ToUTF16(LPCUINT32 From, LPUINT16 To);
        INT8 ToUTF8(LPCUINT32 From, LPUINT8 To);
        INT8 Size(LPCUINT8 Sym);
        INT8 Size(LPCUINT16 Sym);
    }

    namespace UnlimetedUnicodeProcessor
    {
        typedef struct {
            INT64 SymbolsCount;
            INT64 BytesCount;
        } ReturnData;

        ReturnData ToUTF32(LPCUINT8 From, LPUINT32 To);
        ReturnData ToUTF32(LPCUINT16 From, LPUINT32 To);
        ReturnData ToUTF16(LPCUINT32 From, LPUINT16 To);
        ReturnData ToUTF8(LPCUINT32 From, LPUINT8 To);
    }
}

#endif //CYTEXLAB_CYTEXLAB_PLATFORM_UNICODEPROCESSOR_CYTEXLAB_PLATFORM_UNICODEPROCESSOR_HPP
