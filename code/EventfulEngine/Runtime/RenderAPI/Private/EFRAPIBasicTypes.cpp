#pragma once

#include <array>

#include "EFRAPIBasicTypes.h"

#include <cassert>

namespace EventfulEngine{
    // Format mapping table. The rows must be in the exactly same order as Format enum members are defined.
    static const std::array<FormatInfo, 68> FORMAT_INFO = {
            {
                //        format                   name             bytes blk         kind               red   green   blue  alpha  depth  stencl signed  srgb
                {
                    E_Format::UNKNOWN, "UNKNOWN", 0, 0, FormatKind::Integer, false, false, false,
                    false, false, false, false, false
                },
                {
                    E_Format::R8_UINT, "R8_UINT", 1, 1, FormatKind::Integer, true, false, false,
                    false, false, false, false, false
                },
                {
                    E_Format::R8_SINT, "R8_SINT", 1, 1, FormatKind::Integer, true, false, false,
                    false, false, false, true, false
                },
                {
                    E_Format::R8_UNORM, "R8_UNORM", 1, 1, FormatKind::Normalized, true, false, false,
                    false, false, false, false, false
                },
                {
                    E_Format::R8_SNORM, "R8_SNORM", 1, 1, FormatKind::Normalized, true, false, false,
                    false, false, false, true, false
                },
                {
                    E_Format::RG8_UINT, "RG8_UINT", 2, 1, FormatKind::Integer, true, true, false,
                    false, false, false, false, false
                },
                {
                    E_Format::RG8_SINT, "RG8_SINT", 2, 1, FormatKind::Integer, true, true, false,
                    false, false, false, true, false
                },
                {
                    E_Format::RG8_UNORM, "RG8_UNORM", 2, 1, FormatKind::Normalized, true, true, false,
                    false, false, false, false, false
                },
                {
                    E_Format::RG8_SNORM, "RG8_SNORM", 2, 1, FormatKind::Normalized, true, true, false,
                    false, false, false, true, false
                },
                {
                    E_Format::R16_UINT, "R16_UINT", 2, 1, FormatKind::Integer, true, false, false,
                    false, false, false, false, false
                },
                {
                    E_Format::R16_SINT, "R16_SINT", 2, 1, FormatKind::Integer, true, false, false,
                    false, false, false, true, false
                },
                {
                    E_Format::R16_UNORM, "R16_UNORM", 2, 1, FormatKind::Normalized, true, false, false,
                    false, false, false, false, false
                },
                {
                    E_Format::R16_SNORM, "R16_SNORM", 2, 1, FormatKind::Normalized, true, false, false,
                    false, false, false, true, false
                },
                {
                    E_Format::R16_FLOAT, "R16_FLOAT", 2, 1, FormatKind::Float, true, false, false,
                    false, false, false, true, false
                },
                {
                    E_Format::BGRA4_UNORM, "BGRA4_UNORM", 2, 1, FormatKind::Normalized, true, true, true,
                    true, false, false, false, false
                },
                {
                    E_Format::B5G6R5_UNORM, "B5G6R5_UNORM", 2, 1, FormatKind::Normalized, true, true, true,
                    false, false, false, false, false
                },
                {
                    E_Format::B5G5R5A1_UNORM, "B5G5R5A1_UNORM", 2, 1, FormatKind::Normalized, true, true, true,
                    true, false, false, false, false
                },
                {
                    E_Format::RGBA8_UINT, "RGBA8_UINT", 4, 1, FormatKind::Integer, true, true, true,
                    true, false, false, false, false
                },
                {
                    E_Format::RGBA8_SINT, "RGBA8_SINT", 4, 1, FormatKind::Integer, true, true, true,
                    true, false, false, true, false
                },
                {
                    E_Format::RGBA8_UNORM, "RGBA8_UNORM", 4, 1, FormatKind::Normalized, true, true, true,
                    true, false, false, false, false
                },
                {
                    E_Format::RGBA8_SNORM, "RGBA8_SNORM", 4, 1, FormatKind::Normalized, true, true, true,
                    true, false, false, true, false
                },
                {
                    E_Format::BGRA8_UNORM, "BGRA8_UNORM", 4, 1, FormatKind::Normalized, true, true, true,
                    true, false, false, false, false
                },
                {
                    E_Format::SRGBA8_UNORM, "SRGBA8_UNORM", 4, 1, FormatKind::Normalized, true, true, true,
                    true, false, false, false, true
                },
                {
                    E_Format::SBGRA8_UNORM, "SBGRA8_UNORM", 4, 1, FormatKind::Normalized, true, true, true,
                    true, false, false, false, false
                },
                {
                    E_Format::R10G10B10A2_UNORM, "R10G10B10A2_UNORM", 4, 1, FormatKind::Normalized, true, true, true,
                    true, false, false, false, false
                },
                {
                    E_Format::R11G11B10_FLOAT, "R11G11B10_FLOAT", 4, 1, FormatKind::Float, true, true, true,
                    false, false, false, false, false
                },
                {
                    E_Format::RG16_UINT, "RG16_UINT", 4, 1, FormatKind::Integer, true, true, false,
                    false, false, false, false, false
                },
                {
                    E_Format::RG16_SINT, "RG16_SINT", 4, 1, FormatKind::Integer, true, true, false,
                    false, false, false, true, false
                },
                {
                    E_Format::RG16_UNORM, "RG16_UNORM", 4, 1, FormatKind::Normalized, true, true, false,
                    false, false, false, false, false
                },
                {
                    E_Format::RG16_SNORM, "RG16_SNORM", 4, 1, FormatKind::Normalized, true, true, false,
                    false, false, false, true, false
                },
                {
                    E_Format::RG16_FLOAT, "RG16_FLOAT", 4, 1, FormatKind::Float, true, true, false,
                    false, false, false, true, false
                },
                {
                    E_Format::R32_UINT, "R32_UINT", 4, 1, FormatKind::Integer, true, false, false,
                    false, false, false, false, false
                },
                {
                    E_Format::R32_SINT, "R32_SINT", 4, 1, FormatKind::Integer, true, false, false,
                    false, false, false, true, false
                },
                {
                    E_Format::R32_FLOAT, "R32_FLOAT", 4, 1, FormatKind::Float, true, false, false,
                    false, false, false, true, false
                },
                {
                    E_Format::RGBA16_UINT, "RGBA16_UINT", 8, 1, FormatKind::Integer, true, true, true,
                    true, false, false, false, false
                },
                {
                    E_Format::RGBA16_SINT, "RGBA16_SINT", 8, 1, FormatKind::Integer, true, true, true,
                    true, false, false, true, false
                },
                {
                    E_Format::RGBA16_FLOAT, "RGBA16_FLOAT", 8, 1, FormatKind::Float, true, true, true,
                    true, false, false, true, false
                },
                {
                    E_Format::RGBA16_UNORM, "RGBA16_UNORM", 8, 1, FormatKind::Normalized, true, true, true,
                    true, false, false, false, false
                },
                {
                    E_Format::RGBA16_SNORM, "RGBA16_SNORM", 8, 1, FormatKind::Normalized, true, true, true,
                    true, false, false, true, false
                },
                {
                    E_Format::RG32_UINT, "RG32_UINT", 8, 1, FormatKind::Integer, true, true, false,
                    false, false, false, false, false
                },
                {
                    E_Format::RG32_SINT, "RG32_SINT", 8, 1, FormatKind::Integer, true, true, false,
                    false, false, false, true, false
                },
                {
                    E_Format::RG32_FLOAT, "RG32_FLOAT", 8, 1, FormatKind::Float, true, true, false,
                    false, false, false, true, false
                },
                {
                    E_Format::RGB32_UINT, "RGB32_UINT", 12, 1, FormatKind::Integer, true, true, true,
                    false, false, false, false, false
                },
                {
                    E_Format::RGB32_SINT, "RGB32_SINT", 12, 1, FormatKind::Integer, true, true, true,
                    false, false, false, true, false
                },
                {
                    E_Format::RGB32_FLOAT, "RGB32_FLOAT", 12, 1, FormatKind::Float, true, true, true,
                    false, false, false, true, false
                },
                {
                    E_Format::RGBA32_UINT, "RGBA32_UINT", 16, 1, FormatKind::Integer, true, true, true,
                    true, false, false, false, false
                },
                {
                    E_Format::RGBA32_SINT, "RGBA32_SINT", 16, 1, FormatKind::Integer, true, true, true,
                    true, false, false, true, false
                },
                {
                    E_Format::RGBA32_FLOAT, "RGBA32_FLOAT", 16, 1, FormatKind::Float, true, true, true,
                    true, false, false, true, false
                },
                {
                    E_Format::D16, "D16", 2, 1, FormatKind::DepthStencil, false, false, false,
                    false, true, false, false, false
                },
                {
                    E_Format::D24S8, "D24S8", 4, 1, FormatKind::DepthStencil, false, false, false,
                    false, true, true, false, false
                },
                {
                    E_Format::X24G8_UINT, "X24G8_UINT", 4, 1, FormatKind::Integer, false, false, false,
                    false, false, true, false, false
                },
                {
                    E_Format::D32, "D32", 4, 1, FormatKind::DepthStencil, false, false, false,
                    false, true, false, false, false
                },
                {
                    E_Format::D32S8, "D32S8", 8, 1, FormatKind::DepthStencil, false, false, false,
                    false, true, true, false, false
                },
                {
                    E_Format::X32G8_UINT, "X32G8_UINT", 8, 1, FormatKind::Integer, false, false, false,
                    false, false, true, false, false
                },
                {
                    E_Format::BC1_UNORM, "BC1_UNORM", 8, 4, FormatKind::Normalized, true, true, true,
                    true, false, false, false, false
                },
                {
                    E_Format::BC1_UNORM_SRGB, "BC1_UNORM_SRGB", 8, 4, FormatKind::Normalized, true, true, true,
                    true, false, false, false, true
                },
                {
                    E_Format::BC2_UNORM, "BC2_UNORM", 16, 4, FormatKind::Normalized, true, true, true,
                    true, false, false, false, false
                },
                {
                    E_Format::BC2_UNORM_SRGB, "BC2_UNORM_SRGB", 16, 4, FormatKind::Normalized, true, true, true,
                    true, false, false, false, true
                },
                {
                    E_Format::BC3_UNORM, "BC3_UNORM", 16, 4, FormatKind::Normalized, true, true, true,
                    true, false, false, false, false
                },
                {
                    E_Format::BC3_UNORM_SRGB, "BC3_UNORM_SRGB", 16, 4, FormatKind::Normalized, true, true, true,
                    true, false, false, false, true
                },
                {
                    E_Format::BC4_UNORM, "BC4_UNORM", 8, 4, FormatKind::Normalized, true, false, false,
                    false, false, false, false, false
                },
                {
                    E_Format::BC4_SNORM, "BC4_SNORM", 8, 4, FormatKind::Normalized, true, false, false,
                    false, false, false, true, false
                },
                {
                    E_Format::BC5_UNORM, "BC5_UNORM", 16, 4, FormatKind::Normalized, true, true, false,
                    false, false, false, false, false
                },
                {
                    E_Format::BC5_SNORM, "BC5_SNORM", 16, 4, FormatKind::Normalized, true, true, false,
                    false, false, false, true, false
                },
                {
                    E_Format::BC6H_UFLOAT, "BC6H_UFLOAT", 16, 4, FormatKind::Float, true, true, true,
                    false, false, false, false, false
                },
                {
                    E_Format::BC6H_SFLOAT, "BC6H_SFLOAT", 16, 4, FormatKind::Float, true, true, true,
                    false, false, false, true, false
                },
                {
                    E_Format::BC7_UNORM, "BC7_UNORM", 16, 4, FormatKind::Normalized, true, true, true,
                    true, false, false, false, false
                },
                {
                    E_Format::BC7_UNORM_SRGB, "BC7_UNORM_SRGB", 16, 4, FormatKind::Normalized, true, true, true,
                    true, false, false, false, true
                },
            }
        };

    const FormatInfo& GetFormatInfo(E_Format format){
        static_assert(std::size(FORMAT_INFO) == static_cast<size_t>(E_Format::COUNT),
                      "The format info table doesn't have the right number of elements");

        if (static_cast<uint32_t>(format) >= static_cast<uint32_t>(E_Format::COUNT))
            return FORMAT_INFO[0]; // UNKNOWN

        const FormatInfo& info = FORMAT_INFO[static_cast<uint32>(format)];
        assert(info.format == format);
        return info;
    }
} // EventfulEngine
