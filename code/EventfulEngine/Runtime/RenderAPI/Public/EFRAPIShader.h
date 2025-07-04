#pragma once

#include "EFRAPIResource.h"

namespace EventfulEngine{

    enum class F_ShaderType : uint16{
        None = 0x0000,

        Compute = 0x0020,

        Vertex = 0x0001,
        Hull = 0x0002,
        Domain = 0x0004,
        Geometry = 0x0008,
        Pixel = 0x0010,
        Amplification = 0x0040,
        Mesh = 0x0080,
        AllGraphics = 0x00DF,

        RayGeneration = 0x0100,
        AnyHit = 0x0200,
        ClosestHit = 0x0400,
        Miss = 0x0800,
        Intersection = 0x1000,
        Callable = 0x2000,
        AllRayTracing = 0x3F00,

        All = 0x3FFF,
    };

    MAKEFLAG(F_ShaderType)

    enum class F_FastGeometryShaderFlags : uint8{
        ForceFastGS = 0x01,
        UseViewportMask = 0x02,
        OffsetTargetIndexByViewportIndex = 0x04,
        StrictApiOrder = 0x08
    };

    MAKEFLAG(F_FastGeometryShaderFlags)

    struct CustomSemantic{
        enum class E_Type{
            Undefined = 0,
            XRight = 1,
            ViewportMask = 2
        };

        E_Type type;
        std::string name;

        constexpr CustomSemantic& SetType(const E_Type value){
            type = value;
            return *this;
        }

        CustomSemantic& SetName(const std::string_view& value){
            name = value;
            return *this;
        }
    };

    struct ShaderDesc{
        F_ShaderType ShaderType = F_ShaderType::None;
        std::string DebugName;
        std::string EntryName = "main";

        int HlslExtensionsUAV = -1;

        bool B_UseSpecificShaderExt = false;
        uint32 NumCustomSemantics = 0;
        CustomSemantic* PCustomSemantics = nullptr;

        F_FastGeometryShaderFlags FastGSFlags = static_cast<F_FastGeometryShaderFlags>(0);
        uint32* PCoordinateSwizzling = nullptr;

        constexpr ShaderDesc& SetShaderType(const F_ShaderType value){
            ShaderType = value;
            return *this;
        }

        ShaderDesc& SetDebugName(const std::string_view& value){
            DebugName = value;
            return *this;
        }

        ShaderDesc& SetEntryName(const std::string_view& value){
            EntryName = value;
            return *this;
        }

        constexpr ShaderDesc& SetHlslExtensionsUAV(const int& value){
            HlslExtensionsUAV = value;
            return *this;
        }

        constexpr ShaderDesc& SetUseSpecificShaderExt(const bool& value){
            B_UseSpecificShaderExt = value;
            return *this;
        }

        constexpr ShaderDesc& SetCustomSemantics(const uint32 count, CustomSemantic* data){
            NumCustomSemantics = count;
            PCustomSemantics = data;
            return *this;
        }

        constexpr ShaderDesc& SetFastGSFlags(const F_FastGeometryShaderFlags value){
            FastGSFlags = value;
            return *this;
        }

        constexpr ShaderDesc& SetCoordinateSwizzling(uint32* value){
            PCoordinateSwizzling = value;
            return *this;
        }
    };

    struct ShaderSpecialization{
        uint32 constantID = 0;

        union{
            uint32_t u = 0;
            int32_t i;
            float f;
        } value;

        static ShaderSpecialization UInt32(const uint32 constantID, const uint32 u){
            ShaderSpecialization s;
            s.constantID = constantID;
            s.value.u = u;
            return s;
        }

        static ShaderSpecialization Int32(const uint32 constantID, const int32 i){
            ShaderSpecialization s;
            s.constantID = constantID;
            s.value.i = i;
            return s;
        }

        static ShaderSpecialization Float(const uint32 constantID, const float f){
            ShaderSpecialization s;
            s.constantID = constantID;
            s.value.f = f;
            return s;
        }
    };

    class EFRAPIShader : public EFRAPIResource{
    public:
        [[nodiscard]] virtual const ShaderDesc& GetDesc() const = 0;

        virtual void GetBytecode(const void** ppBytecode, size_t* pSize) const = 0;
    };

    using ShaderHandle = RefCountPtr<EFRAPIShader>;

    class EFRAPIShaderLibrary : public EFRAPIResource{
    public:
        virtual void GetBytecode(const void** ppBytecode, size_t* pSize) const = 0;

        virtual ShaderHandle GetShader(const char* entryName, F_ShaderType shaderType) = 0;
    };

    using ShaderLibraryHandle = RefCountPtr<EFRAPIShaderLibrary>;

} // EventfulEngine
