#pragma once

#include "../Public/EventfulEngineLoop.h"

namespace EventfulEngine{
    int32 EventfulEngineLoop::PreInitProcessCLI(int32 ArgC, const EFString ArgV[],
                                                const EFString* AdditionalCommandline){
        EFString cmdLine;
        for (int32 i = 0; i < ArgC; ++i){
            cmdLine += ArgV[i];
            if (i + 1 < ArgC){
                cmdLine += ' ';
            }
        }
        if (AdditionalCommandline != nullptr){
            if (!cmdLine.empty()){
                cmdLine += ' ';
            }
            cmdLine += *AdditionalCommandline;
        }
        return BeforeEngineInit(cmdLine);
    }

    int32 EventfulEngineLoop::BeforeEngineInit(const EFString& CmdLine){
        int32 error = PreInitPreStartupScreen(CmdLine);
        if (error != 0){
            return error;
        }
        error = PreInitPostStartupScreen(CmdLine);
        return error;
    }

    int32 EventfulEngineLoop::PreInitPreStartupScreen(const EFString& CmdLine){
        (void)CmdLine;
        LoadPreInitModules();
        return 0;
    }

    int32 EventfulEngineLoop::PreInitPostStartupScreen(const EFString& CmdLine){
        (void)CmdLine;
        const bool success = LoadCoreModules();
        return success ? 0 : 1;
    }

    void EventfulEngineLoop::LoadPreInitModules(){
        // stub for loading pre-initialization modules
    }

    bool EventfulEngineLoop::LoadCoreModules(){
        // stub for loading core modules
        return true;
    }

    void EventfulEngineLoop::OverrideProjectModule(const EFString& InOriginalProjectModuleName,
                                                   const EFString& InReplacementProjectModuleName){
        (void)InOriginalProjectModuleName;
        (void)InReplacementProjectModuleName;
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

    void EventfulEngineLoop::PostInitRHI(){
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
