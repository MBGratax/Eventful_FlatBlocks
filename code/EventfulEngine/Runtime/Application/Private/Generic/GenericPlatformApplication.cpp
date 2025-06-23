#pragma once

#include "GenericPlatformApplication.h"

namespace EventfulEngine{

    bool GenericPlatformApplication::Startup(){
        return ::glfwInit() == GLFW_TRUE;
    }

    void GenericPlatformApplication::Shutdown(){
        ::glfwTerminate();
    }

    EFString GenericPlatformApplication::GetClipboardText(){
        const char* text = ::glfwGetClipboardString(nullptr);
        return text ? EFString(text) : EFString();
    }

    void GenericPlatformApplication::SetClipboardText(const EFString& text){
        ::glfwSetClipboardString(nullptr, text.c_str());
    }

    float GenericPlatformApplication::PixelsToDIP(const float pixels, const float scale){
        if (scale <= 0.f){ return pixels; }
        return pixels / scale;
    }

    float GenericPlatformApplication::DIPToPixels(const float dips, const float scale){
        return dips * scale;
    }

    float GenericPlatformApplication::GetDPIScale(GLFWmonitor* monitor){
        float x = 1.f;
        float y = 1.f;
        if (monitor){
            ::glfwGetMonitorContentScale(monitor, &x, &y);
        }
        return x;
    }

} // namespace EventfulEngine
