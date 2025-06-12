#pragma once

#include "../Public/CoreGlobals.h"

namespace EventfulEngine{

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
