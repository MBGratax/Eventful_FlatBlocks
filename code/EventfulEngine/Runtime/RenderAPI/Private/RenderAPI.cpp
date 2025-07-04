#pragma once

#include "RenderAPI.h"

#include <ModuleManager.h>

namespace EventfulEngine{

    DynamicRAPIHandle g_dynamicRAPI = nullptr;

    void RenderAPIModule::ShutdownModule(){
        IModule::ShutdownModule();
        if (_currentRAPI){
            ModuleManager::Get().UnloadModule(_currentRAPIName);
        }
    }

    bool RenderAPIModule::SupportsDynamicReloading(){
        return true;
    }

    void RenderAPIModule::InitializeDynamicRAPI(E_RenderAPI api){
        switch (api){
            using enum E_RenderAPI;
        case Headless: _currentRAPIName = "Headless";
            break;
        case OpenGL: _currentRAPIName = "OpenGL";
            break;
        default: break;
        }

        ModuleManager& moduleManager = ModuleManager::Get();

        if (_currentRAPI){
            moduleManager.UnloadModule(_currentRAPIName);
        }

        if (moduleManager.LoadModule(_currentRAPIName)){
            _currentRAPI = moduleManager.GetModuleAs<IDynamicRAPIModule>(_currentRAPIName);
            g_dynamicRAPI = _currentRAPI->CreateDynamicRAPI(E_Feature::WaveLaneCountMinMax);
        }
    }
}
