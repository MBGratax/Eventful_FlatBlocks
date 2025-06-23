#pragma once

#include "CoreTypes.h"

namespace EventfulEngine {

    enum class E_ShaderStage {
        Vertex,
        Fragment,
        Geometry,
        Compute
    };

    struct ShaderDesc {
        EFString name;
        E_ShaderStage stage{E_ShaderStage::Vertex};
        EFString entryPoint{"main"};
        EFString filePath;
    };
} // EventfulEngin
