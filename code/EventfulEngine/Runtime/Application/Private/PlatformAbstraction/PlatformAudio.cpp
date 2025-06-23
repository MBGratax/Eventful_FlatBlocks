#pragma once

#include "PlatformAudio.h"

namespace EventfulEngine{
    bool EFAudioDevice::Init(const EFString& name){
        _name = name;
        _device = ::alcOpenDevice(name.empty() ? nullptr : name.c_str());
        return _device != nullptr;
    }

    void EFAudioDevice::Shutdown(){
        if (_device){
            ::alcCloseDevice(_device);
            _device = nullptr;
        }
    }
} // EventfulEngine
