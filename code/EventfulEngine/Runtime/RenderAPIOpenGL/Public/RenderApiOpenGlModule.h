#pragma once
#include "EFDynamicRAPI.h"
#include <glad/glad.h>


namespace EventfulEngine{
    /* TODO: dynamically loaded graphics API specific implementation for OpenGL that receives calls from RenderAPICore via RenderAPI
     * translates them into OGL domain and issues the actual GPU draw calls
    */


    class EFOpenGLRAPI final : public RefCounter<EFDynamicRAPI>{
    public:
        EFOpenGLRAPI();

        ~EFOpenGLRAPI() override;


        unsigned long AddRef() override;

        unsigned long Release() override;

        HeapHandle CreateHeap(const HeapDesc& d) override;

        TextureHandle CreateTexture(const TextureDesc& d) override;

        MemoryRequirements GetTextureMemoryRequirements(EFRAPITexture* texture) override;

        bool BindTextureMemory(EFRAPITexture* texture, IHeap* heap, uint64_t offset) override;

        TextureHandle
        CreateHandleForNativeTexture(ObjectType objectType, EFRAPIObject texture, const TextureDesc& desc) override;

        StagingTextureHandle CreateStagingTexture(const TextureDesc& d, E_CpuAccessMode cpuAccess) override;

        void* MapStagingTexture(EFRAPIStagingTexture* tex, const TextureSlice& slice, E_CpuAccessMode cpuAccess,
                                size_t* outRowPitch) override;

        void UnmapStagingTexture(EFRAPIStagingTexture* tex) override;

        void GetTextureTiling(EFRAPITexture* texture, uint32_t* numTiles, PackedMipDesc* desc, TileShape* tileShape,
                              uint32_t* subresourceTilingsNum, SubresourceTiling* subresourceTilings) override;

        void UpdateTextureTileMappings(EFRAPITexture* texture, const TextureTilesMapping* tileMappings,
                                       uint32_t numTileMappings, E_CommandQueue executionQueue) override;

        SamplerFeedbackTextureHandle CreateSamplerFeedbackTexture(EFRAPITexture* pairedTexture,
                                                                  const SamplerFeedbackTextureDesc& desc) override;

        SamplerFeedbackTextureHandle CreateSamplerFeedbackForNativeTexture(ObjectType objectType, EFRAPIObject texture,
                                                                           EFRAPITexture* pairedTexture) override;

        BufferHandle CreateBuffer(const BufferDesc& d) override;

        void* MapBuffer(EFRAPIBuffer* buffer, E_CpuAccessMode cpuAccess) override;

        void UnmapBuffer(EFRAPIBuffer* buffer) override;

        MemoryRequirements GetBufferMemoryRequirements(EFRAPIBuffer* buffer) override;

        bool BindBufferMemory(EFRAPIBuffer* buffer, IHeap* heap, uint64_t offset) override;

        BufferHandle
        CreateHandleForNativeBuffer(ObjectType objectType, EFRAPIObject buffer, const BufferDesc& desc) override;

        ShaderHandle CreateShader(const ShaderDesc& d, const void* binary, size_t binarySize) override;

        ShaderHandle CreateShaderSpecialization(EFRAPIShader* baseShader, const ShaderSpecialization* constants,
                                                uint32_t numConstants) override;

        ShaderLibraryHandle CreateShaderLibrary(const void* binary, size_t binarySize) override;

        SamplerHandle CreateSampler(const SamplerDesc& d) override;

        InputLayoutHandle CreateInputLayout(const VertexAttributeDesc* d, uint32_t attributeCount,
                                            EFRAPIShader* vertexShader) override;

        EventQueryHandle CreateEventQuery() override;

        void SetEventQuery(EFRAPIEventQuery* query, E_CommandQueue queue) override;

        bool PollEventQuery(EFRAPIEventQuery* query) override;

        void WaitEventQuery(EFRAPIEventQuery* query) override;

        void ResetEventQuery(EFRAPIEventQuery* query) override;

        TimerQueryHandle CreateTimerQuery() override;

        bool PollTimerQuery(EFRAPITimerQuery* query) override;

        float GetTimerQueryTime(EFRAPITimerQuery* query) override;

        void ResetTimerQuery(EFRAPITimerQuery* query) override;

        E_RenderAPI GetGraphicsAPI() override;

        FramebufferHandle CreateFramebuffer(const FramebufferDesc& desc) override;

        GraphicsPipelineHandle CreateGraphicsPipeline(const GraphicsPipelineDesc& desc, EFRAPIFramebuffer* fb) override;

        ComputePipelineHandle CreateComputePipeline(const ComputePipelineDesc& desc) override;

        MeshletPipelineHandle CreateMeshletPipeline(const MeshletPipelineDesc& desc, EFRAPIFramebuffer* fb) override;

        BindingLayoutHandle CreateBindingLayout(const BindingLayoutDesc& desc) override;

        BindingLayoutHandle CreateBindlessLayout(const BindlessLayoutDesc& desc) override;

        BindingSetHandle CreateBindingSet(const BindingSetDesc& desc, EFRAPIBindingLayout* layout) override;

        DescriptorTableHandle CreateDescriptorTable(EFRAPIBindingLayout* layout) override;

        void ResizeDescriptorTable(IDescriptorTable* descriptorTable, uint32_t newSize, bool keepContents) override;

        bool WriteDescriptorTable(IDescriptorTable* descriptorTable, const BindingSetItem& item) override;

        CommandListHandle CreateCommandList(const CommandListParameters& params) override;

        uint64 ExecuteCommandLists(EFRAPICommandList* const* pCommandLists, size_t numCommandLists,
                                   E_CommandQueue executionQueue) override;

        void QueueWaitForCommandList(E_CommandQueue waitQueue, E_CommandQueue executionQueue,
                                     uint64_t instance) override;

        bool WaitForIdle() override;

        void RunGarbageCollection() override;

        bool QueryFeatureSupport(E_Feature feature, void* pInfo, size_t infoSize) override;

        F_FormatSupport QueryFormatSupport(E_Format format) override;

        EFRAPIObject GetNativeQueue(ObjectType objectType, E_CommandQueue queue) override;

        EFRAPIMessageCallback* GetMessageCallback() override;

        bool IsAftermathEnabled() override;

        EFRAPIObject getNativeObject() override;

    };

    class RenderAPIOpenGLModule : public IDynamicRAPIModule{
    public:
        DynamicRAPIHandle CreateDynamicRAPI(E_Feature requestedFeatureLevel) override;

    };

} // EventfulEngine
