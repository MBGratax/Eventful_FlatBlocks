#pragma once
#include <Platform.h>
#include <cmath>
#include "EFRenderAPIModuleAPI.h"
#include "EnumFlag.h"

namespace EventfulEngine{
    static constexpr uint32_t c_MaxRenderTargets = 8;
    static constexpr uint32_t c_MaxViewports = 16;
    static constexpr uint32_t c_MaxVertexAttributes = 16;
    static constexpr uint32_t c_MaxBindingLayouts = 8;
    static constexpr uint32_t c_MaxBindlessRegisterSpaces = 16;
    static constexpr uint32_t c_MaxVolatileConstantBuffersPerLayout = 6;
    static constexpr uint32_t c_MaxVolatileConstantBuffers = 32;
    static constexpr uint32_t c_MaxPushConstantSize = 128;
    // D3D12: root signature is 256 bytes max., Vulkan: 128 bytes of push constants guaranteed
    static constexpr uint32_t c_ConstantBufferOffsetSizeAlignment = 256;
    // Partially bound constant buffers must have offsets aligned to this and sizes multiple of this

    //TODO: Probably move somewhere else, these are pretty general types
    struct Color{
        float r{};
        float g{};
        float b{};
        float a{};

        Color() : r(0.f), g(0.f), b(0.f), a(0.f){
        }

        Color(float c) : r(c), g(c), b(c), a(c){
        }

        Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a){
        }

        bool operator ==(const Color& _b) const = default;
    };

    struct Viewport{
        float minX;
        float maxX;

        float minY;
        float maxY;

        float minZ;
        float maxZ;

        Viewport() : minX(0.f), maxX(0.f), minY(0.f), maxY(0.f), minZ(0.f), maxZ(1.f){
        }

        Viewport(float width, float height) : minX(0.f), maxX(width), minY(0.f), maxY(height), minZ(0.f), maxZ(1.f){
        }

        Viewport(float _minX, float _maxX, float _minY, float _maxY, float _minZ, float _maxZ)
            : minX(_minX), maxX(_maxX), minY(_minY), maxY(_maxY), minZ(_minZ), maxZ(_maxZ){
        }

        bool operator ==(const Viewport& b) const{
            return minX == b.minX
                && minY == b.minY
                && minZ == b.minZ
                && maxX == b.maxX
                && maxY == b.maxY
                && maxZ == b.maxZ;
        }

        bool operator !=(const Viewport& b) const{ return !(*this == b); }

        [[nodiscard]] float width() const{ return maxX - minX; }
        [[nodiscard]] float height() const{ return maxY - minY; }
    };

    struct Rect{
        int minX, maxX;
        int minY, maxY;

        Rect() : minX(0), maxX(0), minY(0), maxY(0){
        }

        Rect(int width, int height) : minX(0), maxX(width), minY(0), maxY(height){
        }

        Rect(int _minX, int _maxX, int _minY, int _maxY) : minX(_minX), maxX(_maxX), minY(_minY), maxY(_maxY){
        }

        explicit Rect(const Viewport& viewport)
            : minX(int(floorf(viewport.minX)))
              , maxX(int(ceilf(viewport.maxX)))
              , minY(int(floorf(viewport.minY)))
              , maxY(int(ceilf(viewport.maxY))){
        }

        bool operator ==(const Rect& b) const{
            return minX == b.minX && minY == b.minY && maxX == b.maxX && maxY == b.maxY;
        }

        bool operator !=(const Rect& b) const{ return !(*this == b); }

        [[nodiscard]] int width() const{ return maxX - minX; }
        [[nodiscard]] int height() const{ return maxY - minY; }
    };

    enum class E_RenderAPI{
        OpenGL,
        Headless
    };

    enum class E_Format : uint8{
        UNKNOWN,

        R8_UINT,
        R8_SINT,
        R8_UNORM,
        R8_SNORM,
        RG8_UINT,
        RG8_SINT,
        RG8_UNORM,
        RG8_SNORM,
        R16_UINT,
        R16_SINT,
        R16_UNORM,
        R16_SNORM,
        R16_FLOAT,
        BGRA4_UNORM,
        B5G6R5_UNORM,
        B5G5R5A1_UNORM,
        RGBA8_UINT,
        RGBA8_SINT,
        RGBA8_UNORM,
        RGBA8_SNORM,
        BGRA8_UNORM,
        SRGBA8_UNORM,
        SBGRA8_UNORM,
        R10G10B10A2_UNORM,
        R11G11B10_FLOAT,
        RG16_UINT,
        RG16_SINT,
        RG16_UNORM,
        RG16_SNORM,
        RG16_FLOAT,
        R32_UINT,
        R32_SINT,
        R32_FLOAT,
        RGBA16_UINT,
        RGBA16_SINT,
        RGBA16_FLOAT,
        RGBA16_UNORM,
        RGBA16_SNORM,
        RG32_UINT,
        RG32_SINT,
        RG32_FLOAT,
        RGB32_UINT,
        RGB32_SINT,
        RGB32_FLOAT,
        RGBA32_UINT,
        RGBA32_SINT,
        RGBA32_FLOAT,

        D16,
        D24S8,
        X24G8_UINT,
        D32,
        D32S8,
        X32G8_UINT,

        BC1_UNORM,
        BC1_UNORM_SRGB,
        BC2_UNORM,
        BC2_UNORM_SRGB,
        BC3_UNORM,
        BC3_UNORM_SRGB,
        BC4_UNORM,
        BC4_SNORM,
        BC5_UNORM,
        BC5_SNORM,
        BC6H_UFLOAT,
        BC6H_SFLOAT,
        BC7_UNORM,
        BC7_UNORM_SRGB,

        COUNT,
    };

    enum class FormatKind : uint8{
        Integer,
        Normalized,
        Float,
        DepthStencil
    };

    struct FormatInfo{
        E_Format format;
        const char* name;
        uint8 bytesPerBlock;
        uint8 blockSize;
        FormatKind kind;
        bool hasRed : 1;
        bool hasGreen : 1;
        bool hasBlue : 1;
        bool hasAlpha : 1;
        bool hasDepth : 1;
        bool hasStencil : 1;
        bool isSigned : 1;
        bool isSRGB : 1;
    };

    EFRENDERAPI_API const FormatInfo& GetFormatInfo(E_Format format);

    enum class F_FormatSupport : uint32{
        None = 0,

        Buffer = 0x00000001,
        IndexBuffer = 0x00000002,
        VertexBuffer = 0x00000004,

        Texture = 0x00000008,
        DepthStencil = 0x00000010,
        RenderTarget = 0x00000020,
        Blendable = 0x00000040,

        ShaderLoad = 0x00000080,
        ShaderSample = 0x00000100,
        ShaderUavLoad = 0x00000200,
        ShaderUavStore = 0x00000400,
        ShaderAtomic = 0x00000800,
    };

    MAKEFLAG(F_FormatSupport)
} // EventfulEngine
