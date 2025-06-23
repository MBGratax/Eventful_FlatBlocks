#pragma once

#include "CoreTypes.h"

namespace EventfulEngine {

    enum class E_TextureFormat {
        Unknown,
        R8G8B8A8,
        R8G8B8
    };

    struct TextureDesc {
        EFString name;
        uint32 width{0};
        uint32 height{0};
        uint32 mipLevels{1};
        E_TextureFormat format{E_TextureFormat::Unknown};
        bool bIsSRGB{false};
    };

} // EventfulEngine
