#pragma once

#include "EfrapiBuffer.h"

namespace EventfulEngine{
    BufferRange BufferRange::Resolve(const BufferDesc& desc) const{
        BufferRange result;
        result.ByteOffset = std::min(ByteOffset, desc.ByteSize);
        if (ByteSize == 0)
            result.ByteSize = desc.ByteSize - result.ByteOffset;
        else
            result.ByteSize = std::min(ByteSize, desc.ByteSize - result.ByteOffset);
        return result;
    }

    FramebufferInfo::FramebufferInfo(const FramebufferDesc& desc){
        for (const auto& attachment : desc.ColorAttachments){
            colorFormats.push_back(attachment.Format == E_Format::UNKNOWN && attachment.Texture
                                       ? attachment.Texture->GetDesc().Format
                                       : attachment.Format);
        }

        if (desc.DepthAttachment.IsValid()){
            const TextureDesc& textureDesc = desc.DepthAttachment.Texture->GetDesc();
            depthFormat = textureDesc.Format;
            sampleCount = textureDesc.SampleCount;
            sampleQuality = textureDesc.SampleQuality;
        }
        else if (!desc.ColorAttachments.empty() && desc.ColorAttachments[0].IsValid()){
            const TextureDesc& textureDesc = desc.ColorAttachments[0].Texture->GetDesc();
            sampleCount = textureDesc.SampleCount;
            sampleQuality = textureDesc.SampleQuality;
        }
    }

    FramebufferInfoEx::FramebufferInfoEx(const FramebufferDesc& desc){
        if (desc.DepthAttachment.IsValid()){
            const TextureDesc& textureDesc = desc.DepthAttachment.Texture->GetDesc();
            Width = std::max(textureDesc.Width >> desc.DepthAttachment.Subresources.BaseMipLevel, 1u);
            Height = std::max(textureDesc.Height >> desc.DepthAttachment.Subresources.BaseMipLevel, 1u);
        }
        else if (!desc.ColorAttachments.empty() && desc.ColorAttachments[0].IsValid()){
            const TextureDesc& textureDesc = desc.ColorAttachments[0].Texture->GetDesc();
            Width = std::max(textureDesc.Width >> desc.ColorAttachments[0].Subresources.BaseMipLevel, 1u);
            Height = std::max(textureDesc.Height >> desc.ColorAttachments[0].Subresources.BaseMipLevel, 1u);
        }
    }
} // EventfulEngine
