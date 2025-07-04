#pragma once

namespace EventfulEngine{
    enum class E_Feature : uint8_t{
        ComputeQueue,
        ConservativeRasterization,
        ConstantBufferRanges,
        CopyQueue,
        DeferredCommandLists,
        FastGeometryShader,
        HeapDirectlyIndexed,
        HlslExtensionUAV,
        LinearSweptSpheres,
        Meshlets,
        RayQuery,
        RayTracingAccelStruct,
        RayTracingClusters,
        RayTracingOpacityMicromap,
        RayTracingPipeline,
        SamplerFeedback,
        ShaderExecutionReordering,
        ShaderSpecializations,
        SinglePassStereo,
        Spheres,
        VariableRateShading,
        VirtualResources,
        WaveLaneCountMinMax,
    };

    enum class E_MessageSeverity : uint8_t{
        Info,
        Warning,
        Error,
        Fatal
    };

    enum class E_CommandQueue : uint8_t{
        Graphics = 0,
        Compute,
        Copy,

        Count
    };

    struct VariableRateShadingFeatureInfo{
        uint32_t shadingRateImageTileSize;
    };

    struct WaveLaneCountMinMaxFeatureInfo{
        uint32_t minWaveLaneCount;
        uint32_t maxWaveLaneCount;
    };

    // IMessageCallback should be implemented by the application.
    class EFRAPIMessageCallback{
    protected:
        EFRAPIMessageCallback() = default;

        virtual ~EFRAPIMessageCallback() = default;

    public:
        // NVRHI will call message(...) whenever it needs to signal something.
        // The application is free to ignore the messages, show message boxes, or terminate.
        virtual void message(E_MessageSeverity severity, const char* messageText) = 0;

        EFRAPIMessageCallback(const EFRAPIMessageCallback&) = delete;

        EFRAPIMessageCallback(const EFRAPIMessageCallback&&) = delete;

        EFRAPIMessageCallback& operator=(const EFRAPIMessageCallback&) = delete;

        EFRAPIMessageCallback& operator=(const EFRAPIMessageCallback&&) = delete;
    };

    class EFDynamicRAPI;

    struct CommandListParameters{
        // A command list with enableImmediateExecution = true maps to the immediate context on DX11.
        // Two immediate command lists cannot be open at the same time, which is checked by the validation layer.
        bool enableImmediateExecution = true;

        // Minimum size of memory chunks created to upload data to the device on DX12.
        size_t uploadChunkSize = 64 * 1024;

        // Minimum size of memory chunks created for AS build scratch buffers.
        size_t scratchChunkSize = 64 * 1024;

        // Maximum total memory size used for all AS build scratch buffers owned by this command list.
        size_t scratchMaxMemory = 1024 * 1024 * 1024;

        // Type of the queue that this command list is to be executed on.
        // COPY and COMPUTE queues have limited subsets of methods available.
        E_CommandQueue queueType = E_CommandQueue::Graphics;

        CommandListParameters& setEnableImmediateExecution(bool value){
            enableImmediateExecution = value;
            return *this;
        }

        CommandListParameters& setUploadChunkSize(size_t value){
            uploadChunkSize = value;
            return *this;
        }

        CommandListParameters& setScratchChunkSize(size_t value){
            scratchChunkSize = value;
            return *this;
        }

        CommandListParameters& setScratchMaxMemory(size_t value){
            scratchMaxMemory = value;
            return *this;
        }

        CommandListParameters& setQueueType(E_CommandQueue value){
            queueType = value;
            return *this;
        }
    };
    template <class T>
void hash_combine(size_t& seed, const T& v){
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
}
