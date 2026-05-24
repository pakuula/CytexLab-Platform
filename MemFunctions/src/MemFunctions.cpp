//
// Created by Вадим on 23.05.2026.
//

#include "CytexLab-Platform.MemFunctions.h"

extern "C" void* memset(void* dest, int c, unsigned long long count) {
    unsigned char* p = (unsigned char*)dest;
    while (count--) {
        *p++ = (unsigned char)c;
    }
    return dest;
}

extern "C" void memcpy(void* dest, const void* source, unsigned long long count) {
    unsigned char* p = (unsigned char*) dest;
    const unsigned char* s = (const unsigned char*) source;
    while (count--) {
        *p++ = *s++;
    }
}