#pragma once

struct EFGenericPlatformTypes{
    // Unsigned ints

    typedef unsigned char uint8;
    typedef unsigned short int uint16;
    typedef unsigned int uint32;
    typedef unsigned long long uint64;

    // Signed ints
    typedef signed char int8;
    typedef signed short int int16;
    typedef signed int int32;
    typedef signed long long int64;

    // Chars
    typedef char EFChar;
    typedef wchar_t EFWChar;
    typedef char8_t EFChar8;
    typedef char16_t EFChar16;
    typedef char32_t EFChar32;
    typedef EFWChar EFTChar;
};
