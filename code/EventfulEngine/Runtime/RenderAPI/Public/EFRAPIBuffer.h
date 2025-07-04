#pragma once

#include "EFRAPITexture.h"
#include "EFStaticArray.h"

namespace EventfulEngine{

    struct BufferDesc{
        uint64 ByteSize = 0;
        uint32 StructStride = 0; // if non-zero it's structured
        uint32 MaxVersions = 0; // only valid and required to be nonzero for volatile buffers on Vulkan
        EFString DebugName;
        E_Format Format = E_Format::UNKNOWN; // for typed buffer views
        bool B_CanHaveUAVs = false;
        bool B_CanHaveTypedViews = false;
        bool B_CanHaveRawViews = false;
        bool B_IsVertexBuffer = false;
        bool B_IsIndexBuffer = false;
        bool B_IsConstantBuffer = false;
        bool B_IsDrawIndirectArgs = false;
        bool B_IsAccelStructBuildInput = false;
        bool B_IsAccelStructStorage = false;
        bool B_IsShaderBindingTable = false;

        // A dynamic/upload buffer whose contents only live in the current command list
        bool B_IsVolatile = false;

        // Indicates that the buffer is created with no backing memory,
        // and memory is bound to the buffer later using bindBufferMemory.
        // On DX12, the buffer resource is created at the time of memory binding.
        bool B_IsVirtual = false;

        F_ResourceStates InitialState = F_ResourceStates::Common;

        // see TextureDesc::keepInitialState
        bool B_KeepInitialState = false;

        E_CpuAccessMode CpuAccess = E_CpuAccessMode::None;

        F_SharedResourceFlags SharedResourceFlags = F_SharedResourceFlags::None;

        constexpr BufferDesc& SetByteSize(const uint64 value){
            ByteSize = value;
            return *this;
        }

        constexpr BufferDesc& SetStructStride(const uint32 value){
            StructStride = value;
            return *this;
        }

        constexpr BufferDesc& SetMaxVersions(const uint32 value){
            MaxVersions = value;
            return *this;
        }

        BufferDesc& SetDebugName(const std::string_view& value){
            DebugName = value;
            return *this;
        }

        constexpr BufferDesc& SetFormat(const E_Format value){
            Format = value;
            return *this;
        }

        constexpr BufferDesc& SetCanHaveUAVs(const bool value){
            B_CanHaveUAVs = value;
            return *this;
        }

        constexpr BufferDesc& SetCanHaveTypedViews(const bool value){
            B_CanHaveTypedViews = value;
            return *this;
        }

        constexpr BufferDesc& SetCanHaveRawViews(const bool value){
            B_CanHaveRawViews = value;
            return *this;
        }

        constexpr BufferDesc& SetIsVertexBuffer(const bool value){
            B_IsVertexBuffer = value;
            return *this;
        }

        constexpr BufferDesc& SetIsIndexBuffer(const bool value){
            B_IsIndexBuffer = value;
            return *this;
        }

        constexpr BufferDesc& SetIsConstantBuffer(const bool value){
            B_IsConstantBuffer = value;
            return *this;
        }

        constexpr BufferDesc& SetIsDrawIndirectArgs(const bool value){
            B_IsDrawIndirectArgs = value;
            return *this;
        }

        constexpr BufferDesc& SetIsAccelStructBuildInput(const bool value){
            B_IsAccelStructBuildInput = value;
            return *this;
        }

        constexpr BufferDesc& SetIsAccelStructStorage(const bool value){
            B_IsAccelStructStorage = value;
            return *this;
        }

        constexpr BufferDesc& SetIsShaderBindingTable(const bool value){
            B_IsShaderBindingTable = value;
            return *this;
        }

        constexpr BufferDesc& SetIsVolatile(const bool value){
            B_IsVolatile = value;
            return *this;
        }

        constexpr BufferDesc& SetIsVirtual(const bool value){
            B_IsVirtual = value;
            return *this;
        }

        constexpr BufferDesc& SetInitialState(const F_ResourceStates value){
            InitialState = value;
            return *this;
        }

        constexpr BufferDesc& SetKeepInitialState(const bool value){
            B_KeepInitialState = value;
            return *this;
        }

        constexpr BufferDesc& SetCpuAccess(const E_CpuAccessMode value){
            CpuAccess = value;
            return *this;
        }
    };

    struct BufferRange{
        uint64 ByteOffset = 0;
        uint64 ByteSize = 0;

        BufferRange() = default;

        BufferRange(const uint64 byteOffset, const uint64 byteSize)
            : ByteOffset(byteOffset)
              , ByteSize(byteSize){
        }

        [[nodiscard]] EFRENDERAPI_API BufferRange Resolve(const BufferDesc& desc) const;

        [[nodiscard]] constexpr bool IsEntireBuffer(const BufferDesc& desc) const{
            return (ByteOffset == 0) && (ByteSize == ~0uLL || ByteSize == desc.ByteSize);
        }

        constexpr bool operator==(const BufferRange& other) const = default;

        constexpr BufferRange& SetByteOffset(const uint64 value){
            ByteOffset = value;
            return *this;
        }

        constexpr BufferRange& SetByteSize(const uint64 value){
            ByteSize = value;
            return *this;
        }
    };

    static const auto EntireBuffer = BufferRange(0, ~0uLL);

    class EFRAPIBuffer : public EFRAPIResource{
    public:
        [[nodiscard]] virtual const BufferDesc& GetDesc() const = 0;
    };

    using BufferHandle = RefCountPtr<EFRAPIBuffer>;

    //////////////////////////////////////////////////////////////////////////
    // Framebuffer
    //////////////////////////////////////////////////////////////////////////

    struct FramebufferAttachment{
        EFRAPITexture* Texture = nullptr;
        TextureSubresourceSet Subresources = TextureSubresourceSet(0, 1, 0, 1);
        E_Format Format = E_Format::UNKNOWN;
        bool B_IsReadOnly = false;

        constexpr FramebufferAttachment& SetTexture(EFRAPITexture* t){
            Texture = t;
            return *this;
        }

        constexpr FramebufferAttachment& SetSubresources(const TextureSubresourceSet value){
            Subresources = value;
            return *this;
        }

        constexpr FramebufferAttachment& SetArraySlice(const ArraySlice index){
            Subresources.BaseArraySlice = index;
            Subresources.NumArraySlices = 1;
            return *this;
        }

        constexpr FramebufferAttachment& SetArraySliceRange(const ArraySlice index, const ArraySlice count){
            Subresources.BaseArraySlice = index;
            Subresources.NumArraySlices = count;
            return *this;
        }

        constexpr FramebufferAttachment& SetMipLevel(const MipLevel level){
            Subresources.BaseMipLevel = level;
            Subresources.NumMipLevels = 1;
            return *this;
        }

        constexpr FramebufferAttachment& SetFormat(const E_Format format){
            Format = format;
            return *this;
        }

        constexpr FramebufferAttachment& SetReadOnly(const bool readonly){
            B_IsReadOnly = readonly;
            return *this;
        }

        [[nodiscard]] bool IsValid() const{ return Texture != nullptr; }
    };

    struct EFRENDERAPI_API FramebufferDesc{
        EFStaticArray<FramebufferAttachment, c_MaxRenderTargets> ColorAttachments;
        FramebufferAttachment DepthAttachment;
        FramebufferAttachment ShadingRateAttachment;

        FramebufferDesc& AddColorAttachment(const FramebufferAttachment& attachment){
            ColorAttachments.push_back(attachment);
            return *this;
        }

        FramebufferDesc& AddColorAttachment(EFRAPITexture* texture){
            ColorAttachments.push_back(FramebufferAttachment().SetTexture(texture));
            return *this;
        }

        FramebufferDesc& AddColorAttachment(EFRAPITexture* texture, const TextureSubresourceSet subresources){
            ColorAttachments.push_back(FramebufferAttachment().SetTexture(texture).SetSubresources(subresources));
            return *this;
        }

        FramebufferDesc& SetDepthAttachment(const FramebufferAttachment& attachment){
            DepthAttachment = attachment;
            return *this;
        }

        FramebufferDesc& SetDepthAttachment(EFRAPITexture* texture){
            DepthAttachment = FramebufferAttachment().SetTexture(texture);
            return *this;
        }

        FramebufferDesc& SetDepthAttachment(EFRAPITexture* texture, const TextureSubresourceSet subresources){
            DepthAttachment = FramebufferAttachment().SetTexture(texture).SetSubresources(subresources);
            return *this;
        }

        FramebufferDesc& SetShadingRateAttachment(const FramebufferAttachment& d){
            ShadingRateAttachment = d;
            return *this;
        }

        FramebufferDesc& SetShadingRateAttachment(EFRAPITexture* texture){
            ShadingRateAttachment = FramebufferAttachment().SetTexture(texture);
            return *this;
        }

        FramebufferDesc& SetShadingRateAttachment(EFRAPITexture* texture, const TextureSubresourceSet subresources){
            ShadingRateAttachment = FramebufferAttachment().SetTexture(texture).SetSubresources(subresources);
            return *this;
        }
    };

    // Describes the parameters of a framebuffer that can be used to determine if a given framebuffer
    // is compatible with a certain graphics or meshlet pipeline object. All fields of FramebufferInfo
    // must match between the framebuffer and the pipeline for them to be compatible.
    struct FramebufferInfo{
        EFStaticArray<E_Format, c_MaxRenderTargets> colorFormats;
        E_Format depthFormat = E_Format::UNKNOWN;
        uint32_t sampleCount = 1;
        uint32_t sampleQuality = 0;

        FramebufferInfo() = default;

        EFRENDERAPI_API explicit FramebufferInfo(const FramebufferDesc& desc);

        bool operator==(const FramebufferInfo& other) const{
            return formatsEqual(colorFormats, other.colorFormats)
                && depthFormat == other.depthFormat
                && sampleCount == other.sampleCount
                && sampleQuality == other.sampleQuality;
        }

    private:
        static bool formatsEqual(const EFStaticArray<E_Format, c_MaxRenderTargets>& a,
                                 const EFStaticArray<E_Format, c_MaxRenderTargets>& b){
            if (a.size() != b.size()){
                return false;
            }
            const size_t size = a.size();
            for (size_t i = 0; i < size; i++){
                if (a[i] != b[i]){
                    return false;
                }
            }
            return true;
        }
    };

    // An extended version of FramebufferInfo that also contains the 'width' and 'height' members.
    // It is provided mostly for backward compatibility and convenience reasons, as previously these members
    // were available in the regular FramebufferInfo structure.
    struct FramebufferInfoEx : FramebufferInfo{
        uint32 Width = 0;
        uint32 Height = 0;

        FramebufferInfoEx() = default;

        EFRENDERAPI_API explicit FramebufferInfoEx(const FramebufferDesc& desc);

        [[nodiscard]] Viewport GetViewport(const float minZ = 0.f, const float maxZ = 1.f) const{
            return {0.f, static_cast<float>(Width), 0.f, static_cast<float>(Height), minZ, maxZ};
        }
    };

    class EFRAPIFramebuffer : public EFRAPIResource{
    public:
        [[nodiscard]] virtual const FramebufferDesc& GetDesc() const = 0;

        [[nodiscard]] virtual const FramebufferInfoEx& GetFramebufferInfo() const = 0;
    };

    using FramebufferHandle = RefCountPtr<EFRAPIFramebuffer>;

} // EventfulEngine
