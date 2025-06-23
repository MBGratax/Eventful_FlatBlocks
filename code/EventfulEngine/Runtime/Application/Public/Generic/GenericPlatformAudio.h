#pragma once

#include <vector>
#include "CoreTypes.h"

namespace EventfulEngine{

    struct GenericPlatformAudio{
        static std::vector<EFString> EnumerateOutputDevices();

        static EFString GetDefaultOutputDevice();
    };

    using EFPlatformAudio = GenericPlatformAudio;

} // namespace EventfulEngine
