#pragma once

#include <GLFW/glfw3.h>
#include "CoreTypes.h"

namespace EventfulEngine{

    struct GenericPlatformApplication{
        static bool Startup();

        static void Shutdown();

        static EFString GetClipboardText();

        static void SetClipboardText(const EFString& text);

        static float PixelsToDip(float pixels, float scale);

        static float DipToPixels(float dips, float scale);

        static float GetDpiScale(GLFWmonitor* monitor);
    };

    using EFPlatformApplication = GenericPlatformApplication;
}
