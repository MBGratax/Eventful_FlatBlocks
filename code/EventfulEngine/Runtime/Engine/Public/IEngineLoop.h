#pragma once
#include <cstdint>


namespace EventfulEngine{
    class IEngineLoop{
    public:
        virtual ~IEngineLoop() = default;

        virtual int32_t Init() = 0;

        virtual void Tick() = 0;

        virtual void ClearPendingCleanupObjects() = 0;
    };
}
