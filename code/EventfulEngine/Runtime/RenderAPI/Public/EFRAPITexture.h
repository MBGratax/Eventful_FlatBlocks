#pragma once

#include "EFRAPIResource.h"
#include "EFRAPIBasicTypes.h"
#include "EFRAPIMemory.h"
#include "EFRAPIMisc.h"

namespace EventfulEngine{

    enum class E_TextureDimension : uint8{
        Unknown,
        Texture1D,
        Texture1DArray,
        Texture2D,
        Texture2DArray,
        TextureCube,
        TextureCubeArray,
        Texture2DMS,
        Texture2DMSArray,
        Texture3D
    };

    enum class E_CpuAccessMode : uint8{
        None,
        Read,
        Write
    };

    enum class F_ResourceStates : uint32{
        Unknown = 0,
        Common = 0x00000001,
        ConstantBuffer = 0x00000002,
        VertexBuffer = 0x00000004,
        IndexBuffer = 0x00000008,
        IndirectArgument = 0x00000010,
        ShaderResource = 0x00000020,
        UnorderedAccess = 0x00000040,
        RenderTarget = 0x00000080,
        DepthWrite = 0x00000100,
        DepthRead = 0x00000200,
        StreamOut = 0x00000400,
        CopyDest = 0x00000800,
        CopySource = 0x00001000,
        ResolveDest = 0x00002000,
        ResolveSource = 0x00004000,
        Present = 0x00008000,
        AccelStructRead = 0x00010000,
        AccelStructWrite = 0x00020000,
        AccelStructBuildInput = 0x00040000,
        AccelStructBuildBlas = 0x00080000,
        ShadingRateSurface = 0x00100000,
        OpacityMicromapWrite = 0x00200000,
        OpacityMicromapBuildInput = 0x00400000,
    };

    MAKEFLAG(F_ResourceStates)

    using MipLevel = uint32;
    using ArraySlice = uint32;

    // Flags for resources that need to be shared with other graphics APIs or other GPU devices.
    enum class F_SharedResourceFlags : uint32{
        None = 0,

        // D3D11: adds D3D11_RESOURCE_MISC_SHARED
        // D3D12: adds D3D12_HEAP_FLAG_SHARED
        // Vulkan: adds vk::ExternalMemoryImageCreateInfo and vk::ExportMemoryAllocateInfo/vk::ExternalMemoryBufferCreateInfo
        Shared = 0x01,

        // D3D11: adds (D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX | D3D11_RESOURCE_MISC_SHARED_NTHANDLE)
        // D3D12, Vulkan: ignored
        Shared_NTHandle = 0x02,

        // D3D12: adds D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER and D3D12_HEAP_FLAG_SHARED_CROSS_ADAPTER
        // D3D11, Vulkan: ignored
        Shared_CrossAdapter = 0x04,
    };

    MAKEFLAG(F_SharedResourceFlags)

    struct TextureDesc{
        uint32 Width = 1;
        uint32 Height = 1;
        uint32 Depth = 1;
        uint32 ArraySize = 1;
        uint32 MipLevels = 1;
        uint32 SampleCount = 1;
        uint32 SampleQuality = 0;
        E_Format Format = E_Format::UNKNOWN;
        E_TextureDimension Dimension = E_TextureDimension::Texture2D;
        EFString DebugName;

        bool B_IsShaderResource = true; // Note: isShaderResource is initialized to 'true' for backward compatibility
        bool B_IsRenderTarget = false;
        bool B_IsUAV = false;
        bool B_IsTypeless = false;
        bool B_IsShadingRateSurface = false;

        F_SharedResourceFlags SharedResourceFlags = F_SharedResourceFlags::None;

        // Indicates that the texture is created with no backing memory,
        // and memory is bound to the texture later using bindTextureMemory.
        // On DX12, the texture resource is created at the time of memory binding.
        bool B_IsVirtual = false;
        bool B_IsTiled = false;

        Color ClearValue;
        bool B_UseClearValue = false;

        F_ResourceStates InitialState = F_ResourceStates::Unknown;

        // If keepInitialState is true, command lists that use the texture will automatically
        // begin tracking the texture from the initial state and transition it to the initial state
        // on command list close.
        bool B_KeepInitialState = false;

        constexpr TextureDesc& SetWidth(const uint32 value){
            Width = value;
            return *this;
        }

        constexpr TextureDesc& SetHeight(const uint32 value){
            Height = value;
            return *this;
        }

        constexpr TextureDesc& SetDepth(const uint32 value){
            Depth = value;
            return *this;
        }

        constexpr TextureDesc& SetArraySize(const uint32 value){
            ArraySize = value;
            return *this;
        }

        constexpr TextureDesc& SetMipLevels(const uint32 value){
            MipLevels = value;
            return *this;
        }

        constexpr TextureDesc& SetSampleCount(const uint32 value){
            SampleCount = value;
            return *this;
        }

        constexpr TextureDesc& SetSampleQuality(const uint32 value){
            SampleQuality = value;
            return *this;
        }

        constexpr TextureDesc& SetFormat(const E_Format value){
            Format = value;
            return *this;
        }

        constexpr TextureDesc& SetDimension(const E_TextureDimension value){
            Dimension = value;
            return *this;
        }

        TextureDesc& SetDebugName(const std::string_view& value){
            DebugName = value;
            return *this;
        }

        constexpr TextureDesc& SetIsRenderTarget(const bool value){
            B_IsRenderTarget = value;
            return *this;
        }

        constexpr TextureDesc& SetIsUAV(const bool value){
            B_IsUAV = value;
            return *this;
        }

        constexpr TextureDesc& SetIsTypeless(const bool value){
            B_IsTypeless = value;
            return *this;
        }

        constexpr TextureDesc& SetIsVirtual(const bool value){
            B_IsVirtual = value;
            return *this;
        }

        constexpr TextureDesc& SetClearValue(const Color& value){
            ClearValue = value;
            B_UseClearValue = true;
            return *this;
        }

        constexpr TextureDesc& SetUseClearValue(const bool value){
            B_UseClearValue = value;
            return *this;
        }

        constexpr TextureDesc& SetInitialState(const F_ResourceStates value){
            InitialState = value;
            return *this;
        }

        constexpr TextureDesc& SetKeepInitialState(const bool value){
            B_KeepInitialState = value;
            return *this;
        }

        constexpr TextureDesc& SetSharedResourceFlags(const F_SharedResourceFlags value){
            SharedResourceFlags = value;
            return *this;
        }
    };

    // Describes a 2D or 3D section of a single mip level, single array slice of a texture.
    struct TextureSlice{
        uint32 X = 0;
        uint32 Y = 0;
        uint32 Z = 0;
        // -1 means the entire dimension is part of the region
        // resolve() below will translate these values into actual dimensions
        uint32 Width = static_cast<uint32>(-1);
        uint32 Height = static_cast<uint32>(-1);
        uint32 Depth = static_cast<uint32>(-1);

        MipLevel TexMipLevel = 0;
        ArraySlice TexArraySlice = 0;

        [[nodiscard]] EFRENDERAPI_API TextureSlice Resolve(const TextureDesc& desc) const;

        constexpr TextureSlice& SetOrigin(const uint32 vx = 0, const uint32 vy = 0, const uint32 vz = 0){
            X = vx;
            Y = vy;
            Z = vz;
            return *this;
        }

        constexpr TextureSlice& SetWidth(const uint32 value){
            Width = value;
            return *this;
        }

        constexpr TextureSlice& SetHeight(const uint32 value){
            Height = value;
            return *this;
        }

        constexpr TextureSlice& SetDepth(const uint32 value){
            Depth = value;
            return *this;
        }

        constexpr TextureSlice& SetSize(const uint32 vx = static_cast<uint32>(-1),
                                        const uint32 vy = static_cast<uint32>(-1),
                                        const uint32 vz = static_cast<uint32>(-1)){
            Width = vx;
            Height = vy;
            Depth = vz;
            return *this;
        }

        constexpr TextureSlice& SetMipLevel(const MipLevel level){
            TexMipLevel = level;
            return *this;
        }

        constexpr TextureSlice& SetArraySlice(const ArraySlice slice){
            TexArraySlice = slice;
            return *this;
        }
    };

    struct TextureSubresourceSet{
        static constexpr MipLevel ALL_MIP_LEVELS = static_cast<MipLevel>(-1);
        static constexpr ArraySlice ALL_ARRAY_SLICES = static_cast<ArraySlice>(-1);

        MipLevel BaseMipLevel = 0;
        MipLevel NumMipLevels = 1;
        ArraySlice BaseArraySlice = 0;
        ArraySlice NumArraySlices = 1;

        TextureSubresourceSet() = default;

        TextureSubresourceSet(const MipLevel baseMipLevel, const MipLevel numMipLevels,
                              const ArraySlice baseArraySlice,
                              const ArraySlice numArraySlices)
            : BaseMipLevel(baseMipLevel)
              , NumMipLevels(numMipLevels)
              , BaseArraySlice(baseArraySlice)
              , NumArraySlices(numArraySlices){
        }

        [[nodiscard]] EFRENDERAPI_API TextureSubresourceSet Resolve(
            const TextureDesc& desc, bool singleMipLevel) const;

        [[nodiscard]] EFRENDERAPI_API bool IsEntireTexture(const TextureDesc& desc) const;

        bool operator ==(const TextureSubresourceSet& other) const = default;

        constexpr TextureSubresourceSet& SetBaseMipLevel(const MipLevel value){
            BaseMipLevel = value;
            return *this;
        }

        constexpr TextureSubresourceSet& SetNumMipLevels(const MipLevel value){
            NumMipLevels = value;
            return *this;
        }

        constexpr TextureSubresourceSet& SetMipLevels(const MipLevel base, const MipLevel num){
            BaseMipLevel = base;
            NumMipLevels = num;
            return *this;
        }

        constexpr TextureSubresourceSet& SetBaseArraySlice(const ArraySlice value){
            BaseArraySlice = value;
            return *this;
        }

        constexpr TextureSubresourceSet& SetNumArraySlices(const ArraySlice value){
            NumArraySlices = value;
            return *this;
        }

        constexpr TextureSubresourceSet& SetArraySlices(const ArraySlice base, const ArraySlice num){
            BaseArraySlice = base;
            NumArraySlices = num;
            return *this;
        }

    };

    static const auto AllSubresources = TextureSubresourceSet(
        0, TextureSubresourceSet::ALL_MIP_LEVELS, 0, TextureSubresourceSet::ALL_ARRAY_SLICES);

    class EFRAPITexture : public EFRAPIResource{
    public:
        [[nodiscard]] virtual const TextureDesc& GetDesc() const = 0;

        virtual EFRAPIObject GetNativeView(ObjectType objectType, E_Format format/* = E_Format::UNKNOWN*/,
                                           TextureSubresourceSet subresources/* = AllSubresources*/,
                                           E_TextureDimension dimension/* = E_TextureDimension::Unknown*/,
                                           bool isReadOnlyDSV/* = false*/) = 0;
    };

    using TextureHandle = RefCountPtr<EFRAPITexture>;

    class EFRAPIStagingTexture : public EFRAPIResource{
    public:
        [[nodiscard]] virtual const TextureDesc& GetDesc() const = 0;
    };

    using StagingTextureHandle = RefCountPtr<EFRAPIStagingTexture>;

    struct TiledTextureCoordinate{
        uint16 MipLevel = 0;
        uint16 ArrayLevel = 0;
        uint32 X = 0;
        uint32 Y = 0;
        uint32 Z = 0;
    };

    struct TiledTextureRegion{
        uint32 TilesNum = 0;
        uint32 Width = 0;
        uint32 Height = 0;
        uint32 Depth = 0;
    };

    struct TextureTilesMapping{
        TiledTextureCoordinate* TiledTextureCoordinates = nullptr;
        TiledTextureRegion* TiledTextureRegions = nullptr;
        uint64* ByteOffsets = nullptr;
        uint32 NumTextureRegions = 0;
        IHeap* Heap = nullptr;
    };

    struct PackedMipDesc{
        uint32 NumStandardMips = 0;
        uint32 NumPackedMips = 0;
        uint32 NumTilesForPackedMips = 0;
        uint32 StartTileIndexInOverallResource = 0;
    };

    struct TileShape{
        uint32 WidthInTexels = 0;
        uint32 HeightInTexels = 0;
        uint32 DepthInTexels = 0;
    };

    struct SubresourceTiling{
        uint32 WidthInTiles = 0;
        uint32 HeightInTiles = 0;
        uint32 DepthInTiles = 0;
        uint32 StartTileIndexInOverallResource = 0;
    };

    enum class E_SamplerFeedbackFormat : uint8{
        MinMipOpaque = 0x0,
        MipRegionUsedOpaque = 0x1,
    };

    struct SamplerFeedbackTextureDesc{
        E_SamplerFeedbackFormat SamplerFeedbackFormat = E_SamplerFeedbackFormat::MinMipOpaque;
        uint32 SamplerFeedbackMipRegionX = 0;
        uint32 SamplerFeedbackMipRegionY = 0;
        uint32 SamplerFeedbackMipRegionZ = 0;
        F_ResourceStates InitialState = F_ResourceStates::Unknown;
        bool B_KeepInitialState = false;
    };

    class EFRAPISamplerFeedbackTexture : public EFRAPIResource{
    public:
        [[nodiscard]] virtual const SamplerFeedbackTextureDesc& GetDesc() const = 0;

        virtual TextureHandle GetPairedTexture() = 0;
    };

    using SamplerFeedbackTextureHandle = RefCountPtr<EFRAPISamplerFeedbackTexture>;

    //////////////////////////////////////////////////////////////////////////
    // Input Layout
    //////////////////////////////////////////////////////////////////////////

    struct VertexAttributeDesc{
        EFString Name;
        E_Format Format = E_Format::UNKNOWN;
        uint32 ArraySize = 1;
        uint32 BufferIndex = 0;
        uint32 Offset = 0;
        uint32 ElementStride = 0;
        bool B_IsInstanced = false;

        VertexAttributeDesc& SetName(const std::string_view& value){
            Name = value;
            return *this;
        }

        constexpr VertexAttributeDesc& SetFormat(const E_Format value){
            Format = value;
            return *this;
        }

        constexpr VertexAttributeDesc& SetArraySize(const uint32 value){
            ArraySize = value;
            return *this;
        }

        constexpr VertexAttributeDesc& SetBufferIndex(const uint32 value){
            BufferIndex = value;
            return *this;
        }

        constexpr VertexAttributeDesc& SetOffset(const uint32_t value){
            Offset = value;
            return *this;
        }

        constexpr VertexAttributeDesc& SetElementStride(const uint32 value){
            ElementStride = value;
            return *this;
        }

        constexpr VertexAttributeDesc& SetIsInstanced(const bool value){
            B_IsInstanced = value;
            return *this;
        }
    };

    class EFRAPIInputLayout : public EFRAPIResource{
    public:
        [[nodiscard]] virtual uint32 GetNumAttributes() const = 0;

        [[nodiscard]] virtual const VertexAttributeDesc* GetAttributeDesc(uint32 index) const = 0;
    };

    using InputLayoutHandle = RefCountPtr<EFRAPIInputLayout>;

    //////////////////////////////////////////////////////////////////////////
    // Sampler
    //////////////////////////////////////////////////////////////////////////

    enum class E_SamplerAddressMode : uint8{
        // D3D names
        Clamp,
        Wrap,
        Border,
        Mirror,
        MirrorOnce,

        // Vulkan names
        ClampToEdge = Clamp,
        Repeat = Wrap,
        ClampToBorder = Border,
        MirroredRepeat = Mirror,
        MirrorClampToEdge = MirrorOnce
    };

    enum class E_SamplerReductionType : uint8{
        Standard,
        Comparison,
        Minimum,
        Maximum
    };

    struct SamplerDesc{
        Color BorderColor = 1.f;
        float MaxAnisotropy = 1.f;
        float MipBias = 0.f;

        bool MinFilter = true;
        bool MagFilter = true;
        bool MipFilter = true;
        E_SamplerAddressMode AddressU = E_SamplerAddressMode::Clamp;
        E_SamplerAddressMode AddressV = E_SamplerAddressMode::Clamp;
        E_SamplerAddressMode AddressW = E_SamplerAddressMode::Clamp;
        E_SamplerReductionType ReductionType = E_SamplerReductionType::Standard;

        SamplerDesc& SetBorderColor(const Color& color){
            BorderColor = color;
            return *this;
        }

        SamplerDesc& SetMaxAnisotropy(const float value){
            MaxAnisotropy = value;
            return *this;
        }

        SamplerDesc& SetMipBias(const float value){
            MipBias = value;
            return *this;
        }

        SamplerDesc& SetMinFilter(const bool enable){
            MinFilter = enable;
            return *this;
        }

        SamplerDesc& SetMagFilter(const bool enable){
            MagFilter = enable;
            return *this;
        }

        SamplerDesc& SetMipFilter(const bool enable){
            MipFilter = enable;
            return *this;
        }

        SamplerDesc& SetAllFilters(const bool enable){
            MinFilter = MagFilter = MipFilter = enable;
            return *this;
        }

        SamplerDesc& SetAddressU(const E_SamplerAddressMode mode){
            AddressU = mode;
            return *this;
        }

        SamplerDesc& SetAddressV(const E_SamplerAddressMode mode){
            AddressV = mode;
            return *this;
        }

        SamplerDesc& SetAddressW(const E_SamplerAddressMode mode){
            AddressW = mode;
            return *this;
        }

        SamplerDesc& SetAllAddressModes(const E_SamplerAddressMode mode){
            AddressU = AddressV = AddressW = mode;
            return *this;
        }

        SamplerDesc& SetReductionType(const E_SamplerReductionType type){
            ReductionType = type;
            return *this;
        }
    };

    class EFRAPISampler : public EFRAPIResource{
    public:
        [[nodiscard]] virtual const SamplerDesc& GetDesc() const = 0;
    };

    using SamplerHandle = RefCountPtr<EFRAPISampler>;
}

template <>
struct std::hash<EventfulEngine::TextureSubresourceSet>{
    std::size_t operator()(EventfulEngine::TextureSubresourceSet const& s) const noexcept{
        size_t hash = 0;
        EventfulEngine::hash_combine(hash, s.BaseMipLevel);
        EventfulEngine::hash_combine(hash, s.NumMipLevels);
        EventfulEngine::hash_combine(hash, s.BaseArraySlice);
        EventfulEngine::hash_combine(hash, s.NumArraySlices);
        return hash;
    }
};
