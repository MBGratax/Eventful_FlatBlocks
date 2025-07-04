#pragma once

#include "EFRAPIPipelineState.h"

namespace EventfulEngine{
    bool BlendState::RenderTarget::UsesConstantColor() const{
        return srcBlend == E_BlendFactor::ConstantColor || srcBlend == E_BlendFactor::OneMinusConstantColor ||
            destBlend == E_BlendFactor::ConstantColor || destBlend == E_BlendFactor::OneMinusConstantColor ||
            srcBlendAlpha == E_BlendFactor::ConstantColor || srcBlendAlpha == E_BlendFactor::OneMinusConstantColor ||
            destBlendAlpha == E_BlendFactor::ConstantColor || destBlendAlpha == E_BlendFactor::OneMinusConstantColor;
    }

    bool BlendState::UsesConstantColor(const uint32_t numTargets) const{
        for (uint32_t rt = 0; rt < numTargets; rt++){
            if (targets[rt].UsesConstantColor())
                return true;
        }

        return false;
    }
} // EventfulEngine
