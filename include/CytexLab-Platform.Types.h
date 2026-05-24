#pragma once

typedef __INT8_TYPE__ INT8;
typedef __INT16_TYPE__ INT16;
typedef __INT32_TYPE__ INT32;
typedef __INT64_TYPE__ INT64;
typedef __UINT8_TYPE__ UINT8;
typedef __UINT16_TYPE__ UINT16;
typedef __UINT32_TYPE__ UINT32;
typedef __UINT64_TYPE__ UINT64;

// Алиасы
typedef INT8 CHAR;
typedef UINT8 UCHAR;
typedef UINT16 WCHAR;
typedef UINT32 ECHAR; // Extend Char для Unicode UTF32
typedef INT32 BOOL;

// Указатели
typedef INT8* LPINT8;
typedef INT16* LPINT16;
typedef INT32* LPINT32;
typedef INT64* LPINT64;
typedef const INT8* LPCINT8;
typedef const INT16* LPCINT16;
typedef const INT32* LPCINT32;
typedef const INT64* LPCINT64;
typedef UINT8* LPUINT8;
typedef UINT16* LPUINT16;
typedef UINT32* LPUINT32;
typedef UINT64* LPUINT64;
typedef const UINT8* LPCUINT8;
typedef const UINT16* LPCUINT16;
typedef const UINT32* LPCUINT32;
typedef const UINT64* LPCUINT64;
typedef ECHAR* LPECHAR;
typedef const ECHAR* LPCECHAR;

// Другие типы
typedef void* HANDLE;
typedef void* LPVOID;
typedef const LPVOID LPCVOID;
typedef LPCVOID POINTER;

// Define'ы
#define TRUE BOOL(1)
#define FALSE BOOL(0)
#define NULLPTR LPVOID(-1)