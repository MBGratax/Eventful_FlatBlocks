#pragma once

#include "../Public/EventfulEngineLoop.h"

#include <CoreGlobals.h>

namespace EventfulEngine{
    int32 EventfulEngineLoop::PreInitProcessCli(
    ){
        // TODO: Encapsulate default CLI options in a free function somewhere else
        g_commandLine.AddOption<bool>("v,verbose", "Enable verbose logging",
                                      "Use -v or -verbose to enable verbose logging", "false");
        g_commandLine.Parse();
        return BeforeEngineInit();
    }

    int32 EventfulEngineLoop::BeforeEngineInit(){
        bool success = true;
        success &= LoadPreInitModules();
        // TODO: after doing preinit modules, grab cli options we need
        success &= LoadCoreModules();
        return success ? 0 : 1;
    }

    int32 EventfulEngineLoop::PreInitPreStartupScreen(const EFString& cmdLine){
        (void)cmdLine;
        LoadPreInitModules();
        return 0;
    }

    int32 EventfulEngineLoop::PreInitPostStartupScreen(const EFString& cmdLine){
        (void)cmdLine;
        const bool success = LoadCoreModules();
        return success ? 0 : 1;
    }

    bool EventfulEngineLoop::LoadPreInitModules(){
        // stub for loading pre-initialization modules
        return true;
    }

    bool EventfulEngineLoop::LoadCoreModules(){
        // stub for loading core modules
        return true;
    }

    void EventfulEngineLoop::OverrideProjectModule(const EFString& inOriginalProjectModuleName,
                                                   const EFString& inReplacementProjectModuleName){
        (void)inOriginalProjectModuleName;
        (void)inReplacementProjectModuleName;
        // stub for overriding project module
    }

    void EventfulEngineLoop::CleanupPreInitContext(){
        // stub for cleaning up preinit context
    }

#if WITH_ENGINE
    bool EventfulEngineLoop::LoadStartupCoreModules(){
        // stub for loading startup core modules
        return true;
    }

    bool EventfulEngineLoop::LoadStartupModules(){
        // stub for loading startup modules
        return true;
    }

    int32 EventfulEngineLoop::Init(){
        InitTime();
        if (!LoadStartupCoreModules() || !LoadStartupModules()){
            return 1;
        }
        return 0;
    }

    void EventfulEngineLoop::InitTime(){
        EFPlatformTime::Init();
    }

    void EventfulEngineLoop::Exit(){
        AppPreExit();
        AppExit();
    }

    bool EventfulEngineLoop::ShouldUseIdleMode() const{
        return true;
    }

    void EventfulEngineLoop::Tick(){
        EFPlatformTime::StepTime();
    }

    void EventfulEngineLoop::ClearPendingCleanupObjects(){
        // stub for clearing pending cleanup objects
    }
#endif // WITH_ENGINE

    void EventfulEngineLoop::PostInitGraphicsApi(){
        // stub for post RHI initialization
    }

    bool EventfulEngineLoop::AppInit(){
        return true;
    }

    void EventfulEngineLoop::AppPreExit(){
        // stub for application pre-exit tasks
    }

    void EventfulEngineLoop::AppExit(){
        // stub for application exit tasks
    }

} // namespace EventfulEngine
