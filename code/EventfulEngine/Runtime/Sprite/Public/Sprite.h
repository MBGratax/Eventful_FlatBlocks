#pragma once

#include "CoreTypes.h"

namespace EventfulEngine {

    enum class E_SpritePivotMode {
        TopLeft,
        TopCenter,
        TopRight,
        CenterLeft,
        Center,
        CenterRight,
        BottomLeft,
        BottomCenter,
        BottomRight,
        Custom
    };

    struct SpriteImportSettings {
        E_SpritePivotMode pivotMode{E_SpritePivotMode::Center};
        float customPivotX{0.5f};
        float customPivotY{0.5f};
        float pixelsPerUnit{1.0f};
        bool trimTransparentBorder{false};
    };

    struct SpriteDesc {
        EFString name;
        EFString texture;
        uint32 width{0};
        uint32 height{0};
        float pivotX{0.5f};
        float pivotY{0.5f};
        SpriteImportSettings import;
    };

} // EventfulEngine
