#pragma once

#include "ModuleManager.h"
#include "ApplicationModule.h"

void EventfulEngine::ApplicationModule::StartupModule(){
    IModule::StartupModule();
    _application.Init();
}

void EventfulEngine::ApplicationModule::ShutdownModule(){
    IModule::ShutdownModule();
    _application.Shutdown();
}

IMPLEMENT_MODULE(EventfulEngine::ApplicationModule, EFApplication)
