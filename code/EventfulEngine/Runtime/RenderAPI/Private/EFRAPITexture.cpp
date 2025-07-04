#pragma once

#include "EFRAPITexture.h"

#include <cassert>

namespace EventfulEngine{
    TextureSlice TextureSlice::Resolve(const TextureDesc& desc) const{
        TextureSlice ret(*this);

        assert(TexMipLevel < desc.MipLevels);

        if (Width == static_cast<uint32>(-1))
            ret.Width = std::max(desc.Width >> TexMipLevel, 1u);

        if (Height == static_cast<uint32>(-1))
            ret.Height = std::max(desc.Height >> TexMipLevel, 1u);

        if (Depth == static_cast<uint32>(-1)){
            if (desc.Dimension == E_TextureDimension::Texture3D)
                ret.Depth = std::max(desc.Depth >> TexMipLevel, 1u);
            else
                ret.Depth = 1;
        }

        return ret;
    }

    TextureSubresourceSet TextureSubresourceSet::Resolve(const TextureDesc& desc, const bool singleMipLevel) const{
        TextureSubresourceSet ret;
        ret.BaseMipLevel = BaseMipLevel;

        if (singleMipLevel){
            ret.NumMipLevels = 1;
        }
        else{
            const int lastMipLevelPlusOne = std::min(BaseMipLevel + NumMipLevels, desc.MipLevels);
            ret.NumMipLevels = static_cast<MipLevel>(std::max(0u, lastMipLevelPlusOne - BaseMipLevel));
        }

        switch (desc.Dimension) // NOLINT(clang-diagnostic-switch-enum)
        {
        case E_TextureDimension::Texture1DArray:
        case E_TextureDimension::Texture2DArray:
        case E_TextureDimension::TextureCube:
        case E_TextureDimension::TextureCubeArray:
        case E_TextureDimension::Texture2DMSArray: {
            ret.BaseArraySlice = BaseArraySlice;
            const int lastArraySlicePlusOne = std::min(BaseArraySlice + NumArraySlices, desc.ArraySize);
            ret.NumArraySlices = static_cast<ArraySlice>(std::max(0u, lastArraySlicePlusOne - BaseArraySlice));
            break;
        }
        default:
            ret.BaseArraySlice = 0;
            ret.NumArraySlices = 1;
            break;
        }

        return ret;
    }

    bool TextureSubresourceSet::IsEntireTexture(const TextureDesc& desc) const{
        if (BaseMipLevel > 0u || BaseMipLevel + NumMipLevels < desc.MipLevels)
            return false;

        switch (desc.Dimension) // NOLINT(clang-diagnostic-switch-enum)
        {
        case E_TextureDimension::Texture1DArray:
        case E_TextureDimension::Texture2DArray:
        case E_TextureDimension::TextureCube:
        case E_TextureDimension::TextureCubeArray:
        case E_TextureDimension::Texture2DMSArray:
            if (BaseArraySlice > 0u || BaseArraySlice + NumArraySlices < desc.ArraySize)
                return false;
        default:
            return true;
        }
    }

} // EventfulEngine
