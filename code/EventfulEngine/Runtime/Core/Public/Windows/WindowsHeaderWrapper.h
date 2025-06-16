#pragma once

#include "PlatformAbstraction/Platform.h"

#if defined(WINDOWS_H_WRAPPER_GUARD)
#error WINDOWS_H_WRAPPER_GUARD already defined
#endif
#define WINDOWS_H_WRAPPER_GUARD

#ifndef STRICT
#define STRICT
#endif
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#undef WINDOWS_H_WRAPPER_GUARD
