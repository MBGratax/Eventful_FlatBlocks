#pragma once

#include "EFRAPICommandList.h"

namespace EventfulEngine{
    void EFRAPICommandList::SetResourceStatesForFramebuffer(const EFRAPIFramebuffer* framebuffer){
        const FramebufferDesc& desc = framebuffer->GetDesc();

        for (const auto& attachment : desc.ColorAttachments){
            SetTextureState(attachment.Texture, attachment.Subresources,
                            F_ResourceStates::RenderTarget);
        }

        if (desc.DepthAttachment.IsValid()){
            SetTextureState(desc.DepthAttachment.Texture, desc.DepthAttachment.Subresources,
                            desc.DepthAttachment.B_IsReadOnly
                                ? F_ResourceStates::DepthRead
                                : F_ResourceStates::DepthWrite);
        }
    }
}
