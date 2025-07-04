#pragma once

#include "CoreMacros.h"

// Get Compiler
#if defined(__GNUC__)
#if defined(__clang__)
        #define EF_COMPILER_CLANG
#else
        #define EF_COMPILER_GCC
#endif
#elif defined(_MSC_VER)
#define EF_COMPILER_MSVC
#endif

// Set Forceinline macro
#ifdef EF_COMPILER_MSVC
#define EF_FORCE_INLINE __forceinline
#define EF_EXPLICIT_STATIC static
#elif defined(__GNUC__)
        #define EF_FORCE_INLINE __attribute__((always_inline)) inline
        #define EF_EXPLICIT_STATIC
#else
        #define EF_FORCE_INLINE inline
        #define EF_EXPLICIT_STATIC
#endif

// TODO: Currently only support windows and autodefines it, later CMake should set platform defines, this script makes sure to undefine other platforms, even though they are not supported yet
#define EF_PLATFORM_WINDOWS 1

#if !defined(EF_PLATFORM_WINDOWS)
#define EF_PLATFORM_WINDOWS 0
#endif
#if !defined(EF_PLATFORM_LINUX)
#define EF_PLATFORM_LINUX 0
#endif
#if !defined(EF_PLATFORM_APPLE)
#define EF_PLATFORM_APPLE 0
#endif
#if !defined(EF_PLATFORM_MAC)
#define EF_PLATFORM_MAC 0
#endif
// etc...


// This makes it so we are always safe to include Platform.h if we need platform-specific code
#include "GenericPlatform/GenericPlatform.h"
// Commented out until we actually need platform-specific code
#include CONCATPLATFORMHEADER(Platform.h)

// Apply platform-specific typedefs
typedef EFPlatformTypes::uint8 uint8;
typedef EFPlatformTypes::uint16 uint16;
typedef EFPlatformTypes::uint32 uint32;
typedef EFPlatformTypes::uint64 uint64;

typedef EFPlatformTypes::int8 int8;
typedef EFPlatformTypes::int16 int16;
typedef EFPlatformTypes::int32 int32;
typedef EFPlatformTypes::int64 int64;

typedef EFPlatformTypes::EFChar EFChar;
typedef EFPlatformTypes::EFWChar EFWChar;
typedef EFPlatformTypes::EFChar16 EFChar16;
typedef EFPlatformTypes::EFChar32 EFChar32;
typedef EFPlatformTypes::EFWChar EFTChar;
