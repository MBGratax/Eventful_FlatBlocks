#pragma once

#include "EFRAPIBindings.h"

namespace EventfulEngine{

    enum class E_BlendFactor : uint8{
        Zero = 1,
        One = 2,
        SrcColor = 3,
        InvSrcColor = 4,
        SrcAlpha = 5,
        InvSrcAlpha = 6,
        DstAlpha = 7,
        InvDstAlpha = 8,
        DstColor = 9,
        InvDstColor = 10,
        SrcAlphaSaturate = 11,
        ConstantColor = 14,
        InvConstantColor = 15,
        Src1Color = 16,
        InvSrc1Color = 17,
        Src1Alpha = 18,
        InvSrc1Alpha = 19,

        // Vulkan names
        OneMinusSrcColor = InvSrcColor,
        OneMinusSrcAlpha = InvSrcAlpha,
        OneMinusDstAlpha = InvDstAlpha,
        OneMinusDstColor = InvDstColor,
        OneMinusConstantColor = InvConstantColor,
        OneMinusSrc1Color = InvSrc1Color,
        OneMinusSrc1Alpha = InvSrc1Alpha,
    };

    enum class E_BlendOp : uint8{
        Add = 1,
        Subtract = 2,
        ReverseSubtract = 3,
        Min = 4,
        Max = 5
    };

    enum class F_ColorMask : uint8{
        // These values are equal to their counterparts in DX11, DX12, and Vulkan.
        Red = 1,
        Green = 2,
        Blue = 4,
        Alpha = 8,
        All = 0xF
    };

    MAKEFLAG(F_ColorMask)

    struct BlendState{
        struct RenderTarget{
            bool blendEnable = false;
            E_BlendFactor srcBlend = E_BlendFactor::One;
            E_BlendFactor destBlend = E_BlendFactor::Zero;
            E_BlendOp blendOp = E_BlendOp::Add;
            E_BlendFactor srcBlendAlpha = E_BlendFactor::One;
            E_BlendFactor destBlendAlpha = E_BlendFactor::Zero;
            E_BlendOp blendOpAlpha = E_BlendOp::Add;
            F_ColorMask colorWriteMask = F_ColorMask::All;

            constexpr RenderTarget& setBlendEnable(bool enable){
                blendEnable = enable;
                return *this;
            }

            constexpr RenderTarget& enableBlend(){
                blendEnable = true;
                return *this;
            }

            constexpr RenderTarget& disableBlend(){
                blendEnable = false;
                return *this;
            }

            constexpr RenderTarget& setSrcBlend(E_BlendFactor value){
                srcBlend = value;
                return *this;
            }

            constexpr RenderTarget& setDestBlend(E_BlendFactor value){
                destBlend = value;
                return *this;
            }

            constexpr RenderTarget& setBlendOp(E_BlendOp value){
                blendOp = value;
                return *this;
            }

            constexpr RenderTarget& setSrcBlendAlpha(E_BlendFactor value){
                srcBlendAlpha = value;
                return *this;
            }

            constexpr RenderTarget& setDestBlendAlpha(E_BlendFactor value){
                destBlendAlpha = value;
                return *this;
            }

            constexpr RenderTarget& setBlendOpAlpha(E_BlendOp value){
                blendOpAlpha = value;
                return *this;
            }

            constexpr RenderTarget& setColorWriteMask(F_ColorMask value){
                colorWriteMask = value;
                return *this;
            }

            [[nodiscard]] bool UsesConstantColor() const;

            constexpr bool operator ==(const RenderTarget& other) const{
                return blendEnable == other.blendEnable
                    && srcBlend == other.srcBlend
                    && destBlend == other.destBlend
                    && blendOp == other.blendOp
                    && srcBlendAlpha == other.srcBlendAlpha
                    && destBlendAlpha == other.destBlendAlpha
                    && blendOpAlpha == other.blendOpAlpha
                    && colorWriteMask == other.colorWriteMask;
            }

            constexpr bool operator !=(const RenderTarget& other) const{
                return !(*this == other);
            }
        };

        RenderTarget targets[c_MaxRenderTargets];
        bool alphaToCoverageEnable = false;

        constexpr BlendState& setRenderTarget(uint32_t index, const RenderTarget& target){
            targets[index] = target;
            return *this;
        }

        constexpr BlendState& setAlphaToCoverageEnable(bool enable){
            alphaToCoverageEnable = enable;
            return *this;
        }

        constexpr BlendState& enableAlphaToCoverage(){
            alphaToCoverageEnable = true;
            return *this;
        }

        constexpr BlendState& disableAlphaToCoverage(){
            alphaToCoverageEnable = false;
            return *this;
        }

        [[nodiscard]] bool UsesConstantColor(uint32_t numTargets) const;

        constexpr bool operator ==(const BlendState& other) const{
            if (alphaToCoverageEnable != other.alphaToCoverageEnable)
                return false;

            for (uint32_t i = 0; i < c_MaxRenderTargets; ++i){
                if (targets[i] != other.targets[i])
                    return false;
            }

            return true;
        }

        constexpr bool operator !=(const BlendState& other) const{
            return !(*this == other);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Raster State
    //////////////////////////////////////////////////////////////////////////

    enum class E_RasterFillMode : uint8{
        Solid,
        Wireframe,
    };

    enum class E_RasterCullMode : uint8{
        Back,
        Front,
        None
    };

    struct RasterState{
        E_RasterFillMode fillMode = E_RasterFillMode::Solid;
        E_RasterCullMode cullMode = E_RasterCullMode::Back;
        bool frontCounterClockwise = false;
        bool depthClipEnable = false;
        bool scissorEnable = false;
        bool multisampleEnable = false;
        bool antialiasedLineEnable = false;
        int depthBias = 0;
        float depthBiasClamp = 0.f;
        float SlopeScaledDepthBias = 0.f;

        // Extended rasterizer state supported by Maxwell
        // In D3D11, use NvAPI_D3D11_CreateRasterizerState to create such rasterizer state.
        uint8_t ForcedSampleCount = 0;
        bool ProgrammableSamplePositionsEnable = false;
        bool ConservativeRasterEnable = false;
        bool QuadFillEnable = false;
        char SamplePositionsX[16]{};
        char SamplePositionsY[16]{};

        constexpr RasterState& setFillMode(E_RasterFillMode value){
            fillMode = value;
            return *this;
        }

        constexpr RasterState& setFillSolid(){
            fillMode = E_RasterFillMode::Solid;
            return *this;
        }

        constexpr RasterState& setFillWireframe(){
            fillMode = E_RasterFillMode::Wireframe;
            return *this;
        }

        constexpr RasterState& setCullMode(E_RasterCullMode value){
            cullMode = value;
            return *this;
        }

        constexpr RasterState& setCullBack(){
            cullMode = E_RasterCullMode::Back;
            return *this;
        }

        constexpr RasterState& setCullFront(){
            cullMode = E_RasterCullMode::Front;
            return *this;
        }

        constexpr RasterState& setCullNone(){
            cullMode = E_RasterCullMode::None;
            return *this;
        }

        constexpr RasterState& setFrontCounterClockwise(bool value){
            frontCounterClockwise = value;
            return *this;
        }

        constexpr RasterState& setDepthClipEnable(bool value){
            depthClipEnable = value;
            return *this;
        }

        constexpr RasterState& enableDepthClip(){
            depthClipEnable = true;
            return *this;
        }

        constexpr RasterState& disableDepthClip(){
            depthClipEnable = false;
            return *this;
        }

        constexpr RasterState& setScissorEnable(bool value){
            scissorEnable = value;
            return *this;
        }

        constexpr RasterState& enableScissor(){
            scissorEnable = true;
            return *this;
        }

        constexpr RasterState& disableScissor(){
            scissorEnable = false;
            return *this;
        }

        constexpr RasterState& setMultisampleEnable(bool value){
            multisampleEnable = value;
            return *this;
        }

        constexpr RasterState& enableMultisample(){
            multisampleEnable = true;
            return *this;
        }

        constexpr RasterState& disableMultisample(){
            multisampleEnable = false;
            return *this;
        }

        constexpr RasterState& setAntialiasedLineEnable(bool value){
            antialiasedLineEnable = value;
            return *this;
        }

        constexpr RasterState& enableAntialiasedLine(){
            antialiasedLineEnable = true;
            return *this;
        }

        constexpr RasterState& disableAntialiasedLine(){
            antialiasedLineEnable = false;
            return *this;
        }

        constexpr RasterState& setDepthBias(int value){
            depthBias = value;
            return *this;
        }

        constexpr RasterState& setDepthBiasClamp(float value){
            depthBiasClamp = value;
            return *this;
        }

        constexpr RasterState& SetSlopeScaleDepthBias(const float& value){
            SlopeScaledDepthBias = value;
            return *this;
        }

        constexpr RasterState& SetForcedSampleCount(const uint8& value){
            ForcedSampleCount = value;
            return *this;
        }

        constexpr RasterState& SetProgrammableSamplePositionsEnable(const bool& value){
            ProgrammableSamplePositionsEnable = value;
            return *this;
        }

        constexpr RasterState& EnableProgrammableSamplePositions(){
            ProgrammableSamplePositionsEnable = true;
            return *this;
        }

        constexpr RasterState& DisableProgrammableSamplePositions(){
            ProgrammableSamplePositionsEnable = false;
            return *this;
        }

        constexpr RasterState& SetConservativeRasterEnable(const bool& value){
            ConservativeRasterEnable = value;
            return *this;
        }

        constexpr RasterState& EnableConservativeRaster(){
            ConservativeRasterEnable = true;
            return *this;
        }

        constexpr RasterState& DisableConservativeRaster(){
            ConservativeRasterEnable = false;
            return *this;
        }

        constexpr RasterState& SetQuadFillEnable(const bool value){
            QuadFillEnable = value;
            return *this;
        }

        constexpr RasterState& EnableQuadFill(){
            QuadFillEnable = true;
            return *this;
        }

        constexpr RasterState& DisableQuadFill(){
            QuadFillEnable = false;
            return *this;
        }

        constexpr RasterState& SetSamplePositions(const char* x, const char* y, const int& count){
            for (int i = 0; i < count; i++){
                SamplePositionsX[i] = x[i];
                SamplePositionsY[i] = y[i];
            }
            return *this;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Depth Stencil State
    //////////////////////////////////////////////////////////////////////////

    enum class E_StencilOp : uint8{
        Keep = 1,
        Zero = 2,
        Replace = 3,
        IncrementAndClamp = 4,
        DecrementAndClamp = 5,
        Invert = 6,
        IncrementAndWrap = 7,
        DecrementAndWrap = 8
    };

    enum class E_ComparisonFunc : uint8{
        Never = 1,
        Less = 2,
        Equal = 3,
        LessOrEqual = 4,
        Greater = 5,
        NotEqual = 6,
        GreaterOrEqual = 7,
        Always = 8
    };

    struct DepthStencilState{
        struct StencilOpDesc{
            E_StencilOp FailOp = E_StencilOp::Keep;
            E_StencilOp DepthFailOp = E_StencilOp::Keep;
            E_StencilOp PassOp = E_StencilOp::Keep;
            E_ComparisonFunc StencilFunc = E_ComparisonFunc::Always;

            constexpr StencilOpDesc& SetFailOp(const E_StencilOp value){
                FailOp = value;
                return *this;
            }

            constexpr StencilOpDesc& SetDepthFailOp(const E_StencilOp value){
                DepthFailOp = value;
                return *this;
            }

            constexpr StencilOpDesc& SetPassOp(const E_StencilOp value){
                PassOp = value;
                return *this;
            }

            constexpr StencilOpDesc& SetStencilFunc(const E_ComparisonFunc value){
                StencilFunc = value;
                return *this;
            }
        };

        bool B_IsDepthTestEnabled = true;
        bool B_IsDepthWriteEnabled = true;
        E_ComparisonFunc DepthFunc = E_ComparisonFunc::Less;
        bool B_IsStencilEnabled = false;
        uint8_t StencilReadMask = 0xff;
        uint8_t StencilWriteMask = 0xff;
        uint8_t StencilRefValue = 0;
        bool B_IsDynamicStencilRef = false;
        StencilOpDesc FrontFaceStencil;
        StencilOpDesc BackFaceStencil;

        constexpr DepthStencilState& SetDepthTestEnable(const bool& value){
            B_IsDepthTestEnabled = value;
            return *this;
        }

        constexpr DepthStencilState& EnableDepthTest(){
            B_IsDepthTestEnabled = true;
            return *this;
        }

        constexpr DepthStencilState& DisableDepthTest(){
            B_IsDepthTestEnabled = false;
            return *this;
        }

        constexpr DepthStencilState& SetDepthWriteEnable(const bool& value){
            B_IsDepthWriteEnabled = value;
            return *this;
        }

        constexpr DepthStencilState& EnableDepthWrite(){
            B_IsDepthWriteEnabled = true;
            return *this;
        }

        constexpr DepthStencilState& DisableDepthWrite(){
            B_IsDepthWriteEnabled = false;
            return *this;
        }

        constexpr DepthStencilState& SetDepthFunc(const E_ComparisonFunc value){
            DepthFunc = value;
            return *this;
        }

        constexpr DepthStencilState& SetStencilEnable(const bool& value){
            B_IsStencilEnabled = value;
            return *this;
        }

        constexpr DepthStencilState& EnableStencil(){
            B_IsStencilEnabled = true;
            return *this;
        }

        constexpr DepthStencilState& DisableStencil(){
            B_IsStencilEnabled = false;
            return *this;
        }

        constexpr DepthStencilState& SetStencilReadMask(const uint8 value){
            StencilReadMask = value;
            return *this;
        }

        constexpr DepthStencilState& SetStencilWriteMask(const uint8 value){
            StencilWriteMask = value;
            return *this;
        }

        constexpr DepthStencilState& SetStencilRefValue(const uint8 value){
            StencilRefValue = value;
            return *this;
        }

        constexpr DepthStencilState& SetFrontFaceStencil(const StencilOpDesc& value){
            FrontFaceStencil = value;
            return *this;
        }

        constexpr DepthStencilState& SetBackFaceStencil(const StencilOpDesc& value){
            BackFaceStencil = value;
            return *this;
        }

        constexpr DepthStencilState& SetDynamicStencilRef(const bool& value){
            B_IsDynamicStencilRef = value;
            return *this;
        }

    };

    //////////////////////////////////////////////////////////////////////////
    // Viewport State
    //////////////////////////////////////////////////////////////////////////

    struct ViewportState{
        //These are in pixels
        // note: you can only set each of these either in the PSO or per draw call in DrawArguments
        // it is not legal to have the same state set in both the PSO and DrawArguments
        // leaving these vectors empty means no state is set
        EFStaticArray<Viewport, c_MaxViewports> Viewports;
        EFStaticArray<Rect, c_MaxViewports> ScissorRects;

        ViewportState& AddViewport(const Viewport& v){
            Viewports.push_back(v);
            return *this;
        }

        ViewportState& AddScissorRect(const Rect& r
        ){
            ScissorRects.push_back(r
            );
            return *this;
        }

        ViewportState& AddViewportAndScissorRect(const Viewport& v){
            return AddViewport(v).AddScissorRect(Rect(v));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Draw State
    //////////////////////////////////////////////////////////////////////////

    enum class E_PrimitiveType : uint8{
        PrimitiveType,
        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip,
        TriangleFan,
        TriangleListWithAdjacency,
        TriangleStripWithAdjacency,
        PatchList
    };

    struct SinglePassStereoState{
        bool B_IsEnabled = false;
        bool B_IsIndependentViewportMask = false;
        uint16_t RenderTargetIndexOffset = 0;

        bool operator ==(const SinglePassStereoState& b) const = default;

        constexpr SinglePassStereoState& SetEnabled(const bool value){
            B_IsEnabled = value;
            return *this;
        }

        constexpr SinglePassStereoState& SetIndependentViewportMask(const bool value){
            B_IsIndependentViewportMask = value;
            return *this;
        }

        constexpr SinglePassStereoState& SetRenderTargetIndexOffset(const uint16 value){
            RenderTargetIndexOffset = value;
            return *this;
        }
    };

    struct RenderState{
        BlendState RenderBlendState;
        DepthStencilState RenderDepthStencilState;
        RasterState RenderRasterState;
        SinglePassStereoState SinglePassStereo;

        constexpr RenderState& SetBlendState(const BlendState& value){
            RenderBlendState = value;
            return *this;
        }

        constexpr RenderState& SetDepthStencilState(const DepthStencilState& value){
            RenderDepthStencilState = value;
            return *this;
        }

        constexpr RenderState& SetRasterState(const RasterState& value){
            RenderRasterState = value;
            return *this;
        }

        constexpr RenderState& SetSinglePassStereoState(const SinglePassStereoState& value){
            SinglePassStereo = value;
            return *this;
        }
    };

    enum class E_VariableShadingRate : uint8{
        e1x1,
        e1x2,
        e2x1,
        e2x2,
        e2x4,
        e4x2,
        e4x4
    };

    enum class E_ShadingRateCombiner : uint8{
        Passthrough,
        Override,
        Min,
        Max,
        ApplyRelative
    };

    struct VariableRateShadingState{
        bool B_IsEnabled = false;
        E_VariableShadingRate ShadingRate = E_VariableShadingRate::e1x1;
        E_ShadingRateCombiner PipelinePrimitiveCombiner = E_ShadingRateCombiner::Passthrough;
        E_ShadingRateCombiner ImageCombiner = E_ShadingRateCombiner::Passthrough;

        bool operator ==(const VariableRateShadingState& b) const = default;

        constexpr VariableRateShadingState& SetEnabled(const bool value){
            B_IsEnabled = value;
            return *this;
        }

        constexpr VariableRateShadingState& SetShadingRate(const E_VariableShadingRate value){
            ShadingRate = value;
            return *this;
        }

        constexpr VariableRateShadingState& SetPipelinePrimitiveCombiner(const E_ShadingRateCombiner value){
            PipelinePrimitiveCombiner = value;
            return *this;
        }

        constexpr VariableRateShadingState& SetImageCombiner(const E_ShadingRateCombiner value){
            ImageCombiner = value;
            return *this;
        }
    };

    using BindingLayoutVector = EFStaticArray<BindingLayoutHandle, c_MaxBindingLayouts>;

    struct GraphicsPipelineDesc{
        E_PrimitiveType PrimType = E_PrimitiveType::TriangleList;
        uint32 PatchControlPoints = 0;
        InputLayoutHandle InputLayout;

        ShaderHandle VS;
        ShaderHandle HS;
        ShaderHandle DS;
        ShaderHandle GS;
        ShaderHandle PS;

        RenderState GraphicsRenderState;
        VariableRateShadingState ShadingRateState;

        BindingLayoutVector BindingLayouts;

        GraphicsPipelineDesc& SetPrimType(const E_PrimitiveType value){
            PrimType = value;
            return *this;
        }

        GraphicsPipelineDesc& SetPatchControlPoints(const uint32 value){
            PatchControlPoints = value;
            return *this;
        }

        GraphicsPipelineDesc& SetInputLayout(EFRAPIInputLayout* value){
            InputLayout = value;
            return *this;
        }

        GraphicsPipelineDesc& SetVertexShader(EFRAPIShader* value){
            VS = value;
            return *this;
        }

        GraphicsPipelineDesc& SetHullShader(EFRAPIShader* value){
            HS = value;
            return *this;
        }

        GraphicsPipelineDesc& SetTessellationControlShader(EFRAPIShader* value){
            HS = value;
            return *this;
        }

        GraphicsPipelineDesc& SetDomainShader(EFRAPIShader* value){
            DS = value;
            return *this;
        }

        GraphicsPipelineDesc& SetTessellationEvaluationShader(EFRAPIShader* value){
            DS = value;
            return *this;
        }

        GraphicsPipelineDesc& SetGeometryShader(EFRAPIShader* value){
            GS = value;
            return *this;
        }

        GraphicsPipelineDesc& SetPixelShader(EFRAPIShader* value){
            PS = value;
            return *this;
        }

        GraphicsPipelineDesc& SetFragmentShader(EFRAPIShader* value){
            PS = value;
            return *this;
        }

        GraphicsPipelineDesc& SetRenderState(const RenderState& value){
            GraphicsRenderState = value;
            return *this;
        }

        GraphicsPipelineDesc& SetVariableRateShadingState(const VariableRateShadingState& value){
            ShadingRateState = value;
            return *this;
        }

        GraphicsPipelineDesc& AddBindingLayout(EFRAPIBindingLayout* layout){
            BindingLayouts.push_back(layout);
            return *this;
        }
    };

    class EFRAPIGraphicsPipeline : public EFRAPIResource{
    public:
        [[nodiscard]] virtual const GraphicsPipelineDesc& GetDesc() const = 0;

        [[nodiscard]] virtual const FramebufferInfo& GetFramebufferInfo() const = 0;
    };

    using GraphicsPipelineHandle = RefCountPtr<EFRAPIGraphicsPipeline>;

    struct ComputePipelineDesc{
        ShaderHandle CS;

        BindingLayoutVector BindingLayouts;

        ComputePipelineDesc& SetComputeShader(EFRAPIShader* value){
            CS = value;
            return *this;
        }

        ComputePipelineDesc& AddBindingLayout(EFRAPIBindingLayout* layout){
            BindingLayouts.push_back(layout);
            return *this;
        }
    };

    class EFRAPIComputePipeline : public EFRAPIResource{
    public:
        [[nodiscard]] virtual const ComputePipelineDesc& GetDesc() const = 0;
    };

    using ComputePipelineHandle = RefCountPtr<EFRAPIComputePipeline>;

    struct MeshletPipelineDesc{
        E_PrimitiveType PrimType = E_PrimitiveType::TriangleList;

        ShaderHandle AS;
        ShaderHandle MS;
        ShaderHandle PS;

        RenderState MeshletRenderState;

        BindingLayoutVector BindingLayouts;

        MeshletPipelineDesc& SetPrimType(const E_PrimitiveType value){
            PrimType = value;
            return *this;
        }

        MeshletPipelineDesc& SetTaskShader(EFRAPIShader* value){
            AS = value;
            return *this;
        }

        MeshletPipelineDesc& SetAmplificationShader(EFRAPIShader* value){
            AS = value;
            return *this;
        }

        MeshletPipelineDesc& SetMeshShader(EFRAPIShader* value){
            MS = value;
            return *this;
        }

        MeshletPipelineDesc& SetPixelShader(EFRAPIShader* value){
            PS = value;
            return *this;
        }

        MeshletPipelineDesc& SetFragmentShader(EFRAPIShader* value){
            PS = value;
            return *this;
        }

        MeshletPipelineDesc& SetRenderState(const RenderState& value){
            MeshletRenderState = value;
            return *this;
        }

        MeshletPipelineDesc& AddBindingLayout(EFRAPIBindingLayout* layout){
            BindingLayouts.push_back(layout);
            return *this;
        }
    };

    class IMeshletPipeline : public EFRAPIResource{
    public:
        [[nodiscard]] virtual const MeshletPipelineDesc& GetDesc() const = 0;

        [[nodiscard]] virtual const FramebufferInfo& GetFramebufferInfo() const = 0;
    };

    using MeshletPipelineHandle = RefCountPtr<IMeshletPipeline>;
} // EventfulEngine
