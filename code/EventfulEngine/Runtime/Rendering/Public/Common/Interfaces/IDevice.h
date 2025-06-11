#pragma once
#include <cstdint>
#include <vector>
#include <memory>

#include "../../RendererAPI.h"

namespace EventfulEngine::Rendering{
    class ISwapChain;
    class ICommandQueue;
    class ICommandBuffer;
    class IPipelineState;
    class IShaderModule;
    class IBuffer;
    class ITexture;
    class ISampler;
    class IDescriptorPool;
    class IFence;
    class ISemaphore;

    class IDevice{
    public:
        virtual ~IDevice() = default;

        [[nodiscard]] virtual RendererAPI GetAPI() const = 0;

        // Factory-like creation methods
        virtual std::unique_ptr<ISwapChain> CreateSwapChain(void* nativeWindow, uint32_t width, uint32_t height) = 0;

        virtual std::unique_ptr<ICommandQueue> CreateCommandQueue() = 0;

        virtual std::unique_ptr<ICommandBuffer> CreateCommandBuffer() = 0;

        virtual std::unique_ptr<IPipelineState> CreatePipelineState(const /*PipelineDesc&*/ void* desc) = 0;

        virtual std::unique_ptr<IShaderModule> CreateShaderModule(const std::vector<uint8_t>& code, uint32_t stageFlags)
        = 0;

        virtual std::unique_ptr<IBuffer> CreateBuffer(size_t size, uint32_t usage, uint32_t memoryFlags) = 0;

        virtual std::unique_ptr<ITexture> CreateTexture(uint32_t width, uint32_t height, uint32_t format,
                                                        uint32_t flags) = 0;

        virtual std::unique_ptr<ISampler> CreateSampler(const /*SamplerDesc&*/ void* desc) = 0;

        virtual std::unique_ptr<IDescriptorPool> CreateDescriptorPool(uint32_t maxSets) = 0;

        virtual std::unique_ptr<IFence> CreateFence(bool signaled) = 0;

        virtual std::unique_ptr<ISemaphore> CreateSemaphore() = 0;

        // Feature queries

        [[nodiscard]] virtual bool SupportsCompute() const = 0;

        [[nodiscard]] virtual bool SupportsTessellation() const = 0;

        [[nodiscard]] virtual bool SupportsRayTracing() const = 0;
    };
}
