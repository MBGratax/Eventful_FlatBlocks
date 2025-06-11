#pragma once

#include "../../Public/GenericPlatform/GenericPlatformTime.h"
#include <chrono>

double GenericPlatformTime::InitTiming(){
    const auto now = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> dur = now.time_since_epoch();
    return dur.count();
}
