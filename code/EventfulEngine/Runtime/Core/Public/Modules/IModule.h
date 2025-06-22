#pragma once

#include <ModuleLoadPhase.h>

namespace EventfulEngine{
    /*!
     * @brief Interface class to define an entry point for all modules. Initializes modules and provides important callbacks
     */
    class IModule{
    public:
        virtual ~IModule(){
            // Virtual destructor to free stuff once the module is destroyed (End of unload)
        }


        virtual void StartupModule(){
            // Optional Method for Modules to do something once the module is started.
        }


        virtual void PreUnloadCallback(){
            // Optional Method for Modules that support dynamic loading. Use this to free stuff before unload.
        }


        virtual void PostLoadCallback(){
            // Optional Method for Modules to do something once loaded.
        }


        virtual void ShutdownModule(){
            /// Optional Method for Modules to do something before shutdown. Note that shutdown is not called on unload,
            /// rather when the Loadphase specified in GetLoadPhase() ends.
        }


        virtual bool SupportsDynamicReloading(){
            return true;
        }


        virtual bool SupportsAutomaticShutdown(){
            return true;
        }

        virtual E_ModuleLoadPhase GetLoadPhase(){
            return E_ModuleLoadPhase::Startup;
        }
    };
}
