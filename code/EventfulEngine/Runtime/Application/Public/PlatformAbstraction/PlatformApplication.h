#pragma once
// Single include header for application code
// Includes Cores Platform.h to detect platform, and includes GenericPlatform headers and Specific platform Headers if necessary

#include "Platform.h"
#include "GenericPlatformApplication.h"
// Include platformspecific application here in theory

#include "PlatformAudio.h"
#include "PlatformWindow.h"
#include "EFApplicationModuleAPI.h"

namespace EventfulEngine{


    class EFAPPLICATION_API EFApplication final{
    public:
        ~EFApplication();

        bool Init();

        void Shutdown();

        std::shared_ptr<EFWindow> CreateWindow(int width, int height, const char* title);

        void DestroyWindow(EFWindow* window);

        std::shared_ptr<EFAudioDevice> OpenAudioDevice(const EFString& name);

        void CloseAudioDevice(EFAudioDevice* device);

        static EFString GetClipboardText();

        static void SetClipboardText(const EFString& text);

        float PixelsToDIP(float pixels) const;

        float DIPToPixels(float dips) const;

        bool IsGamepadConnected(int index) const;

        bool GetGamepadState(int index, GLFWgamepadstate& state) const;

        bool IsKeyPressed(GLFWwindow* window, int key) const;

        bool IsMouseButtonPressed(GLFWwindow* window, int button) const;

        void GetCursorPos(GLFWwindow* window, double& x, double& y) const;

        std::vector<EFString> GetAudioDevices() const;

        EFString GetDefaultAudioDevice() const;

        void PollEvents();

    protected:
        float _dpiScale{1.f};
        std::vector<std::shared_ptr<EFWindow>> _windows;
        std::vector<std::shared_ptr<EFAudioDevice>> _audioDevices;
    };
}
