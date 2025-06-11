#pragma once
#include <cstdint>

namespace EventfulEngine::Rendering{
    class ICommandBuffer;
    class IFence;

    class ICommandQueue{
    public:
        virtual ~ICommandQueue() = default;

        virtual void Execute(ICommandBuffer& cmdBuffer) = 0;

        virtual void Signal(IFence& fence, uint64_t value) = 0;

        virtual void Wait(IFence& fence, uint64_t value) = 0;
    };
}
