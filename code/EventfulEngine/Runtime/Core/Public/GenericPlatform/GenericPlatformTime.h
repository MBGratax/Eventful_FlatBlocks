#pragma once

#include "CoreTypes.h"


struct GenericPlatformTime{
    // inline static data (C++17+); initialized on first ODR-use
    inline static EFTimePoint appStartTime{};
    inline static EFTimePoint lastTime{};
    inline static EFTimePoint gameStartTime{};

    // must be called once at startup
    static EFCORE_API void Init();

    static EFCORE_API double InitTiming();

    // mark a new frame/timestep; returns delta since last call
    // and advances lastTime to 'now'
    static EFCORE_API EFDuration StepTime();

    // how much real time has passed since an arbitrary timestamp
    static EFCORE_API EFDuration GetElapsed(const EFTimePoint& timePoint);

    // total game time since Init or since last SetGameTime()
    static EFCORE_API EFDuration GetTotalRuntime();

    static EFCORE_API void SetGameTime(EFDuration const newGameTime);

#pragma region CONVERSION HELPERS

    // get raw durations in various units
    static double ToSeconds(const EFDuration d);

    static long long ToMilliseconds(const EFDuration d);

    static double ToMinutes(const EFDuration d);

    static double ToHours(const EFDuration d);

    static double ToDays(const EFDuration d);

#pragma endregion CONVERSION HELPERS

#pragma region STRING OPERATIONS

    static EFString DurationToString(EFDuration d);

    // "YYYY-MM-DD HH:MM:SS" from a system-clock time_point
    static EFString DateTimeToString(const SystemClock::time_point timePoint);

    // convenience: get the real-world "now" as a string
    static EFString NowDateTimeString();

    // get the current game time as a formatted string
    static EFString NowGameTimeString();
#pragma endregion STRING OPERATIONS
};

typedef GenericPlatformTime EFPlatformTime;
