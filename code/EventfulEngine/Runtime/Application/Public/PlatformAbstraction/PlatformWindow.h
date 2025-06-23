#pragma once

#include "Platform.h"
#include "GenericPlatformWindow.h"

namespace EventfulEngine{
    class EFWindow{
    public:
        NOCOPY(EFWindow)

        EFWindow() = default;

        ~EFWindow() = default;

        bool Init(int width, int height, const char* title);

        void Shutdown();

        GLFWwindow* GetNativeHandle() const{ return _window; }

    private:
        GLFWwindow* _window{nullptr};
    };
}
