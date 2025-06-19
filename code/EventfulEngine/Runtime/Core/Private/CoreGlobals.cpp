#pragma once
#include "CoreTypes.h"
#include "../Public/CoreGlobals.h"

namespace EventfulEngine{

    EFCORE_API EF_DEFINE_LOG_CATEGORY(CoreLog, "CoreLog.txt", E_EFLogLevel::trace)
    GameEditorOverrides g_gameEditorOverrides;

    bool g_isRequestingExit = false;

    bool g_shouldRequestExit = false;

    void EFCORE_API BeginExitIfRequested(){
        if (g_shouldRequestExit){
            g_isRequestingExit = true;
        }
    }

    void EFCORE_API RequestEngineExit(){
        g_shouldRequestExit = true;
    }

    double g_startTime = EFPlatformTime::InitTiming();
}
