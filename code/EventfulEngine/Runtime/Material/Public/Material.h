#pragma once

#include "CoreTypes.h"

namespace EventfulEngine {

    enum class E_ShadingModel {
        Unlit,
        DefaultLit
    };

    enum class E_BlendMode {
        Opaque,
        Masked,
        Translucent
    };

    struct MaterialDesc {
        EFString name;
        E_ShadingModel shading{E_ShadingModel::DefaultLit};
        E_BlendMode blendMode{E_BlendMode::Opaque};
    };
} // EventfulEngine
