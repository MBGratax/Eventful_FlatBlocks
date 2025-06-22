#pragma once

#include "GenericPlatformMisc.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

//TODO: Should not be in generic platform, sinc this is a platform specific check! Works for now though.
namespace EventfulEngine{
    EFModuleHandle GenericPlatformLibraryUtilities::LoadDynamicLibrary(const EFString& path){
#if defined(_WIN32)
        return std::bit_cast<EFModuleHandle>(::LoadLibraryA(path.c_str()));
#else
        return dlopen(path.c_str(), RTLD_LAZY);
#endif
    }

    EFModuleHandle GenericPlatformLibraryUtilities::GetLibrarySymbol(EFModuleHandle handle, const char* symbol){
#if defined(_WIN32)
        return std::bit_cast<EFModuleHandle>(::GetProcAddress(std::bit_cast<HMODULE>(handle), symbol));
#else
        return dlsym(handle, symbol);
#endif
    }

    void GenericPlatformLibraryUtilities::UnloadDynamicLibrary(EFModuleHandle handle){
        if (!handle){
            return;
        }
#if defined(_WIN32)
        ::FreeLibrary(std::bit_cast<HMODULE>(handle));
#else
        dlclose(handle);
#endif
    }
} // EventfulEngine
