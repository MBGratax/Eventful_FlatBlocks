#pragma once

#include "GenericPlatformAudio.h"
#include <AL/alc.h>
#include <cstring>

namespace EventfulEngine{

    std::vector<EFString> GenericPlatformAudio::EnumerateOutputDevices(){
        std::vector<EFString> devices;
#ifdef ALC_ENUMERATION_EXT
        const ALCchar* ptr = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);
        while (ptr && *ptr) {
            devices.emplace_back(ptr);
            ptr += std::strlen(ptr) + 1;
        }
#else
        if (const ALCchar* device = ::alcGetString(nullptr, ALC_DEVICE_SPECIFIER)){
            devices.emplace_back(device);
        }
#endif
        return devices;
    }

    EFString GenericPlatformAudio::GetDefaultOutputDevice(){
        const ALCchar* device = ::alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
        return device ? EFString(device) : EFString();
    }

} // namespace EventfulEngine
