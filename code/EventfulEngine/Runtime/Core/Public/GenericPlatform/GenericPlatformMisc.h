#pragma once
#include "CoreTypes.h"

#include "CoreMacros.h"

namespace EventfulEngine{

    DECLARE_HANDLE(EFModuleHandle);

    struct GenericPlatformLibraryUtilities{
        static EFModuleHandle LoadDynamicLibrary(const EFString& path);

        static EFModuleHandle GetLibrarySymbol(EFModuleHandle handle, const char* symbol);

        static void UnloadDynamicLibrary(EFModuleHandle handle);
    };

    using EFLibraryUtilities = GenericPlatformLibraryUtilities;
} // EventfulEngine
