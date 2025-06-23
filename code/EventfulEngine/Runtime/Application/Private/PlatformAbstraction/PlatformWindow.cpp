#pragma once

#include "PlatformWindow.h"

namespace EventfulEngine{
    bool EFWindow::Init(const int width, const int height, const char* title){
        _window = EFPlatformWindow::CreateWindow(width, height, title);
        return _window != nullptr;
    }

    void EFWindow::Shutdown(){
        if (_window){
            EFPlatformWindow::DestroyWindow(_window);
            _window = nullptr;
        }
    }
} // EventfulEngine
