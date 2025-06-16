#pragma once

#include "../../Public/GenericPlatform/GenericPlatformTime.h"

namespace EventfulEngine{
    void GenericPlatformTime::Init(){
        appStartTime = EFClock::now();
        lastTime = appStartTime;
    }

    double GenericPlatformTime::InitTiming(){
        Init();
        return ToSeconds(appStartTime - lastTime);
    }

    EFDuration GenericPlatformTime::StepTime(){
        const auto now = EFClock::now();
        const auto delta = now - lastTime;
        lastTime = now;
        return delta;
    }

    EFDuration GenericPlatformTime::GetElapsed(const EFTimePoint& timePoint){
        return EFClock::now() - timePoint;
    }

    EFDuration GenericPlatformTime::GetTotalRuntime(){
        return EFClock::now() - appStartTime;
    }

    void GenericPlatformTime::SetGameTime(EFDuration const newGameTime){
        // now – startTime = currentGameTime, so we invert:
        gameStartTime = EFClock::now() - newGameTime;
        lastTime = EFClock::now();
    }

    double GenericPlatformTime::ToSeconds(const EFDuration d){
        return std::chrono::duration_cast<std::chrono::duration<double>>(d).count();
    }

    long long GenericPlatformTime::ToMilliseconds(const EFDuration d){
        return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
    }

    double GenericPlatformTime::ToMinutes(const EFDuration d){
        return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<60>>>(d).count();
    }

    double GenericPlatformTime::ToHours(const EFDuration d){
        return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<3600>>>(d).count();
    }

    double GenericPlatformTime::ToDays(const EFDuration d){
        return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<86400>>>(d).count();
    }

    EFString GenericPlatformTime::DurationToString(EFDuration d){
        using namespace std::chrono;
        const auto h = duration_cast<hours>(d);
        d -= h;
        const auto m = duration_cast<minutes>(d);
        d -= m;
        const auto s = duration_cast<seconds>(d);
        d -= s;
        const auto ms = duration_cast<milliseconds>(d);

        std::ostringstream oss;
        oss << std::setfill('0')
            << std::setw(2) << h.count() << ":"
            << std::setw(2) << m.count() << ":"
            << std::setw(2) << s.count() << "."
            << std::setw(3) << ms.count();
        return oss.str();
    }

    EFString GenericPlatformTime::DateTimeToString(const SystemClock::time_point timePoint){
        const std::time_t time = SystemClock::to_time_t(timePoint);
        struct tm* timeStructure = nullptr;
        localtime_s(timeStructure, &time);
        std::ostringstream oss;
        oss << std::put_time(timeStructure, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    EFString GenericPlatformTime::NowDateTimeString(){
        return DateTimeToString(SystemClock::now());
    }

    EFString GenericPlatformTime::NowGameTimeString(){
        return DurationToString(GetTotalRuntime());
    }
}
