#pragma once
#include <cstdint>

namespace EventfulEngine::Rendering{
    class ITexture;

    class ISwapChain{
    public:
        virtual ~ISwapChain() = default;

        virtual void Resize(uint32_t width, uint32_t height) = 0;

        virtual ITexture& GetCurrentBackBuffer() = 0;

        virtual void Present(uint32_t vsync) = 0;
    };
}
