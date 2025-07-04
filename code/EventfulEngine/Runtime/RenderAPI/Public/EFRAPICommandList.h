#pragma once

#include "EFRAPIDrawDispatch.h"

namespace EventfulEngine{
    //////////////////////////////////////////////////////////////////////////
    // ICommandList
    //////////////////////////////////////////////////////////////////////////

    // Represents a sequence of GPU operations.
    // - DX11: All command list objects map to the single immediate context. Only one command list may be in the open
    //   state at any given time, and all command lists must have CommandListParameters::enableImmediateExecution = true.
    // - DX12: One command list object may contain multiple instances of ID3D12GraphicsCommandList* and
    //   ID3D12CommandAllocator objects, reusing older ones as they finish executing on the GPU. A command list object
    //   also contains the upload manager (for suballocating memory from the upload heap on operations such as
    //   writeBuffer) and the DXR scratch manager (for suballocating memory for acceleration structure builds).
    //   The upload and scratch managers' memory is reused when possible, but it is only freed when the command list
    //   object is destroyed. Thus, it might be a good idea to use a dedicated NVRHI command list for uploading large
    //   amounts of data and to destroy it when uploading is finished.
    // - Vulkan: The command list objects don't own the VkCommandBuffer-s but request available ones from the queue
    //   instead. The upload and scratch buffers behave the same way they do on DX12.
    class EFRAPICommandList : public EFRAPIResource{
    public:
        // Prepares the command list for recording a new sequence of commands.
        // All other methods of ICommandList must only be used when the command list is open.
        // - DX11: The immediate command list may always stay in the open state, although that prohibits other
        //   command lists from opening.
        // - DX12, Vulkan: Creates or reuses the command list or buffer object and the command allocator (DX12),
        //   starts tracking the resources being referenced in the command list.
        virtual void Open() = 0;

        // Finalizes the command list and prepares it for execution.
        // Use IDevice::executeCommandLists(...) to execute it.
        // Re-opening the command list without execution is allowed but not well-tested.
        virtual void Close() = 0;

        // Resets the NVRHI state cache associated with the command list, clears some of the underlying API state.
        // This method is mostly useful when switching from recording commands to the open command list using
        // non-NVRHI code - see getNativeObject(...) - to recording further commands using NVRHI.
        virtual void ClearState() = 0;

        // Clears some or all subresources of the given color texture using the provided color.
        // - DX11/12: The clear operation uses either an RTV or a UAV, depending on the texture usage flags
        //   (isRenderTarget and isUAV).
        // - Vulkan: vkCmdClearColorImage is always used with the Float32 color fields set.
        // At least one of the 'isRenderTarget' and 'isUAV' flags must be set, and the format of the texture
        // must be of a color type.
        virtual void ClearTextureFloat(EFRAPITexture* t, TextureSubresourceSet subresources,
                                       const Color& clearColor) = 0;

        // Clears some or all subresources of the given depth-stencil texture using the provided depth and/or stencil
        // values. The texture must have the isRenderTarget flag set, and its format must be of a depth-stencil type.
        virtual void ClearDepthStencilTexture(EFRAPITexture* t, TextureSubresourceSet subresources, bool clearDepth,
                                              float depth, bool clearStencil, uint8_t stencil) = 0;

        // Clears some or all subresources of the given color texture using the provided integer value.
        // - DX11/12: If the texture has the isUAV flag set, the clear is performed using ClearUnorderedAccessViewUint.
        //   Otherwise, the clear value is converted to a float, and the texture is cleared as an RTV with all 4
        //   color components using the same value.
        // - Vulkan: vkCmdClearColorImage is always used with the UInt32 and Int32 color fields set.
        virtual void ClearTextureUInt(EFRAPITexture* t, TextureSubresourceSet subresources, uint32_t clearColor) = 0;

        // Copies a single 2D or 3D region of texture data from texture 'src' into texture 'dst'.
        // The region's dimensions must be compatible between the two textures, meaning that for simple color textures
        // they must be equal, and for reinterpret copies between compressed and uncompressed textures, they must differ
        // by a factor equal to the block size. The function does not resize textures, only 1:1 pixel copies are
        // supported.
        virtual void CopyTexture(EFRAPITexture* dest, const TextureSlice& destSlice, EFRAPITexture* src,
                                 const TextureSlice& srcSlice) = 0;

        // Copies a single 2D or 3D region of texture data from regular texture 'src' into staging texture 'dst'.
        virtual void CopyTexture(EFRAPIStagingTexture* dest, const TextureSlice& destSlice, EFRAPITexture* src,
                                 const TextureSlice& srcSlice) = 0;

        // Copies a single 2D or 3D region of texture data from staging texture 'src' into regular texture 'dst'.
        virtual void CopyTexture(EFRAPITexture* dest, const TextureSlice& destSlice, EFRAPIStagingTexture* src,
                                 const TextureSlice& srcSlice) = 0;

        // Uploads the contents of an entire 2D or 3D mip level of a single array slice of the texture from CPU memory.
        // The data in CPU memory must be in the same pixel format as the texture. Pixels in every row must be tightly
        // packed, rows are packed with a stride of 'rowPitch' which must not be 0 unless the texture has a height of 1,
        // and depth slices are packed with a stride of 'depthPitch' which also must not be 0 if the texture is 3D.
        // - DX11: Maps directly to UpdateSubresource.
        // - DX12, Vulkan: A region of the automatic upload buffer is suballocated, data is copied there, and then
        //   copied on the GPU into the destination texture using CopyTextureRegion (DX12) or vkCmdCopyBufferToImage (VK).
        //   The upload buffer region can only be reused when this command list instance finishes executing on the GPU.
        // For more advanced uploading operations, such as updating only a region in the texture, use staging texture
        // objects and copyTexture(...).
        virtual void WriteTexture(EFRAPITexture* dest, uint32_t arraySlice, uint32_t mipLevel, const void* data,
                                  size_t rowPitch, size_t depthPitch = 0) = 0;

        // Performs a resolve operation to combine samples from some or all subresources of a multisample texture 'src'
        // into matching subresources of a non-multisample texture 'dest'. Both textures' formats must be of color type.
        // - DX11/12: Maps to a sequence of ResolveSubresource calls, one per subresource.
        // - Vulkan: Maps to a single vkCmdResolveImage call.
        virtual void ResolveTexture(EFRAPITexture* dest, const TextureSubresourceSet& dstSubresources,
                                    EFRAPITexture* src,
                                    const TextureSubresourceSet& srcSubresources) = 0;

        // Uploads 'dataSize' bytes of data from CPU memory into the GPU buffer 'b' at offset 'destOffsetBytes'.
        // - DX11: If the buffer's 'cpuAccess' mode is set to Write, maps the buffer and uploads the data that way.
        //   Otherwise, uses UpdateSubresource.
        // - DX12: If the buffer's 'isVolatile' flag is set, a region of the automatic upload buffer is suballocated,
        //   and the data is copied there. Subsequent uses of the buffer will directly refer to that location in the
        //   upload buffer, until the next call to writeBuffer(...) or until the command list is closed. A volatile
        //   buffer can not be used until writeBuffer(...) is called on it every time after the command list is opened.
        //   If the 'isVolatile' flag is not set, a region of the automatic upload buffer is suballocated, the data
        //   is copied there, and then copied into the real GPU buffer object using CopyBufferRegion.
        // - Vulkan: Similar behavior to DX12, except that each volatile buffer actually has its own Vulkan resource.
        //   The size of such resource is determined by the 'maxVersions' field of the BufferDesc. When writeBuffer(...)
        //   is called on a volatile buffer, a region of that buffer object (a single version) is suballocated, data
        //   is copied there, and subsequent uses of the buffer in the same command list will refer to that version.
        //   For non-volatile buffers, writes of 64 kB or smaller use vkCmdUpdateBuffer. Larger writes suballocate
        //   a portion of the automatic upload buffer and copy the data to the real GPU buffer through that and
        //   vkCmdCopyBuffer.
        virtual void WriteBuffer(EFRAPIBuffer* b, const void* data, size_t dataSize, uint64_t destOffsetBytes = 0) = 0;

        // Fills the entire buffer using the provided uint32 value.
        // - DX11/12: Maps to ClearUnorderedAccessViewUint.
        // - Vulkan: Maps to vkCmdFillBuffer.
        virtual void ClearBufferUInt(EFRAPIBuffer* b, uint32_t clearValue) = 0;

        // Copies 'dataSizeBytes' of data from buffer 'src' at offset 'srcOffsetBytes' into buffer 'dest' at offset
        // 'destOffsetBytes'. The source and destination regions must be within the sizes of the respective buffers.
        // - DX11: Maps to CopySubresourceRegion.
        // - DX12: Maps to CopyBufferRegion.
        // - Vulkan: Maps to vkCmdCopyBuffer.
        virtual void CopyBuffer(EFRAPIBuffer* dest, uint64_t destOffsetBytes, EFRAPIBuffer* src,
                                uint64_t srcOffsetBytes,
                                uint64_t dataSizeBytes) = 0;

        // Clears the entire sampler feedback texture.
        // - DX12: Maps to ClearUnorderedAccessViewUint.
        // - DX11, Vulkan: Unsupported.
        virtual void ClearSamplerFeedbackTexture(EFRAPISamplerFeedbackTexture* texture) = 0;

        // Decodes the sampler feedback texture into an application-usable format, storing data into the provided buffer.
        // The 'format' parameter should be Format::R8_UINT.
        // - DX12: Maps to ResolveSubresourceRegion.
        //   See https://microsoft.github.io/DirectX-Specs/d3d/SamplerFeedback.html
        // - DX11, Vulkan: Unsupported.
        virtual void DecodeSamplerFeedbackTexture(EFRAPIBuffer* buffer, EFRAPISamplerFeedbackTexture* texture,
                                                  E_Format format) = 0;

        // Transitions the sampler feedback texture into the requested state, placing a barrier if necessary.
        // The barrier is appended into the pending barrier list and not issued immediately,
        // instead waiting for any rendering, compute or transfer operation.
        // Use commitBarriers() to issue the barriers explicitly.
        // Like the other sampler feedback functions, only supported on DX12.
        virtual void SetSamplerFeedbackTextureState(EFRAPISamplerFeedbackTexture* texture, F_ResourceStates stateBits) =
        0;

        // Writes the provided data into the push constants block for the currently set pipeline.
        // A graphics, compute, ray tracing or meshlet state must be set using the corresponding call
        // (setGraphicsState etc.) before using setPushConstants. Changing the state invalidates push constants.
        // - DX11: Push constants for all pipelines and command lists use a single buffer associated with the
        //   NVRHI context. This function maps to UpdateSubresource on that buffer.
        // - DX12: Push constants map to root constants in the PSO/root signature. This function maps to
        //   SetGraphicsRoot32BitConstants for graphics or meshlet pipelines, and SetComputeRoot32BitConstants for
        //   compute or ray tracing pipelines.
        // - Vulkan: Push constants are just Vulkan push constants. This function maps to vkCmdPushConstants.
        // Note that NVRHI only supports one push constants binding in all layouts used in a pipeline.
        virtual void SetPushConstants(const void* data, size_t byteSize) = 0;

        // Sets the specified graphics state on the command list.
        // The state includes the pipeline (or individual shaders on DX11) and all resources bound to it,
        // from input buffers to render targets. See the members of GraphicsState for more information.
        // State is cached by NVRHI, so if some parts of it are not modified by the setGraphicsState(...) call,
        // the corresponding changes won't be made on the underlying graphics API. When combining command list
        // operations made through NVRHI and through direct access to the command list, state caching may lead to
        // incomplete or incorrect state being set on the underlying API because of cache mismatch with the actual
        // state. To avoid these issues, call clearState() when switching from direct command list access to NVRHI.
        virtual void SetGraphicsState(const GraphicsState& state) = 0;

        // Draws non-indexed primitivies using the current graphics state.
        // setGraphicsState(...) must be called between opening the command list or using other types of pipelines
        // and calling draw(...) or any of its siblings. If the pipeline uses push constants, those must be set
        // using setPushConstants(...) between setGraphicsState(...) and draw(...). If the pipeline uses volatile
        // constant buffers, their contents must be written using writeBuffer(...) between open(...) and draw(...),
        // which may be before or after setGraphicsState(...).
        // - DX11/12: Maps to DrawInstanced.
        // - Vulkan: Maps to vkCmdDraw.
        virtual void Draw(const DrawArguments& args) = 0;

        // Draws indexed primitivies using the current graphics state.
        // See the comment to draw(...) for state information.
        // - DX11/12: Maps to DrawIndexedInstanced.
        // - Vulkan: Maps to vkCmdDrawIndexed.
        virtual void DrawIndexed(const DrawArguments& args) = 0;

        // Draws one or multiple sets of non-indexed primitives using the parameters provided in the indirect buffer
        // specified in the prior call to setGraphicsState(...). The memory layout in the buffer is the same for all
        // graphics APIs and is described by the DrawIndirectArguments structure. If drawCount is more than 1,
        // multiple sets of primitives are drawn, and the parameter structures for them are tightly packed in the
        // indirect parameter buffer one after another.
        // See the comment to draw(...) for state information.
        // - DX11: Maps to multiple calls to DrawInstancedIndirect.
        // - DX12: Maps to ExecuteIndirect with a predefined signature.
        // - Vulkan: Maps to vkCmdDrawIndirect.
        virtual void DrawIndirect(uint32_t offsetBytes, uint32_t drawCount = 1) = 0;

        // Draws one or multiple sets of indexed primitives using the parameters provided in the indirect buffer
        // specified in the prior call to setGraphicsState(...). The memory layout in the buffer is the same for all
        // graphics APIs and is described by the DrawIndexedIndirectArguments structure. If drawCount is more than 1,
        // multiple sets of primitives are drawn, and the parameter structures for them are tightly packed in the
        // indirect parameter buffer one after another.
        // See the comment to draw(...) for state information.
        // - DX11: Maps to multiple calls to DrawIndexedInstancedIndirect.
        // - DX12: Maps to ExecuteIndirect with a predefined signature.
        // - Vulkan: Maps to vkCmdDrawIndexedIndirect.
        virtual void DrawIndexedIndirect(uint32_t offsetBytes, uint32_t drawCount = 1) = 0;

        // Sets the specified compute state on the command list.
        // The state includes the pipeline (or individual shaders on DX11) and all resources bound to it.
        // See the members of ComputeState for more information.
        // See the comment to setGraphicsState(...) for information on state caching.
        virtual void SetComputeState(const ComputeState& state) = 0;

        // Launches a compute kernel using the current compute state.
        // See the comment to draw(...) for information on state setting, push constants, and volatile constant buffers,
        // replacing graphics with compute.
        // - DX11/12: Maps to Dispatch.
        // - Vulkan: Maps to vkCmdDispatch.
        virtual void Dispatch(uint32_t groupsX, uint32_t groupsY = 1, uint32_t groupsZ = 1) = 0;

        // Launches a compute kernel using the parameters provided in the indirect buffer specified in the prior
        // call to setComputeState(...). The memory layout in the buffer is the same for all graphics APIs and is
        // described by the DispatchIndirectArguments structure.
        // See the comment to dispatch(...) for state information.
        // - DX11: Maps to DispatchIndirect.
        // - DX12: Maps to ExecuteIndirect with a predefined signature.
        // - Vulkan: Maps to vkCmdDispatchIndirect.
        virtual void DispatchIndirect(uint32_t offsetBytes) = 0;

        // Sets the specified meshlet rendering state on the command list.
        // The state includes the pipeline and all resources bound to it.
        // Not supported on DX11.
        // Meshlet support on DX12 and Vulkan can be queried using IDevice::queryFeatureSupport(Feature::Meshlets).
        // See the members of MeshletState for more information.
        // See the comment to setGraphicsState(...) for information on state caching.
        virtual void SetMeshletState(const MeshletState& state) = 0;

        // Draws meshlet primitives using the current meshlet state.
        // See the comment to draw(...) for information on state setting, push constants, and volatile constant buffers,
        // replacing graphics with meshlets.
        // - DX11: Not supported.
        // - DX12: Maps to DispatchMesh.
        // - Vulkan: Maps to vkCmdDispatchMesh.
        virtual void DispatchMesh(uint32_t groupsX, uint32_t groupsY = 1, uint32_t groupsZ = 1) = 0;

        // Starts measuring GPU execution time using the provided timer query at this point in the command list.
        // Use endTimerQuery(...) to stop measusing time, and IDevice::getTimerQueryTime(...) to get the results later.
        // The same timer query cannot be used multiple times within the same command list, or in different
        // command lists until it is resolved.
        // - DX11: Maps to Begin and End calls on two ID3D11Query objects.
        // - DX12: Maps to EndQuery.
        // - Vulkan: Maps to vkCmdResetQueryPool and vkCmdWriteTimestamp.
        virtual void BeginTimerQuery(EFRAPITimerQuery* query) = 0;

        // Stops measuring GPU execution time using the provided timer query at this point in the command list.
        // beginTimerQuery(...) must have been used on the same timer query in this command list previously.
        // - DX11: Maps to End calls on two ID3D11Query objects.
        // - DX12: Maps to EndQuery and ResolveQueryData.
        // - Vulkan: Maps to vkCmdWriteTimestamp.
        virtual void EndTimerQuery(EFRAPITimerQuery* query) = 0;

        // Places a debug marker denoting the beginning of a range of commands in the command list.
        // Use endMarker() to denote the end of the range. Ranges may be nested, i.e. calling beginMarker(...)
        // multiple times, followed by multiple endMarker(), is allowed.
        // - DX11: Maps to ID3DUserDefinedAnnotation::BeginEvent.
        // - DX12: Maps to PIXBeginEvent.
        // - Vulkan: Maps to cmdBeginDebugUtilsLabelEXT or cmdDebugMarkerBeginEXT.
        // If Nsight Aftermath integration is enabled, also calls GFSDK_Aftermath_SetEventMarker on DX11 and DX12.
        virtual void BeginMarker(const char* name) = 0;

        // Places a debug marker denoting the end of a range of commands in the command list.
        // - DX11: Maps to ID3DUserDefinedAnnotation::EndEvent.
        // - DX12: Maps to PIXEndEvent.
        // - Vulkan: Maps to cmdEndDebugUtilsLabelEXT or cmdDebugMarkerEndEXT.
        virtual void EndMarker() = 0;

        // Enables or disables the automatic barrier placement on set[...]State, copy, write, and clear operations.
        // By default, automatic barriers are enabled, but can be optionally disabled to improve CPU performance
        // and/or specific barrier placement. When automatic barriers are disabled, it is application's responsibility
        // to set correct states for all used resources.
        virtual void SetEnableAutomaticBarriers(bool enable) = 0;

        // Sets the necessary resource states for all non-permanent resources used in the binding set.
        virtual void SetResourceStatesForBindingSet(EFRAPIBindingSet* bindingSet) = 0;

        // Sets the necessary resource states for all targets of the framebuffer.
        EFRENDERAPI_API void SetResourceStatesForFramebuffer(const EFRAPIFramebuffer* framebuffer);

        // Enables or disables the placement of UAV barriers for the given texture (DX12/VK) or all resources (DX11)
        // between draw or dispatch calls. Disabling UAV barriers may improve performance in cases when the same
        // resource is used by multiple draws or dispatches, but they don't depend on each other's results.
        // Note that this only affects barriers between multiple uses of the same texture as a UAV, and the
        // transition barrier when the texture is first used as a UAV will still be placed.
        // - DX11: Maps to NvAPI_D3D11_BeginUAVOverlap (once - see source code) and requires NVAPI.
        // - DX12, Vulkan: Does not map to any specific API calls, affects NVRHI automatic barriers.
        virtual void SetEnableUavBarriersForTexture(EFRAPITexture* texture, bool enableBarriers) = 0;

        // Enables or disables the placement of UAV barriers for the given buffer (DX12/VK) or all resources (DX11)
        // between draw or dispatch calls.
        // See the comment to setEnableUavBarriersForTexture(...) for more information.
        virtual void SetEnableUavBarriersForBuffer(EFRAPIBuffer* buffer, bool enableBarriers) = 0;

        // Informs the command list state tracker of the current state of a texture or some of its subresources.
        // This function must be called after opening the command list and before the first use of any textures
        // that do not have the keepInitialState flag set, and that were not transitioned to a permanent state
        // previously using setPermanentTextureState(...).
        virtual void BeginTrackingTextureState(EFRAPITexture* texture, TextureSubresourceSet subresources,
                                               F_ResourceStates stateBits) = 0;

        // Informs the command list state tracker of the current state of a buffer.
        // See the comment to beginTrackingTextureState(...) for more information.
        virtual void BeginTrackingBufferState(EFRAPIBuffer* buffer, F_ResourceStates stateBits) = 0;

        // Places the neccessary barriers to make sure that the texture or some of its subresources are in the given
        // state. If the texture or subresources are already in that state, no action is performed.
        // If the texture was previously transitioned to a permanent state, the new state must be compatible
        // with that permanent state, and no action is performed.
        // The barriers are not immediately submitted to the underlying graphics API, but are placed to the pending
        // list instead. Call commitBarriers() to submit them to the grahics API explicitly or set graphics
        // or other type of state.
        // Has no effect on DX11.
        virtual void SetTextureState(EFRAPITexture* texture, TextureSubresourceSet subresources,
                                     F_ResourceStates stateBits) = 0;

        // Places the neccessary barriers to make sure that the buffer is in the given state.
        // See the comment to setTextureState(...) for more information.
        // Has no effect on DX11.
        virtual void SetBufferState(EFRAPIBuffer* buffer, F_ResourceStates stateBits) = 0;

        // Places the neccessary barriers to make sure that the entire texture is in the given state, and marks that
        // state as the texture's permanent state. Once a texture is transitioned into a permanent state, its state
        // can not be modified. This can improve performance by excluding the texture from automatic state tracking
        // in the future.
        // The barriers are not immediately submitted to the underlying graphics API, but are placed to the pending
        // list instead. Call commitBarriers() to submit them to the grahics API explicitly or set graphics
        // or other type of state.
        // Note that the permanent state transitions affect all command lists, and are only applied when the command
        // list that sets them is executed. If the command list is closed but not executed, the permanent states
        // will be abandoned.
        // Has no effect on DX11.
        virtual void SetPermanentTextureState(EFRAPITexture* texture, F_ResourceStates stateBits) = 0;

        // Places the neccessary barriers to make sure that the buffer is in the given state, and marks that state
        // as the buffer's permanent state. See the comment to setPermanentTextureState(...) for more information.
        // Has no effect on DX11.
        virtual void SetPermanentBufferState(EFRAPIBuffer* buffer, F_ResourceStates stateBits) = 0;

        // Flushes the barriers from the pending list into the graphics API command list.
        // Has no effect on DX11.
        virtual void CommitBarriers() = 0;

        // Returns the current tracked state of a texture subresource.
        // If the state is not known to the command list, returns ResourceStates::Unknown. Using the texture in this
        // state is not allowed.
        // On DX11, always returns ResourceStates::Common.
        virtual F_ResourceStates GetTextureSubresourceState(EFRAPITexture* texture, ArraySlice arraySlice,
                                                            MipLevel mipLevel) = 0;

        // Returns the current tracked state of a buffer.
        // See the comment to getTextureSubresourceState(...) for more information.
        virtual F_ResourceStates GetBufferState(EFRAPIBuffer* buffer) = 0;

        // Returns the owning device, does NOT call AddRef on it.
        virtual EFDynamicRAPI* GetDevice() = 0;

        // Returns the CommandListParameters structure that was used to create the command list.
        virtual const CommandListParameters& GetDesc() = 0;
    };

    typedef RefCountPtr<EFRAPICommandList> CommandListHandle;
}
