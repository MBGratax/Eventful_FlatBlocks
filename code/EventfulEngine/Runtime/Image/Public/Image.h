#pragma once

#include "CoreTypes.h"

//TODO: This is just a descriptor stub to use in dependant modules before actual implementation happens
namespace EventfulEngine{
    enum class E_ImageFormat {
        Unknown,
        R8G8B8A8,
        R8G8B8
    };

    struct ImageDesc {
        EFString name;
        uint32 width{0};
        uint32 height{0};
        uint32 numChannels{0};
        E_ImageFormat format{E_ImageFormat::Unknown};
        bool bIsSRGB{false};
    };
}
