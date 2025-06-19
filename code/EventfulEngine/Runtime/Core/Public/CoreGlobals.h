#pragma once
#include "CoreMinimal.h"

#include "Exports/EFCoreModuleAPI.h"

namespace EventfulEngine{

    EFCORE_API EF_DECLARE_LOG_CATEGORY_EXTERN(CoreLog)

    struct GameEditorOverrides{
        /** Check if we are PIE or embedded in a window */
        bool bIsEmbedded = false;

        /** The window handle to embed the engine into */
        void* WindowHandle = nullptr;

        /** The overridden width of the embedded viewport */
        int32 WindowWidth = 0;

        /** The overridden height of the embedded viewport */
        int32 WindowHeight = 0;
    };

    extern EFCORE_API GameEditorOverrides g_gameEditorOverrides;

    extern EFCORE_API bool g_isRequestingExit;

    /**
     * This will check if a RequestExit has come in, if it has will set GIsRequestingExit.
     */
    extern EFCORE_API void BeginExitIfRequested();

    inline bool IsEngineExitRequested(){
        return g_isRequestingExit;
    }

    EFCORE_API void RequestEngineExit();

    extern EFCORE_API double g_startTime;
}
