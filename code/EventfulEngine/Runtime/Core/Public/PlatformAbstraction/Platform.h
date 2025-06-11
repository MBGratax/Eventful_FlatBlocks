#pragma once

#include "CoreMacros.h"
// TODO: Currently only support windows and autodefines it, later CMake should set platform defines, this script makes sure to undefine other platforms, even though they are not supported yet
#define PLATFORM_WINDOWS = 1

#if !defined(PLATFORM_WINDOWS)
#define PLATFORM_WINDOWS 0
#endif
#if !defined(PLATFORM_LINUX)
#define PLATFORM_LINUX 0
#endif
#if !defined(PLATFORM_MAC)
#define PLATFORM_MAC 0
#endif
// etc...

// This makes it so we are always safe to include Platform.h if we need platform specific code
#include "GenericPlatform/GenericPlatform.h"
#include CONCATPLATFORMHEADER(Platform.h)

