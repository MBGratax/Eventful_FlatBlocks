#pragma once

#include <IModule.h>
#include <mutex>

#include "EFRAPICommandList.h"

namespace EventfulEngine{

    class EFDynamicRAPI : public EFRAPIResource{
    public:
        virtual HeapHandle CreateHeap(const HeapDesc& d) = 0;

        virtual TextureHandle CreateTexture(const TextureDesc& d) = 0;

        virtual MemoryRequirements GetTextureMemoryRequirements(EFRAPITexture* texture) = 0;

        virtual bool BindTextureMemory(EFRAPITexture* texture, IHeap* heap, uint64_t offset) = 0;

        virtual TextureHandle CreateHandleForNativeTexture(ObjectType objectType, EFRAPIObject texture,
                                                           const TextureDesc& desc) = 0;

        virtual StagingTextureHandle CreateStagingTexture(const TextureDesc& d, E_CpuAccessMode cpuAccess) = 0;

        virtual void* MapStagingTexture(EFRAPIStagingTexture* tex, const TextureSlice& slice, E_CpuAccessMode cpuAccess,
                                        size_t* outRowPitch) = 0;

        virtual void UnmapStagingTexture(EFRAPIStagingTexture* tex) = 0;

        virtual void GetTextureTiling(EFRAPITexture* texture, uint32_t* numTiles, PackedMipDesc* desc,
                                      TileShape* tileShape,
                                      uint32_t* subresourceTilingsNum, SubresourceTiling* subresourceTilings) = 0;

        virtual void UpdateTextureTileMappings(EFRAPITexture* texture, const TextureTilesMapping* tileMappings,
                                               uint32_t numTileMappings,
                                               E_CommandQueue executionQueue/* = E_CommandQueue::Graphics*/) = 0;

        virtual SamplerFeedbackTextureHandle CreateSamplerFeedbackTexture(
            EFRAPITexture* pairedTexture, const SamplerFeedbackTextureDesc& desc) = 0;

        virtual SamplerFeedbackTextureHandle CreateSamplerFeedbackForNativeTexture(
            ObjectType objectType, EFRAPIObject texture, EFRAPITexture* pairedTexture) = 0;

        virtual BufferHandle CreateBuffer(const BufferDesc& d) = 0;

        //TODO: Opaque handle for API specific buffer handles
        virtual void* MapBuffer(EFRAPIBuffer* buffer, E_CpuAccessMode cpuAccess) = 0;

        virtual void UnmapBuffer(EFRAPIBuffer* buffer) = 0;

        virtual MemoryRequirements GetBufferMemoryRequirements(EFRAPIBuffer* buffer) = 0;

        virtual bool BindBufferMemory(EFRAPIBuffer* buffer, IHeap* heap, uint64_t offset) = 0;

        virtual BufferHandle CreateHandleForNativeBuffer(ObjectType objectType, EFRAPIObject buffer, const BufferDesc& desc) =
        0;

        //TODO: Opaque handle for ShaderBinary
        virtual ShaderHandle CreateShader(const ShaderDesc& d, const void* binary, size_t binarySize) = 0;

        virtual ShaderHandle CreateShaderSpecialization(EFRAPIShader* baseShader, const ShaderSpecialization* constants,
                                                        uint32_t numConstants) = 0;

        virtual ShaderLibraryHandle CreateShaderLibrary(const void* binary, size_t binarySize) = 0;

        virtual SamplerHandle CreateSampler(const SamplerDesc& d) = 0;

        // Note: vertexShader is only necessary on D3D11, otherwise it may be null
        virtual InputLayoutHandle CreateInputLayout(const VertexAttributeDesc* d, uint32_t attributeCount,
                                                    EFRAPIShader* vertexShader) = 0;

        // Event queries
        virtual EventQueryHandle CreateEventQuery() = 0;

        virtual void SetEventQuery(EFRAPIEventQuery* query, E_CommandQueue queue) = 0;

        virtual bool PollEventQuery(EFRAPIEventQuery* query) = 0;

        virtual void WaitEventQuery(EFRAPIEventQuery* query) = 0;

        virtual void ResetEventQuery(EFRAPIEventQuery* query) = 0;

        // Timer queries - see also begin/endTimerQuery in ICommandList
        virtual TimerQueryHandle CreateTimerQuery() = 0;

        virtual bool PollTimerQuery(EFRAPITimerQuery* query) = 0;

        // returns time in seconds
        virtual float GetTimerQueryTime(EFRAPITimerQuery* query) = 0;

        virtual void ResetTimerQuery(EFRAPITimerQuery* query) = 0;

        // Returns the API kind that the RHI backend is running on top of.
        virtual E_RenderAPI GetGraphicsAPI() = 0;

        virtual FramebufferHandle CreateFramebuffer(const FramebufferDesc& desc) = 0;

        virtual GraphicsPipelineHandle CreateGraphicsPipeline(const GraphicsPipelineDesc& desc, EFRAPIFramebuffer* fb) =
        0;

        virtual ComputePipelineHandle CreateComputePipeline(const ComputePipelineDesc& desc) = 0;

        virtual MeshletPipelineHandle CreateMeshletPipeline(const MeshletPipelineDesc& desc, EFRAPIFramebuffer* fb) = 0;

        virtual BindingLayoutHandle CreateBindingLayout(const BindingLayoutDesc& desc) = 0;

        virtual BindingLayoutHandle CreateBindlessLayout(const BindlessLayoutDesc& desc) = 0;

        virtual BindingSetHandle CreateBindingSet(const BindingSetDesc& desc, EFRAPIBindingLayout* layout) = 0;

        virtual DescriptorTableHandle CreateDescriptorTable(EFRAPIBindingLayout* layout) = 0;

        virtual void ResizeDescriptorTable(IDescriptorTable* descriptorTable, uint32_t newSize,
                                           bool keepContents/* = true*/) = 0;

        virtual bool WriteDescriptorTable(IDescriptorTable* descriptorTable, const BindingSetItem& item) = 0;

        virtual CommandListHandle CreateCommandList(const CommandListParameters& params /*= CommandListParameters()*/) =
        0;

        virtual uint64 ExecuteCommandLists(EFRAPICommandList* const* pCommandLists, size_t numCommandLists,
                                           E_CommandQueue executionQueue/* = E_CommandQueue::Graphics*/) = 0;

        virtual void QueueWaitForCommandList(E_CommandQueue waitQueue, E_CommandQueue executionQueue,
                                             uint64_t instance) =
        0;

        // returns true if the wait completes successfully, false if detecting a problem (e.g. device removal)
        virtual bool WaitForIdle() = 0;

        //Ideally at least once per frame
        virtual void RunGarbageCollection() = 0;

        //TODO: Opaque handle for API device info to query featuer support
        virtual bool QueryFeatureSupport(E_Feature feature, void* pInfo/* = nullptr*/, size_t infoSize/* = 0*/) = 0;

        virtual F_FormatSupport QueryFormatSupport(E_Format format) = 0;

        virtual EFRAPIObject GetNativeQueue(ObjectType objectType, E_CommandQueue queue) = 0;

        virtual EFRAPIMessageCallback* GetMessageCallback() = 0;

        virtual bool IsAftermathEnabled() = 0;

        // Front-end for executeCommandLists(..., 1) for compatibility and convenience
        uint64 ExecuteCommandList(EFRAPICommandList* commandList,
                                  const E_CommandQueue executionQueue = E_CommandQueue::Graphics){
            return ExecuteCommandLists(&commandList, 1, executionQueue);
        }
    };

    using DynamicRAPIHandle = RefCountPtr<EFDynamicRAPI>;

    class IDynamicRAPIModule : public IModule{
    public:
        virtual DynamicRAPIHandle CreateDynamicRAPI(E_Feature requestedFeatureLevel) = 0;
    };

    EFRENDERAPI_API BlendState::RenderTarget CreateAddBlendState(
        E_BlendFactor srcBlend,
        E_BlendFactor dstBlend);


    EFRENDERAPI_API BufferDesc CreateStaticConstantBufferDesc(
        uint32 byteSize,
        const EFChar* debugName);

    EFRENDERAPI_API BufferDesc CreateVolatileConstantBufferDesc(
        uint32_t byteSize,
        const EFChar* debugName,
        uint32 maxVersions);

    EFRENDERAPI_API bool CreateBindingSetAndLayout(
        F_ShaderType visibility,
        uint32 registerSpace,
        const BindingSetDesc& bindingSetDesc, BindingLayoutHandle& bindingLayout,
        BindingSetHandle& bindingSet,
        bool registerSpaceIsDescriptorSet = false);

    EFRENDERAPI_API void ClearColorAttachment(
        EFRAPICommandList* commandList,
        const EFRAPIFramebuffer* framebuffer,
        uint32_t attachmentIndex,
        Color color
    );

    EFRENDERAPI_API void ClearDepthStencilAttachment(
        EFRAPICommandList* commandList,
        const EFRAPIFramebuffer* framebuffer,
        float depth,
        uint32_t stencil
    );

    // Places a UAV barrier on the provided texture.
    // Useful when doing multiple consecutive dispatch calls with the same resources but different constants.
    // Ignored if there was a call to setEnableUavBarriersForTexrure(..., false) on this texture.
    EFRENDERAPI_API void TextureUavBarrier(
        EFRAPICommandList* commandList,
        EFRAPITexture* texture);

    // Places a UAV barrier on the provided buffer.
    // Useful when doing multiple consecutive dispatch calls with the same resources but different constants.
    // Ignored if there was a call to setEnableUavBarriersForBuffer(..., false) on this buffer.
    EFRENDERAPI_API void BufferUavBarrier(
        EFRAPICommandList* commandList,
        EFRAPIBuffer* buffer);

    // Selects a format from the supplied list that supports all the required features on the given device.
    // The formats are tested in the same order they're provided, and the first matching one is returned.
    // If no formats are matching, Format::UNKNOWN is returned.
    EFRENDERAPI_API E_Format ChooseFormat(
        F_FormatSupport requiredFeatures,
        const E_Format* requestedFormats,
        size_t requestedFormatCount);

    EFRENDERAPI_API const EFChar* GraphicsApiToString(E_RenderAPI api);

    EFRENDERAPI_API const EFChar* TextureDimensionToString(E_TextureDimension dimension);

    EFRENDERAPI_API const EFChar* DebugNameToString(const EFString& debugName);

    EFRENDERAPI_API const EFChar* ShaderStageToString(F_ShaderType stage);

    EFRENDERAPI_API const EFChar* ResourceTypeToString(E_ResourceType type);

    EFRENDERAPI_API const EFChar* FormatToString(E_Format format);

    EFRENDERAPI_API const EFChar* CommandQueueToString(E_CommandQueue queue);

    EFString GenerateHeapDebugName(const HeapDesc& desc);

    EFString GenerateTextureDebugName(const TextureDesc& desc);

    EFString GenerateBufferDebugName(const BufferDesc& desc);

    void NotImplemented();

    void NotSupported();

    void InvalidEnum();

    class BitSetAllocator{
    public:
        explicit BitSetAllocator(size_t capacity, bool multithreaded);

        int Allocate();

        void Release(int index);

        [[nodiscard]] size_t GetCapacity() const{ return _allocated.size(); }

    private:
        int _nextAvailable = 0;
        std::vector<bool> _allocated;
        bool b_isMultiThreaded;
        std::mutex _mutex;
    };

    // Automatic begin/end marker for command list
    class ScopedMarker{
    public:
        EFRAPICommandList* CommandList;

        NOMOVEORCOPY(ScopedMarker)

        ScopedMarker(EFRAPICommandList* commandList, const EFChar* markerName) : CommandList(commandList){
            CommandList->BeginMarker(markerName);
        }

        ScopedMarker(const CommandListHandle* commandList, const EFChar* markerName) :
            ScopedMarker(commandList->Get(), markerName){
        }

        ~ScopedMarker(){
            CommandList->EndMarker();
        }
    };
}
