#pragma once

#include <GLFW/glfw3.h>
#include "CoreTypes.h"

namespace EventfulEngine{

    struct GenericPlatformApplication{
        static bool Startup();

        static void Shutdown();

        static EFString GetClipboardText();

        static void SetClipboardText(const EFString& text);

        static float PixelsToDIP(float pixels, float scale);

        static float DIPToPixels(float dips, float scale);

        static float GetDPIScale(GLFWmonitor* monitor);
    };

    using EFPlatformApplication = GenericPlatformApplication;
}
