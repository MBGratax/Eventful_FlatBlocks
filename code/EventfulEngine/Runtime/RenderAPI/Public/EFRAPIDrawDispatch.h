#pragma once

#include "EFRAPIPipelineState.h"


namespace EventfulEngine{

    //////////////////////////////////////////////////////////////////////////
    // Draw and Dispatch
    //////////////////////////////////////////////////////////////////////////

    class EFRAPIEventQuery : public EFRAPIResource{
    };

    typedef RefCountPtr<EFRAPIEventQuery> EventQueryHandle;

    class EFRAPITimerQuery : public EFRAPIResource{
    };

    typedef RefCountPtr<EFRAPITimerQuery> TimerQueryHandle;

    struct VertexBufferBinding{
        EFRAPIBuffer* Buffer = nullptr;
        uint32_t Slot;
        uint64_t Offset;

        bool operator ==(const VertexBufferBinding& b) const = default;

        VertexBufferBinding& SetBuffer(EFRAPIBuffer* value){
            Buffer = value;
            return *this;
        }

        VertexBufferBinding& SetSlot(const uint32 value){
            Slot = value;
            return *this;
        }

        VertexBufferBinding& SetOffset(const uint64 value){
            Offset = value;
            return *this;
        }
    };

    struct IndexBufferBinding{
        EFRAPIBuffer* Buffer = nullptr;
        E_Format Format;
        uint32 Offset;

        bool operator ==(const IndexBufferBinding& b) const = default;

        IndexBufferBinding& SetBuffer(EFRAPIBuffer* value){
            Buffer = value;
            return *this;
        }

        IndexBufferBinding& SetFormat(const E_Format value){
            Format = value;
            return *this;
        }

        IndexBufferBinding& SetOffset(const uint32 value){
            Offset = value;
            return *this;
        }
    };

    typedef EFStaticArray<EFRAPIBindingSet*, c_MaxBindingLayouts> BindingSetVector;


    struct GraphicsState{
        EFRAPIGraphicsPipeline* Pipeline = nullptr;
        EFRAPIFramebuffer* Framebuffer = nullptr;
        ViewportState Viewport;
        VariableRateShadingState ShadingRateState;
        Color BlendConstantColor{};
        uint8 DynamicStencilRefValue = 0;

        BindingSetVector Bindings;

        EFStaticArray<VertexBufferBinding, c_MaxVertexAttributes> VertexBuffers;
        IndexBufferBinding IndexBuffer;

        EFRAPIBuffer* IndirectParams = nullptr;

        GraphicsState& SetPipeline(EFRAPIGraphicsPipeline* value){
            Pipeline = value;
            return *this;
        }

        GraphicsState& SetFramebuffer(EFRAPIFramebuffer* value){
            Framebuffer = value;
            return *this;
        }

        GraphicsState& SetViewport(const ViewportState& value){
            Viewport = value;
            return *this;
        }

        GraphicsState& SetShadingRateState(const VariableRateShadingState& value){
            ShadingRateState = value;
            return *this;
        }

        GraphicsState& SetBlendColor(const Color& value){
            BlendConstantColor = value;
            return *this;
        }

        GraphicsState& SetDynamicStencilRefValue(const uint8 value){
            DynamicStencilRefValue = value;
            return *this;
        }

        GraphicsState& AddBindingSet(EFRAPIBindingSet* value){
            Bindings.push_back(value);
            return *this;
        }

        GraphicsState& AddVertexBuffer(const VertexBufferBinding& value){
            VertexBuffers.push_back(value);
            return *this;
        }

        GraphicsState& SetIndexBuffer(const IndexBufferBinding& value){
            IndexBuffer = value;
            return *this;
        }

        GraphicsState& SetIndirectParams(EFRAPIBuffer* value){
            IndirectParams = value;
            return *this;
        }
    };

    struct DrawArguments{
        uint32 VertexCount = 0;
        uint32 InstanceCount = 1;
        uint32 StartIndexLocation = 0;
        uint32 StartVertexLocation = 0;
        uint32 StartInstanceLocation = 0;

        constexpr DrawArguments& setVertexCount(uint32_t value){
            VertexCount = value;
            return *this;
        }

        constexpr DrawArguments& setInstanceCount(uint32_t value){
            InstanceCount = value;
            return *this;
        }

        constexpr DrawArguments& setStartIndexLocation(uint32_t value){
            StartIndexLocation = value;
            return *this;
        }

        constexpr DrawArguments& setStartVertexLocation(uint32_t value){
            StartVertexLocation = value;
            return *this;
        }

        constexpr DrawArguments& setStartInstanceLocation(uint32_t value){
            StartInstanceLocation = value;
            return *this;
        }
    };

    struct DrawIndirectArguments{
        uint32_t vertexCount = 0;
        uint32_t instanceCount = 1;
        uint32_t startVertexLocation = 0;
        uint32_t startInstanceLocation = 0;

        constexpr DrawIndirectArguments& setVertexCount(uint32_t value){
            vertexCount = value;
            return *this;
        }

        constexpr DrawIndirectArguments& setInstanceCount(uint32_t value){
            instanceCount = value;
            return *this;
        }

        constexpr DrawIndirectArguments& setStartVertexLocation(uint32_t value){
            startVertexLocation = value;
            return *this;
        }

        constexpr DrawIndirectArguments& setStartInstanceLocation(uint32_t value){
            startInstanceLocation = value;
            return *this;
        }
    };

    struct DrawIndexedIndirectArguments{
        uint32_t indexCount = 0;
        uint32_t instanceCount = 1;
        uint32_t startIndexLocation = 0;
        int32_t baseVertexLocation = 0;
        uint32_t startInstanceLocation = 0;

        constexpr DrawIndexedIndirectArguments& setIndexCount(uint32_t value){
            indexCount = value;
            return *this;
        }

        constexpr DrawIndexedIndirectArguments& setInstanceCount(uint32_t value){
            instanceCount = value;
            return *this;
        }

        constexpr DrawIndexedIndirectArguments& setStartIndexLocation(uint32_t value){
            startIndexLocation = value;
            return *this;
        }

        constexpr DrawIndexedIndirectArguments& setBaseVertexLocation(int32_t value){
            baseVertexLocation = value;
            return *this;
        }

        constexpr DrawIndexedIndirectArguments& setStartInstanceLocation(uint32_t value){
            startInstanceLocation = value;
            return *this;
        }
    };

    struct ComputeState{
        EFRAPIComputePipeline* pipeline = nullptr;

        BindingSetVector bindings;

        EFRAPIBuffer* indirectParams = nullptr;

        ComputeState& setPipeline(EFRAPIComputePipeline* value){
            pipeline = value;
            return *this;
        }

        ComputeState& addBindingSet(EFRAPIBindingSet* value){
            bindings.push_back(value);
            return *this;
        }

        ComputeState& setIndirectParams(EFRAPIBuffer* value){
            indirectParams = value;
            return *this;
        }
    };

    struct DispatchIndirectArguments{
        uint32_t groupsX = 1;
        uint32_t groupsY = 1;
        uint32_t groupsZ = 1;

        constexpr DispatchIndirectArguments& setGroupsX(uint32_t value){
            groupsX = value;
            return *this;
        }

        constexpr DispatchIndirectArguments& setGroupsY(uint32_t value){
            groupsY = value;
            return *this;
        }

        constexpr DispatchIndirectArguments& setGroupsZ(uint32_t value){
            groupsZ = value;
            return *this;
        }

        constexpr DispatchIndirectArguments& setGroups2D(uint32_t x, uint32_t y){
            groupsX = x;
            groupsY = y;
            return *this;
        }

        constexpr DispatchIndirectArguments& setGroups3D(uint32_t x, uint32_t y, uint32_t z){
            groupsX = x;
            groupsY = y;
            groupsZ = z;
            return *this;
        }
    };

    struct MeshletState{
        IMeshletPipeline* pipeline = nullptr;
        EFRAPIFramebuffer* framebuffer = nullptr;
        ViewportState viewport;
        Color blendConstantColor{};
        uint8_t dynamicStencilRefValue = 0;

        BindingSetVector bindings;

        EFRAPIBuffer* indirectParams = nullptr;

        MeshletState& setPipeline(IMeshletPipeline* value){
            pipeline = value;
            return *this;
        }

        MeshletState& setFramebuffer(EFRAPIFramebuffer* value){
            framebuffer = value;
            return *this;
        }

        MeshletState& setViewport(const ViewportState& value){
            viewport = value;
            return *this;
        }

        MeshletState& setBlendColor(const Color& value){
            blendConstantColor = value;
            return *this;
        }

        MeshletState& addBindingSet(EFRAPIBindingSet* value){
            bindings.push_back(value);
            return *this;
        }

        MeshletState& setIndirectParams(EFRAPIBuffer* value){
            indirectParams = value;
            return *this;
        }

        MeshletState& setDynamicStencilRefValue(uint8_t value){
            dynamicStencilRefValue = value;
            return *this;
        }
    };

} // EventfulEngine
