#pragma once

#include <chrono>
#include <memory>
#include <string>

#include "Platform.h"
#include "EnumFlag.h"

// TODO: Instead of abusing these using directives, actually make our own way to handle strings, chars and ints etc. For now this is a workaround to save on immediate work
using EFString = std::string;
template <typename T>
using Ref = std::shared_ptr<T>;
using EFClock = std::chrono::high_resolution_clock;
using SystemClock = std::chrono::system_clock;
using EFTimePoint = EFClock::time_point;
using EFDuration = EFClock::duration;

#include "PlatformTime.h"
