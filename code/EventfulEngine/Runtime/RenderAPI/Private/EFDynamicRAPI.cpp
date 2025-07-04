#pragma once

#include "EFDynamicRAPI.h"

#include <RenderAPI.h>

namespace EventfulEngine{

    BlendState::RenderTarget CreateAddBlendState(const E_BlendFactor srcBlend, const E_BlendFactor dstBlend){
        BlendState::RenderTarget target;
        target.blendEnable = true;
        target.blendOp = E_BlendOp::Add;
        target.srcBlend = srcBlend;
        target.destBlend = dstBlend;
        target.srcBlendAlpha = E_BlendFactor::Zero;
        target.destBlendAlpha = E_BlendFactor::One;
        return target;
    }

    BufferDesc CreateStaticConstantBufferDesc(const uint32 byteSize, const EFChar* debugName){
        BufferDesc constantBufferDesc;
        constantBufferDesc.ByteSize = byteSize;
        constantBufferDesc.DebugName = debugName;
        constantBufferDesc.B_IsConstantBuffer = true;
        constantBufferDesc.B_IsVolatile = false;
        return constantBufferDesc;
    }

    BufferDesc CreateVolatileConstantBufferDesc(const uint32 byteSize, const EFChar* debugName,
                                                const uint32 maxVersions){
        BufferDesc constantBufferDesc;
        constantBufferDesc.ByteSize = byteSize;
        constantBufferDesc.DebugName = debugName;
        constantBufferDesc.B_IsConstantBuffer = true;
        constantBufferDesc.B_IsVolatile = true;
        constantBufferDesc.MaxVersions = maxVersions;
        return constantBufferDesc;
    }

    bool CreateBindingSetAndLayout(const F_ShaderType visibility, const uint32 registerSpace,
                                   const BindingSetDesc& bindingSetDesc, BindingLayoutHandle& bindingLayout,
                                   BindingSetHandle& bindingSet,
                                   const bool registerSpaceIsDescriptorSet){
        auto convertSetToLayout = [](const std::vector<BindingSetItem>& setDesc,
                                     std::vector<BindingLayoutItem>& layoutDesc){
            for (auto& item : setDesc){
                BindingLayoutItem layoutItem{};
                layoutItem.slot = item.slot;
                layoutItem.type = item.type;
                layoutItem.size = 1;
                if (item.type == E_ResourceType::PushConstants)
                    layoutItem.size = static_cast<uint16>(item.range.ByteSize);
                layoutDesc.push_back(layoutItem);
            }
        };

        if (!bindingLayout){
            BindingLayoutDesc bindingLayoutDesc;
            bindingLayoutDesc.visibility = visibility;
            bindingLayoutDesc.registerSpace = registerSpace;
            bindingLayoutDesc.registerSpaceIsDescriptorSet = registerSpaceIsDescriptorSet;
            convertSetToLayout(bindingSetDesc.bindings, bindingLayoutDesc.bindings);

            bindingLayout = g_dynamicRAPI->CreateBindingLayout(bindingLayoutDesc);

            if (!bindingLayout){
                return false;
            }
        }

        if (!bindingSet){
            bindingSet = g_dynamicRAPI->CreateBindingSet(bindingSetDesc, bindingLayout);

            if (!bindingSet){
                return false;
            }
        }

        return true;
    }

    void ClearColorAttachment(EFRAPICommandList* commandList, const EFRAPIFramebuffer* framebuffer,
                              const uint32_t attachmentIndex,
                              const Color color){
        if (const FramebufferAttachment& attachment = framebuffer->GetDesc().ColorAttachments[attachmentIndex];
            attachment.Texture){
            commandList->ClearTextureFloat(attachment.Texture, attachment.Subresources, color);
        }
    }

    void ClearDepthStencilAttachment(EFRAPICommandList* commandList, const EFRAPIFramebuffer* framebuffer,
                                     const float depth,
                                     const uint32_t stencil){
        if (const FramebufferAttachment& attachment = framebuffer->GetDesc().DepthAttachment; attachment.Texture){
            const FormatInfo& formatInfo = GetFormatInfo(attachment.Texture->GetDesc().Format);
            commandList->ClearDepthStencilTexture(attachment.Texture, attachment.Subresources, formatInfo.hasDepth,
                                                  depth,
                                                  formatInfo.hasStencil, static_cast<uint8_t>(stencil));
        }
    }

    void TextureUavBarrier(EFRAPICommandList* commandList, EFRAPITexture* texture){
        commandList->SetTextureState(texture, AllSubresources, F_ResourceStates::UnorderedAccess);
    }

    void BufferUavBarrier(EFRAPICommandList* commandList, EFRAPIBuffer* buffer){
        commandList->SetBufferState(buffer, F_ResourceStates::UnorderedAccess);
    }

    E_Format ChooseFormat(const F_FormatSupport requiredFeatures,
                          const E_Format* requestedFormats,
                          const size_t requestedFormatCount){
        assert(g_dynamicRAPI);
        assert(requestedFormats || requestedFormatCount == 0);

        for (size_t i = 0; i < requestedFormatCount; i++){
            // ReSharper disable CppDFANullDereference Cannot be null due to assert above
            if ((g_dynamicRAPI->QueryFormatSupport(requestedFormats[i]) & requiredFeatures) == requiredFeatures)
                // ReSharper restore CppDFANullDereference
                return requestedFormats[i];
        }

        return E_Format::UNKNOWN;
    }

    const EFChar* GraphicsApiToString(const E_RenderAPI api){
        switch (api){
        case E_RenderAPI::Headless: return "Headless";
        case E_RenderAPI::OpenGL: return "OpenGL";
        default: return "<UNKNOWN>";
        }
    }

    const EFChar* TextureDimensionToString(const E_TextureDimension dimension){
        switch (dimension){
            using enum E_TextureDimension;
        case Texture1D: return "Texture1D";
        case Texture1DArray: return "Texture1DArray";
        case Texture2D: return "Texture2D";
        case Texture2DArray: return "Texture2DArray";
        case TextureCube: return "TextureCube";
        case TextureCubeArray: return "TextureCubeArray";
        case Texture2DMS: return "Texture2DMS";
        case Texture2DMSArray: return "Texture2DMSArray";
        case Texture3D: return "Texture3D";
        case Unknown: return "Unknown";
        default: return "<INVALID>";
        }
    }

    const EFChar* DebugNameToString(const EFString& debugName){
        return debugName.empty() ? "<UNNAMED>" : debugName.c_str();
    }

    const EFChar* ShaderStageToString(const F_ShaderType stage){
        switch (stage){
            using enum F_ShaderType;
        case None: return "None";
        case Compute: return "Compute";
        case Vertex: return "Vertex";
        case Hull: return "Hull";
        case Domain: return "Domain";
        case Geometry: return "Geometry";
        case Pixel: return "Pixel";
        case Amplification: return "Amplification";
        case Mesh: return "Mesh";
        case AllGraphics: return "AllGraphics";
        case RayGeneration: return "RayGeneration";
        case AnyHit: return "AnyHit";
        case ClosestHit: return "ClosestHit";
        case Miss: return "Miss";
        case Intersection: return "Intersection";
        case Callable: return "Callable";
        case AllRayTracing: return "AllRayTracing";
        case All: return "All";
        default: return "<INVALID>";
        }
    }

    const EFChar* ResourceTypeToString(const E_ResourceType type){
        switch (type){
            using enum E_ResourceType;
        case None: return "None";
        case Texture_SRV: return "Texture_SRV";
        case Texture_UAV: return "Texture_UAV";
        case TypedBuffer_SRV: return "Buffer_SRV";
        case TypedBuffer_UAV: return "Buffer_UAV";
        case StructuredBuffer_SRV: return "StructuredBuffer_SRV";
        case StructuredBuffer_UAV: return "StructuredBuffer_UAV";
        case RawBuffer_SRV: return "RawBuffer_SRV";
        case RawBuffer_UAV: return "RawBuffer_UAV";
        case ConstantBuffer: return "ConstantBuffer";
        case VolatileConstantBuffer: return "VolatileConstantBuffer";
        case Sampler: return "Sampler";
        case RayTracingAccelStruct: return "RayTracingAccelStruct";
        case PushConstants: return "PushConstants";
        case Count:
        default: return "<INVALID>";
        }
    }

    const EFChar* FormatToString(const E_Format format){
        return GetFormatInfo(format).name;
    }

    const EFChar* CommandQueueToString(const E_CommandQueue queue){
        switch (queue){
            using enum E_CommandQueue;
        case Graphics: return "Graphics";
        case Compute: return "Compute";
        case Copy: return "Copy";
        case Count:
        default:
            return "<INVALID>";
        }
    }

    EFString GenerateHeapDebugName(const HeapDesc& desc){
        std::stringstream ss;

        ss << "Unnamed ";

        switch (desc.type){
            using enum HeapType;
        case DeviceLocal:
            ss << "DeviceLocal";
            break;
        case Upload:
            ss << "Upload";
            break;
        case Readback:
            ss << "Readback";
            break;
        default:
            ss << "Invalid-Type";
            break;
        }

        ss << " Heap (" << desc.capacity << " bytes)";

        return ss.str();
    }

    EFString GenerateTextureDebugName(const TextureDesc& desc){
        using enum E_TextureDimension;
        std::stringstream ss;

        ss << "Unnamed " << TextureDimensionToString(desc.Dimension);
        ss << " (" << GetFormatInfo(desc.Format).name;
        ss << ", Width = " << desc.Width;

        if (desc.Dimension >= Texture2D)
            ss << ", Height = " << desc.Height;

        if (desc.Dimension == Texture3D)
            ss << ", Depth = " << desc.Depth;

        if (desc.Dimension == Texture1DArray ||
            desc.Dimension == Texture2DArray ||
            desc.Dimension == TextureCubeArray ||
            desc.Dimension == Texture2DMSArray)
            ss << ", ArraySize = " << desc.ArraySize;

        if (desc.Dimension == Texture1D ||
            desc.Dimension == Texture1DArray ||
            desc.Dimension == Texture2D ||
            desc.Dimension == Texture2DArray ||
            desc.Dimension == TextureCube ||
            desc.Dimension == TextureCubeArray)
            ss << ", MipLevels = " << desc.MipLevels;

        if (desc.Dimension == Texture2DMS ||
            desc.Dimension == Texture2DMSArray)
            ss << ", SampleCount = " << desc.SampleCount << ", SampleQuality = " << desc.SampleQuality;

        if (desc.B_IsRenderTarget) ss << ", IsRenderTarget";
        if (desc.B_IsUAV) ss << ", IsUAV";
        if (desc.B_IsTypeless) ss << ", IsTypeless";

        ss << ")";

        return ss.str();
    }

    EFString GenerateBufferDebugName(const BufferDesc& desc){
        std::stringstream ss;

        ss << "Unnamed Buffer (ByteSize = " << desc.ByteSize;

        if (desc.Format != E_Format::UNKNOWN)
            ss << ", Format = " << GetFormatInfo(desc.Format).name;

        if (desc.StructStride > 0)
            ss << ", StructStride = " << desc.StructStride;

        if (desc.B_IsVolatile)
            ss << ", IsVolatile, MaxVersions = " << desc.MaxVersions;

        if (desc.B_CanHaveUAVs) ss << ", CanHaveUAVs";
        if (desc.B_CanHaveTypedViews) ss << ", CanHaveTypedViews";
        if (desc.B_CanHaveRawViews) ss << ", CanHaveRawViews";
        if (desc.B_IsVertexBuffer) ss << ", IsVertexBuffer";
        if (desc.B_IsIndexBuffer) ss << ", IsIndexBuffer";
        if (desc.B_IsConstantBuffer) ss << ", IsConstantBuffer";
        if (desc.B_IsDrawIndirectArgs) ss << ", IsDrawIndirectArgs";
        if (desc.B_IsAccelStructBuildInput) ss << ", IsAccelStructBuildInput";
        if (desc.B_IsAccelStructStorage) ss << ", IsAccelStructStorage";
        if (desc.B_IsShaderBindingTable) ss << ", IsShaderBindingTable";

        ss << ")";

        return ss.str();
    }

    [[noreturn]] void NotImplemented(){
        assert(!"Not Implemented");
    }

    [[noreturn]] void NotSupported(){
        assert(!"Not Supported");
    }

    [[noreturn]] void InvalidEnum(){
        assert(!"Invalid Enumeration Value");
    }

    BitSetAllocator::BitSetAllocator(const size_t capacity,
                                     const bool multithreaded) : b_isMultiThreaded(multithreaded){
        _allocated.resize(capacity);
    }

    int BitSetAllocator::Allocate(){
        std::unique_lock<std::mutex> lock = b_isMultiThreaded
                                                ? std::unique_lock(_mutex)
                                                : std::unique_lock<std::mutex>();

        int result = -1;

        const auto capacity = static_cast<int>(_allocated.size());
        for (int i = 0; i < capacity; i++){
            if (const int next = (_nextAvailable + i) % capacity; !_allocated[next]){
                result = next;
                _nextAvailable = (next + 1) % capacity;
                _allocated[next] = true;
                break;
            }
        }

        return result;
    }

    void BitSetAllocator::Release(const int index){
        if (index >= 0 && index < static_cast<int>(_allocated.size())){
            std::unique_lock<std::mutex> lock = b_isMultiThreaded
                                                    ? std::unique_lock(_mutex)
                                                    : std::unique_lock<std::mutex>();

            _allocated[index] = false;
            _nextAvailable = std::min(_nextAvailable, index);
        }
    }
}
