#pragma once

#include <CoreMacros.h>
#include <CoreTypes.h>

#include "IManager.h"
#include "IModule.h"
#include "ModuleLoadPhase.h"
#include "PlatformMisc.h"
#include <unordered_map>
#include <string>

#if defined(_WIN32)
#   define EF_MODULE_EXPORT __declspec(dllexport)
#else
#   define EF_MODULE_EXPORT __attribute__((visibility("default")))
#endif

namespace EventfulEngine{

    class ModuleManager : public IManager<ModuleManager>{
    public:
        using ModuleFactory = IModule*(*)();

        /** Register a statically linked module instance. */
        void RegisterStaticModule(const std::string& name,
                                  E_ModuleLoadPhase phase,
                                  IModule* instance);

        /** Register a dynamically loaded module. */
        void RegisterDynamicModule(const EFString& name,
                                   E_ModuleLoadPhase phase,
                                   const std::string_view& libraryPath);

        /** Load a single module by name. */
        bool LoadModule(const std::string& name);

        /** Load all modules registered for a given phase. */
        bool LoadModules(E_ModuleLoadPhase phase);

        /** Unload a single module by name. */
        bool UnloadModule(const std::string& name);

        /** Unload all modules for a given phase. */
        bool UnloadModules(E_ModuleLoadPhase phase);

        /** Reload a single module including dependent phases. */
        bool ReloadModule(const std::string& name);

        /** Retrieve a module instance if loaded. */
        IModule* GetModule(const std::string& name);

        /** Retrieve a module instance cast to a specific type. */
        template <typename T>
        T* GetModuleAs(const std::string& name){
            return dynamic_cast<T*>(GetModule(name));
        }

        /** Check if a module with the given name has been registered. */
        [[nodiscard]] bool IsModuleRegistered(const std::string& name) const;

        /** Shutdown and unload all modules. */
        void Shutdown();

    private:
        struct ModuleEntry{
            E_ModuleLoadPhase Phase{E_ModuleLoadPhase::Startup};
            bool bIsDynamic{false};
            EFString Library;
            EFModuleHandle Handle{nullptr};
            ModuleFactory Factory{nullptr};
            IModule* Instance{nullptr};
            bool bIsLoaded{false};
        };

        std::unordered_map<EFString, ModuleEntry> _modules;
    };

    /**
     * Convenience macro to implement a module. This exports a creation
     * function for dynamically loaded modules and registers the module with
     * the ModuleManager when linked statically.
     */

#define IMPLEMENT_MODULE(ModuleImplClass, ModuleName) \
    static_assert(std::is_base_of<EventfulEngine::IModule, ModuleImplClass>::value, "IMPLEMENT_MODULE requires ModuleImplClass to derive from IModule"); \
    extern "C" EF_MODULE_EXPORT EventfulEngine::IModule* Create##ModuleName() \
    { \
        static ModuleImplClass ModuleInstance; \
        return &ModuleInstance; \
    } \
    namespace \
    { \
        struct ModuleName##AutoRegister \
        { \
            ModuleName##AutoRegister() \
            { \
                EventfulEngine::ModuleManager& Manager = EventfulEngine::ModuleManager::Get(); \
                if (!Manager.IsModuleRegistered(#ModuleName)) \
                { \
                    Manager.RegisterStaticModule( \
                        #ModuleName, \
                        ModuleInstance.GetLoadPhase(), \
                        &ModuleInstance); \
                } \
            } \
        }; \
        static ModuleName##AutoRegister AutoRegister##ModuleName; \
    }
} // namespace EventfulEngine
