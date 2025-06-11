#pragma once

namespace EventfulEngine::Rendering{
    class ITexture{
    public:
        virtual ~ITexture() = default;

        virtual void* Map() = 0;

        virtual void Unmap() = 0;
    };
}
