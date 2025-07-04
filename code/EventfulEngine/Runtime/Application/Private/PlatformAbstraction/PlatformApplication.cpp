#pragma once

#include "PlatformApplication.h"
#include "PlatformWindow.h"
#include "PlatformAudio.h"
#include "PlatformInput.h"

namespace EventfulEngine{
    EFApplication::~EFApplication(){
        Shutdown();
    }

    bool EFApplication::Init(){
        if (EFPlatformApplication::Startup()){
            _dpiScale = EFPlatformApplication::GetDpiScale(::glfwGetPrimaryMonitor());
            return true;
        }
        return false;
    }

    void EFApplication::Shutdown(){
        for (auto& window : _windows){
            if (window){
                window->Shutdown();
            }
        }
        _windows.clear();

        for (auto& device : _audioDevices){
            if (device){
                device->Shutdown();
            }
        }
        _audioDevices.clear();

        EFPlatformApplication::Shutdown();
    }

    std::shared_ptr<EFWindow> EFApplication::CreateWindow(const int width, const int height, const char* title){
        if (auto window = std::make_shared<EFWindow>(); window->Init(width, height, title)){
            _windows.emplace_back(std::move(window));
            return window;
        }
        return nullptr;
    }

    void EFApplication::DestroyWindow(EFWindow* window){
        if (const auto windowItem = std::ranges::find_if(_windows, [window](const auto& w){ return w.get() == window; })
            ; windowItem != _windows.end()){
            (*windowItem)->Shutdown();
            _windows.erase(windowItem);
        }
    }

    std::shared_ptr<EFAudioDevice> EFApplication::OpenAudioDevice(const EFString& name){
        if (auto device = std::make_shared<EFAudioDevice>(); device->Init(name)){
            _audioDevices.emplace_back(std::move(device));
            return device;
        }
        return nullptr;
    }

    void EFApplication::CloseAudioDevice(EFAudioDevice* device){
        if (const auto it = std::ranges::find_if(_audioDevices, [device](const auto& d){ return d.get() == device; });
            it != _audioDevices.end()){
            (*it)->Shutdown();
            _audioDevices.erase(it);
        }
    }

    EFString EFApplication::GetClipboardText(){
        return EFPlatformApplication::GetClipboardText();
    }

    void EFApplication::SetClipboardText(const EFString& text){
        EFPlatformApplication::SetClipboardText(text);
    }

    float EFApplication::PixelsToDip(const float pixels) const{
        return EFPlatformApplication::PixelsToDip(pixels, _dpiScale);
    }

    float EFApplication::DipToPixels(const float dips) const{
        return EFPlatformApplication::DipToPixels(dips, _dpiScale);
    }


    bool EFApplication::IsGamepadConnected(const int index) const{
        return EFPlatformInput::IsGamepadConnected(index);
    }

    bool EFApplication::GetGamepadState(const int index, GLFWgamepadstate& state) const{
        return EFPlatformInput::GetGamepadState(index, state);
    }

    bool EFApplication::IsKeyPressed(GLFWwindow* window, const int key) const{
        return EFPlatformInput::IsKeyPressed(window, key);
    }

    bool EFApplication::IsMouseButtonPressed(GLFWwindow* window, const int button) const{
        return EFPlatformInput::IsMouseButtonPressed(window, button);
    }

    void EFApplication::GetCursorPos(GLFWwindow* window, double& x, double& y) const{
        return EFPlatformInput::GetCursorPos(window, x, y);
    }

    std::vector<EFString> EFApplication::GetAudioDevices() const{
        return EFPlatformAudio::EnumerateOutputDevices();
    }

    EFString EFApplication::GetDefaultAudioDevice() const{
        return EFPlatformAudio::GetDefaultOutputDevice();
    }

    void EFApplication::PollEvents(){
        EFPlatformWindow::PollEvents();
    }

} // EventfulEngine
