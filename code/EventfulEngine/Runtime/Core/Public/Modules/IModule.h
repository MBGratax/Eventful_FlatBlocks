#pragma once

#include "CoreMinimal.h"

namespace EventfulEngine{
    /*!
     * @brief Interface class to define an entry point for all modules. Initializes modules and provides important callbacks
     */
    class IModule{
    public:
        virtual ~IModule(){
        }


        virtual void StartupModule(){
        }


        virtual void PreUnloadCallback(){
        }


        virtual void PostLoadCallback(){
        }


        virtual void ShutdownModule(){
        }


        virtual bool SupportsDynamicReloading(){
            return true;
        }


        virtual bool SupportsAutomaticShutdown(){
            return true;
        }
    };
}
