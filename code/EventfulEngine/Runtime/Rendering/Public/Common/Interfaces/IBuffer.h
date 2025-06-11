#pragma once

namespace EventfulEngine::Rendering{
    class IBuffer{
    public:
        virtual ~IBuffer() = default;

        virtual void* Map() = 0;

        virtual void Unmap() = 0;

        virtual void UpdateData(const void* data, size_t size) = 0;
    };
}
