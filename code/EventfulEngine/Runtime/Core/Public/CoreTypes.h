#pragma once

#include <chrono>
#include <memory>
#include <string>

#include "Platform.h"
#include "CoreMacros.h"
#include "PlatformTime.h"

// TODO: Instead of abusing these using directives, actually make our own way to handle strings, chars and ints etc. For now this is a workaround to save on immediate work
using uint32 = uint32_t;
using int32 = int32_t;
using EFString = std::string;
using Ref = std::shared_ptr;
using EFChar = char8_t;
using int64 = int64_t;
using uint64 = uint64_t;
using EFClock = std::chrono::high_resolution_clock;
using SystemClock = std::chrono::system_clock;
using EFTimePoint = EFClock::time_point;
using EFDuration = EFClock::duration;
