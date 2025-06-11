#pragma once

#include <cstdint>

namespace EventfulEngine::Rendering{
    class IRenderPass;
    class IPipelineState;
    class IDescriptorSet;
    class IBuffer;
    class ITexture;

    class ICommandBuffer{
    public:
        virtual ~ICommandBuffer() = default;

        virtual void Reset() = 0;

        virtual void Begin() = 0;

        virtual void End() = 0;

        virtual void BeginRenderPass(IRenderPass& renderPass) = 0;

        virtual void EndRenderPass() = 0;

        virtual void SetViewport(float x, float y, float w, float h, float minDepth, float maxDepth) = 0;

        virtual void SetScissor(int32_t x, int32_t y, uint32_t w, uint32_t h) = 0;

        virtual void BindPipeline(IPipelineState& pipeline) = 0;

        virtual void BindDescriptorSet(IDescriptorSet& set, uint32_t index) = 0;

        virtual void BindVertexBuffer(IBuffer& buffer, uint32_t slot, uint64_t offset) = 0;

        virtual void BindIndexBuffer(IBuffer& buffer, uint64_t offset, uint32_t indexType) = 0;

        virtual void Draw(uint32_t vertexCount, uint32_t startVertex) = 0;

        virtual void DrawIndexed(uint32_t indexCount, uint32_t startIndex) = 0;

        virtual void PipelineBarrier(/*BarrierDesc&*/const void* desc) = 0;

        virtual void CopyBufferToTexture(IBuffer& src, ITexture& dst) = 0;

        virtual void CopyBuffer(IBuffer& src, IBuffer& dst, uint64_t srcOffset, uint64_t dstOffset, uint64_t size) = 0;
    };
}
