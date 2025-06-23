#pragma once

#include "Platform.h"
#include "GenericPlatformAudio.h"

#include <AL/alc.h>
#include <vector>

namespace EventfulEngine{
    class EFAudioDevice{
    public:
        NOCOPY(EFAudioDevice)

        EFAudioDevice() = default;

        explicit EFAudioDevice(const EFString& name){ Init(name); }

        ~EFAudioDevice() = default;

        bool Init(const EFString& name);

        void Shutdown();

        [[nodiscard]] bool IsValid() const{ return _device != nullptr; }
        [[nodiscard]] const EFString& GetName() const{ return _name; }

    private:
        ALCdevice* _device{nullptr};
        EFString _name;
    };

    std::vector<EFString> EnumerateAudioDevices();

    EFString GetDefaultAudioDevice();
}
