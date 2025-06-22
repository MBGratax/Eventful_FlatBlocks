#pragma once

#include "ModuleManager.h"
#include "IModule.h"
#include "CoreGlobals.h"
#include "EFText.h"

#include <array>
#include <ranges>

namespace EventfulEngine{
    using enum E_ModuleLoadPhase;
    static constexpr std::array<E_ModuleLoadPhase, 7> PhaseOrder{
            PreInit,
            Core,
            Startup,
            EditorStartup,
            Editor,
            GameSessionInit,
            GameSession
        };

    void ModuleManager::RegisterStaticModule(const EFString& name, const E_ModuleLoadPhase phase, IModule* instance){
        ModuleEntry entry{};
        entry.Phase = phase;
        entry.bIsDynamic = false;
        entry.Instance = instance;
        _modules.insert_or_assign(name, std::move(entry));
    }

    void ModuleManager::RegisterDynamicModule(const EFString& name, const E_ModuleLoadPhase phase,
                                              const std::string_view& libraryPath){
        ModuleEntry entry{};
        entry.Phase = phase;
        entry.bIsDynamic = true;
        entry.Library = libraryPath;
        _modules.insert_or_assign(name, std::move(entry));
    }

    bool ModuleManager::LoadModule(const EFString& name){
        const auto efModule = _modules.find(name);
        if (efModule == _modules.end()){
            return false;
        }

        ModuleEntry& entry = efModule->second;
        if (entry.bIsLoaded){
            return true;
        }

        if (entry.bIsDynamic){
            entry.Handle = EFLibraryUtilities::LoadDynamicLibrary(entry.Library);
            if (!entry.Handle){
                EF_LOG(CoreLog, err, EFText::Format("Failed to load module '{}'", name));
                return false;
            }

            const std::string symbol = "Create" + name;
            entry.Factory = std::bit_cast<ModuleFactory>(EFLibraryUtilities::GetLibrarySymbol(
                entry.Handle, symbol.c_str()));
            if (!entry.Factory){
                EFLibraryUtilities::UnloadDynamicLibrary(entry.Handle);
                entry.Handle = nullptr;
                EF_LOG(CoreLog, err, EFText::Format("Failed to find module factory for '{}'!", name));
                return false;
            }

            entry.Instance = entry.Factory();
        }

        if (entry.Instance){
            entry.Instance->StartupModule();
            entry.bIsLoaded = true;
            EF_LOG(CoreLog, info, EFText::Format("Loaded Module '{}'", name));
            return true;
        }

        return false;
    }

    bool ModuleManager::LoadModules(const E_ModuleLoadPhase phase){
        bool success = true;
        for (auto const& [name, entry] : _modules){
            if (entry.Phase == phase){
                success &= LoadModule(name);
            }
        }
        return success;
    }

    bool ModuleManager::UnloadModule(const std::string& name){
        auto efModule = _modules.find(name);
        if (efModule == _modules.end())
            return false;

        ModuleEntry& entry = efModule->second;
        if (!entry.bIsLoaded)
            return true;

        if (entry.Instance){
            entry.Instance->PreUnloadCallback();
            if (entry.Instance->SupportsAutomaticShutdown())
                entry.Instance->ShutdownModule();
        }

        if (entry.bIsDynamic){
            EFLibraryUtilities::UnloadDynamicLibrary(entry.Handle);
            entry.Handle = nullptr;
            entry.Instance = nullptr;
            entry.Factory = nullptr;
        }

        entry.bIsLoaded = false;
        EF_LOG(CoreLog, info, EFText::Format("Unloaded module '{}'", name));
        return true;
    }

    bool ModuleManager::UnloadModules(const E_ModuleLoadPhase phase){
        bool success = true;
        for (const auto& [name, entry] : _modules){
            if (entry.Phase == phase){
                success &= UnloadModule(name);
            }
        }
        return success;
    }

    bool ModuleManager::ReloadModule(const EFString& name){
        const auto efModule = _modules.find(name);
        if (efModule == _modules.end()){
            return false;
        }

        const E_ModuleLoadPhase phase = efModule->second.Phase;

        // TODO: This should probably be async at some point
        for (const auto unloadPhase : std::ranges::reverse_view(PhaseOrder)){
            if (unloadPhase >= phase){
                UnloadModules(unloadPhase);
            }
        }

        for (const auto loadPhase : PhaseOrder){
            if (loadPhase >= phase){
                LoadModules(loadPhase);
            }
        }

        return true;
    }

    IModule* ModuleManager::GetModule(const EFString& name){
        if (const auto efModule = _modules.find(name); efModule != _modules.end()){
            return efModule->second.Instance;
        }
        return nullptr;
    }

    bool ModuleManager::IsModuleRegistered(const EFString& name) const{
        return _modules.contains(name);
    }

    void ModuleManager::Shutdown(){
        for (auto& entry : _modules | std::views::values){
            if (entry.bIsLoaded && entry.Instance){
                entry.Instance->PreUnloadCallback();
                entry.Instance->ShutdownModule();

                if (entry.bIsDynamic){
                    EFLibraryUtilities::UnloadDynamicLibrary(entry.Handle);
                    entry.Handle = nullptr;
                }
                entry.bIsLoaded = false;
            }
        }
    }
} // EventfulEngine
