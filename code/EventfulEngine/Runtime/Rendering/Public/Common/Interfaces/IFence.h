#pragma once
#include <cstdint>

namespace EventfulEngine::Rendering{
    class ICommandQueue;

    class IFence{
    public:
        virtual ~IFence() = default;

        virtual uint64_t Signal(ICommandQueue& queue) = 0;

        virtual void Wait(uint64_t value) = 0;

        virtual bool IsCompleted(uint64_t value) = 0;
    };
}
